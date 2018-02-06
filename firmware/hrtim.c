#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/hrtim.h>

#include "hrtim.h"
#include "constants.h"
#include "systick.h"
#include "can.h"
#include "config.h"

/*
 * Pin mapping
 * -----------
 *
 * Channel 1 (Warm):
 *  - SW: PA8 / HRTIM_CHA1
 *  - !SW: PA9 / HRTIM_CHA2
 *
 * Channel 2 (Cold):
 *  - SW: PB14 / HRTIM1_CHD1
 *  - !SW: PB15 / HRTIM1_CHD2
 *
 * Fault output: PB4 / LED2
 */

#define HRTIM_PWM_FREQ 281250

#define HRTIM_PERIOD (F_HRTIM_DLL / HRTIM_PWM_FREQ)

static void hrtim_calibrate(void)
{
	HRTIM_ICR |= HRTIM_ICR_DLLRDYC;

	HRTIM_DLLCR |= HRTIM_DLLCR_CAL;

	while (!(HRTIM_ISR & HRTIM_ISR_DLLRDY))
		;

	HRTIM_DLLCR |= HRTIM_DLLCR_CALRTE_1048576 |
	               HRTIM_DLLCR_CALEN;
}

static void _set_hrtim_channel(enum channel ch, uint16_t val) {
	uint16_t setval = val;

	/*
	 * When we have simultaneous sets and resets, resets win, thus we clamp
	 * the reset point below the set point (= HRTIM_PERIOD) to avoid a
	 * large value resulting in no output
	 */
	if (val >= HRTIM_PERIOD)
		setval = HRTIM_PERIOD - 1;

	// values below 0x60 are forbidden for CKPSC == 0
	if (val < 0x60)
		setval = 0x60;

	// map zero to HRTIM_PERIOD to achive true disabling
	if (val == 0)
		setval = HRTIM_PERIOD;

	if (ch == CHANNEL_WARM)
		HRTIM_TIMx_CMP1(HRTIM_TIMA) = setval;
	else
		HRTIM_TIMx_CMP1(HRTIM_TIMD) = setval;
}

void hrtim_set_channel(enum channel ch, uint16_t val) {
	// prevent low-side shootthrough
	uint32_t max_duty = (HRTIM_PERIOD * 95ull)/100ull;
	if (val > max_duty)
		val = max_duty;

	_set_hrtim_channel(ch, val);

	/* update ADC triggers to trigger in the middle of the longest idle
	 * phase during a cycle to avoid picking up switching noise */

	uint32_t halfpoint;
	if (val > HRTIM_PERIOD / 2)
		halfpoint = val / 2;
	else
		halfpoint = (HRTIM_PERIOD + val) / 2;

	if (ch == 0)
		HRTIM_TIMx_CMP2(HRTIM_TIMA) = halfpoint;
	else
		HRTIM_TIMx_CMP2(HRTIM_TIMD) = halfpoint;
}

static void setup_timer_common(uint8_t tim)
{
	HRTIM_TIMx_TIMCR(tim) =
		HRTIM_TIMx_CR_PREEN |
		HRTIM_TIMx_CR_CONT |
		HRTIM_TIMx_CR_TxREPU;

	HRTIM_TIMx_DT(tim) = HRTIM_TIMx_DT_DTFLKx | HRTIM_TIMx_DT_DTFSLKx |
	                     HRTIM_TIMx_DT_DTRLKx | HRTIM_TIMx_DT_DTRSLKx |
	                     100 << HRTIM_TIMx_DT_DTFx_SHIFT |
	                     100 << HRTIM_TIMx_DT_DTRx_SHIFT;

	HRTIM_TIMx_OUT(tim) = HRTIM_TIMx_OUT_FAULT1_ACTIVE |
	                      HRTIM_TIMx_OUT_FAULT2_INACTIVE |
	                      HRTIM_TIMx_OUT_DTEN;

	/* We want high CMP values to correspond to high output voltages. Thus
	 * we need a long duty cycle of output 2 (low side) in that case.
	 * While mucking about with sets and resets causes weird corner cases,
	 * inverting the outputs causes counter-intuitive logic in various
	 * places such as deadtime generation.
	 *
	 * So we'll live with the set/reset corner-cases (AFAICT the only
	 * problem is glitching of the low side when turning an output up to
	 * 100%, which doesn't make sense anyway and is prevented in
	 * hrtim_set_channel).
	 *
	 * There's no need to configure output 2, as it is enslaved to output 1
	 * because of deadtime generation.
	 */

	HRTIM_TIMx_SET1(tim) = HRTIM_TIMx_SETy_CMP1;
	HRTIM_TIMx_RST1(tim) = HRTIM_TIMx_RSTy_PER;

	HRTIM_TIMx_PER(tim) = HRTIM_PERIOD;
}

