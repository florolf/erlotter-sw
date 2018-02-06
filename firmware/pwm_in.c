#include <stdbool.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "pwm_in.h"

#include "constants.h"
#include "systick.h"
#include "usart.h"

/*
 * Pin mapping
 * -----------
 *  PWM in: PA15 / TIM2_CH1
 */

static bool pwm_idle = false;
static bool pwm_state;

void tim2_isr(void)
{
	if (TIM2_SR & TIM_SR_CC4IF) {
		TIM2_CR1 &= ~TIM_CR1_CEN;
		TIM2_SR = ~TIM_SR_CC4IF; // ack interrupt

		gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);

		pwm_idle = true;
		pwm_state = gpio_get(GPIOA, GPIO15);

		gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);

		/* enable one-shot interrupt for CC1 so that we can detect the
		 * resumption of PWM
		 */
		TIM2_DIER |= TIM_DIER_CC1IE;
		TIM2_CR1 |= TIM_CR1_CEN;
	}

	if (TIM2_SR & TIM_SR_CC1IF) {
		pwm_idle = false;

		TIM2_DIER &= ~TIM_DIER_CC1IE;
		TIM2_SR = ~TIM_SR_CC1IF; // ack interrupt
	}
}

void pwm_in_init(void)
{
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO15);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO15);

	rcc_periph_clock_enable(RCC_TIM2);
	rcc_periph_reset_pulse(RST_TIM2);

	TIM2_CCMR1 = TIM_CCMR1_CC1S_IN_TI1 | TIM_CCMR1_CC2S_IN_TI1;
	TIM2_CCER = TIM_CCER_CC2P;
	TIM2_SMCR = TIM_SMCR_TS_TI1FP1 | TIM_SMCR_SMS_RM;
	TIM2_CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	/*
	 * The PWM capture logic won't handle 0% and 100% duty cycles properly.
	 * Instead, after a certain timeout (100 ms) we fire an interrupt, the
	 * handler of which will then determine the state of the input.
	 */

	nvic_enable_irq(NVIC_TIM2_IRQ);
	nvic_set_priority(NVIC_TIM2_IRQ, 9 << 4);
	TIM2_CCR4 = F_SYSCLK / 10;
	TIM2_DIER |= TIM_DIER_CC4IE;

	TIM2_CR1 |= TIM_CR1_CEN;
}

uint8_t pwm_in_get_value(void)
{
	if (pwm_idle)
		return pwm_state ? 255 : 0;

	return (uint8_t)(((uint32_t)TIM2_CCR2 * 255) / TIM2_CCR1);
}
