#include <stdint.h>

#include <libopencm3/cm3/assert.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <stm/dac.h>
#include <stm/comp.h>

#include "comp.h"
#include "config.h"

/*
 * Pin mapping
 * -----------
 *
 * Warm overcurrent:
 *  PB0 / COMP4_INP
 *
 * Cold overcurrent:
 *  PB11 / COMP6_INP
 *
 * Overcurrent negative input: DAC1 Channel 1
 *
 * Common overvoltage:
 *  PA7 / COMP2_INP
 *  Negative input: DAC2 Channel 1
 */

static void init_dac(void)
{
	rcc_periph_clock_enable(RCC_DAC1);
	rcc_periph_reset_pulse(RST_DAC1);

	rcc_periph_clock_enable(RCC_DAC2);
	rcc_periph_reset_pulse(RST_DAC2);

	// TODO: is routing the DACs to the outputs really necessary?
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4 | GPIO6);

	DAC1->CR = DAC_CR_EN1;
	DAC2->CR = DAC_CR_EN1 | DAC_CR_OUTEN1;
}

static void init_comp(void)
{
	/*
	 * We don't do a reset here, as the comperators share their RCC config
	 * with the SYSCFG block, which is partially configured by strapping
	 * pins. Not sure what would happen if we reset it.
	 *
	 * As we completely define the state of the comperators below anyway,
	 * leaving this out shouldn't hurt.
	 */

	rcc_periph_clock_enable(RCC_SYSCFG);

	// inputs
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO7);
	gpio_mode_setup(GPIOB, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO11);

	// outputs
	gpio_set_af(GPIOA, GPIO_AF8, GPIO2 | GPIO10);
	gpio_set_af(GPIOB, GPIO_AF8, GPIO1);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO10);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO1);

	COMP2->CSR = (0x1 << 22) | // DAC2_CH1
	             COMP_CSR_COMPxEN |
	             COMP_CSR_COMPxLOCK;

	COMP4->CSR = (0x4 << COMP_CSR_COMPxINSEL_Pos) | // DAC1_CH1
	             COMP_CSR_COMPxEN |
	             COMP_CSR_COMPxLOCK;

	COMP6->CSR = (0x4 << COMP_CSR_COMPxINSEL_Pos) | // DAC1_CH1
	             COMP_CSR_COMPxEN |
	             COMP_CSR_COMPxLOCK;
}

static void comp_set(enum comp_channel c, uint16_t val)
{
	cm3_assert(val < 4096);

	switch (c) {
		case COMP_IMAX:
			DAC1->DHR12R1 = val;
			break;
		case COMP_VMAX:
			DAC2->DHR12R1 = val;
			break;
	}
}

void comp_init(void)
{
	init_dac();
	init_comp();

	comp_set(COMP_IMAX, PROTECT_IMAX);
	comp_set(COMP_VMAX, PROTECT_VMAX);
}