void hrtim_init(void)
{
	// configure GPIOs
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14 | GPIO15);

	gpio_set_af(GPIOA, GPIO_AF13, GPIO8 | GPIO9);
	gpio_set_af(GPIOB, GPIO_AF13, GPIO14 | GPIO15);

	gpio_set(GPIOB, GPIO4);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);

	// set HRTIM clock source to PLL
	RCC_CFGR3 |= 1 << 12;

	rcc_periph_clock_enable(RCC_HRTIM);
	rcc_periph_reset_pulse(RST_HRTIM);

	hrtim_calibrate();

	setup_timer_common(HRTIM_TIMA);
	setup_timer_common(HRTIM_TIMD);

#ifndef HRTIM_NOFAULT
	/*
	 * Fault routing:
	 *
	 *  Warm overcurrent -> COMP4 -> FLT2
	 *  Cold overcurrent -> COMP6 -> FLT3
	 *  Common overvoltage -> COMP2 -> FLT1
	 */
	HRTIM_FLTINR1 = HRTIM_FLTINR1_FLTxSRC(1) | HRTIM_FLTINR1_FLTxSRC(2) | HRTIM_FLTINR1_FLTxSRC(3) |
	                HRTIM_FLTINR1_FLTxP(1)   | HRTIM_FLTINR1_FLTxP(2)   | HRTIM_FLTINR1_FLTxP(3)   |
	                HRTIM_FLTINR1_FLTxE(1)   | HRTIM_FLTINR1_FLTxE(2)   | HRTIM_FLTINR1_FLTxE(3)   |
	                HRTIM_FLTINR1_FLTxLCK(1) | HRTIM_FLTINR1_FLTxLCK(2) | HRTIM_FLTINR1_FLTxLCK(3) |
	                3 << HRTIM_FLTINR1_FLTxF_SHIFT(1) |
	                3 << HRTIM_FLTINR1_FLTxF_SHIFT(2) |
	                3 << HRTIM_FLTINR1_FLTxF_SHIFT(3);

	HRTIM_TIMx_FLT(HRTIM_TIMA) = HRTIM_TIMx_FLT_FLT1EN | HRTIM_TIMx_FLT_FLT2EN | HRTIM_TIMx_FLT_FLTLCK;
	HRTIM_TIMx_FLT(HRTIM_TIMD) = HRTIM_TIMx_FLT_FLT1EN | HRTIM_TIMx_FLT_FLT3EN | HRTIM_TIMx_FLT_FLTLCK;

	HRTIM_IER |= HRTIM_IER_FLT1IE | HRTIM_IER_FLT2IE | HRTIM_IER_FLT3IE;
	nvic_enable_irq(NVIC_HRTIM_FLT_IRQ);
	nvic_set_priority(NVIC_HRTIM_FLT_IRQ, 9 << 4);
#endif

	/*
	 * ADC triggers
	 *
	 * We route triggers 1 and 2 to the warm channel (TIMA) and triggers 3
	 * and 4 to the cold channel (TIMD) so that we can both do injected and
	 * regular conversions based on those triggers (odd triggers can
	 * trigger regular conversions, even ones can trigger injected
	 * conversions).
	 *
	 * ADCs are triggerd on CMP2 matches.
	 */
	HRTIM_CR1 |= HRTIM_CR1_ADxUSRC_TIMA << HRTIM_CR1_AD1USRC_SHIFT |
	             HRTIM_CR1_ADxUSRC_TIMA << HRTIM_CR1_AD2USRC_SHIFT |
	             HRTIM_CR1_ADxUSRC_TIMD << HRTIM_CR1_AD3USRC_SHIFT |
	             HRTIM_CR1_ADxUSRC_TIMD << HRTIM_CR1_AD4USRC_SHIFT;

	HRTIM_ADC1R = HRTIM_ADC1R_AD1TAC2;
	HRTIM_ADC2R = HRTIM_ADC2R_AD2TAC2;
	HRTIM_ADC3R = HRTIM_ADC3R_AD3TDC2;
	HRTIM_ADC4R = HRTIM_ADC4R_AD4TDC2;

	hrtim_set_channel(0, 0);
	hrtim_set_channel(1, 0);
	HRTIM_CR2 = HRTIM_CR2_TASWU | HRTIM_CR2_TDSWU;

	HRTIM_MCR = HRTIM_MCR_TACEN | HRTIM_MCR_TDCEN;
	HRTIM_OENR = HRTIM_OENR_TA1OEN | HRTIM_OENR_TA2OEN |
	             HRTIM_OENR_TD1OEN | HRTIM_OENR_TD2OEN;
}

static void hrtim_handle_fault(uint8_t fault)
{
	struct can_msg msg;

	gpio_clear(GPIOB, GPIO4);

	msg.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 1);
	msg.len = 1;
	msg.data[0] = fault;
	can_send(&msg);

#ifndef MODE_test
	delay_ms(10000);
	scb_reset_system();
#endif
}

void hrtim_flt_isr(void)
{
	uint8_t fault = HRTIM_ISR & 0x7;

	/* Fault interrupts are level-triggered. Disable the interrupt to
	 * prevent being stuck in it.
	 */
	HRTIM_IER &= ~fault;

	hrtim_handle_fault(fault);

	HRTIM_ICR = fault;
}
