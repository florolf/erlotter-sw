#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencmsis/core_cm3.h>

#include "systick.h"
#include "hrtim.h"
#include "usart.h"
#include "can.h"
#include "comp.h"
#include "adc.h"
#include "pwm_in.h"
#include "rtc.h"
#include "pid.h"
#include "lamp.h"
#include "config.h"

static void init_clocktree(void)
{
	// are we on PLL already? nothing to do
	if ((RCC_CFGR & RCC_CFGR_SWS_MASK) == RCC_CFGR_SWS_PLL)
		return;

	// need 2 wait states for SYSCLK > 48 MHz
	FLASH_ACR |= FLASH_ACR_LATENCY_2WS;

	// init HSE
	//RCC_CR |= RCC_CR_HSEBYP; // HACK: discovery has no quartz
	RCC_CR |= RCC_CR_HSEON;
	while (!(RCC_CR & RCC_CR_HSERDY))
		;

	// init PLL
	// 8 MHz HSE -> 72 MHz PLLCLK/SYSCLK
	RCC_CFGR |= RCC_CFGR_PLLSRC |
	           (RCC_CFGR_PLLMUL_PLL_IN_CLK_X9 << RCC_CFGR_PLLMUL_SHIFT);

	RCC_CR |= RCC_CR_PLLON;
	while (!(RCC_CR & RCC_CR_PLLRDY))
		;

	// PCLK1 must be <= 36 MHz, divide by 2
	RCC_CFGR |= RCC_CFGR_PPRE1_DIV_2 << RCC_CFGR_PPRE1_SHIFT;

	// switch SYSCLK to PLLCLK
	RCC_CFGR = (RCC_CFGR & (~3)) | (RCC_CFGR_SW_PLL << RCC_CFGR_SW_SHIFT);

	while (((RCC_CFGR >> RCC_CFGR_SWS_SHIFT) & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL)
		;
}

static void gpio_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_reset_pulse(RST_GPIOA);

	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_reset_pulse(RST_GPIOB);
}

static uint32_t next_heartbeat;
static void heartbeat(void)
{
	if (time_before(ticks, next_heartbeat))
		return;

	struct can_msg msg = {
		.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 3),
		.len = 0,
	};

	can_send(&msg);

	next_heartbeat = ticks + HEARTBEAT_INTERVAL * HZ;
}

void main(void)
{
	SCB_VTOR = 0x8000800;

	init_clocktree();
	systick_init();
	config_init();
	rtc_init();
	gpio_init();
	usart_init();
	comp_init();
	adc_init();
	pwm_in_init();
	hrtim_init();
	can_if_init();
	pid_init();

	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);
	gpio_clear(GPIOB, GPIO3);

	lamp_set_color(SAFE_COLOR);
	lamp_set_brightness(0);

	delay_ms(100); // wait for offset compensation

	lamp_start_autocolor();

	for (int i = 0; i <= 255; i++) {
		lamp_set_brightness(i);
		delay_ms(1);
	}

	usart_puts("Ohai\n");

	struct can_msg msg = {
		.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 0),
		.len = 8,
		.data = "ohai"
	};

	*(uint32_t*)&msg.data[4] = GIT_REV;

	can_send(&msg);

	while (1) {
		heartbeat();
	}
}
