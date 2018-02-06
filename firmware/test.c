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
	RCC_CFGR |= RCC_CFGR_SW_PLL << RCC_CFGR_SW_SHIFT;

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

static void test_leds(void)
{
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3 | GPIO4 | GPIO5);
	gpio_clear(GPIOB, GPIO3 | GPIO4 | GPIO5);

	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2 | GPIO10);
	gpio_set(GPIOB, GPIO1);
	gpio_set(GPIOA, GPIO2 | GPIO10);

	delay_ms(1000);

	gpio_init();
}

static void testmode_result(uint8_t code)
{
	struct can_msg msg;

	msg.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 42);
	msg.len = 1;
	msg.data[0] = code;
	can_send(&msg);
}

void main(void)
{
	SCB_VTOR = 0x20000000;

	init_clocktree();
	systick_init();
	config_init();
	rtc_init();
	gpio_init();

	test_leds();

	usart_init();
	comp_init();
	adc_init();
	pwm_in_init();
	hrtim_init();
	can_if_init();
	pid_init();

	usart_puts("Ohai\n");

	struct can_msg msg = {
		.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 0),
		.len = 8,
		.data = "testmode"
	};

	can_send(&msg);

	uint8_t pwm;
	pwm = pwm_in_get_value();
	if (!(pwm >= 100 && pwm <= 120)) {
		while (1) {
			pwm = pwm_in_get_value();
			msg.id = CAN_ID_LOCAL_EVENT(BOARD_ID, 42);
			msg.len = 2;
			msg.data[0] = 1;
			msg.data[1] = pwm;
			can_send(&msg);
		}

		while (1)
			;
	}

	// test off current
	pid_set_target(CHANNEL_WARM, 0);
	pid_set_target(CHANNEL_COLD, 0);
	delay_ms(1000);

	if (adc_last_sample[CHANNEL_WARM] > 100 || adc_last_sample[CHANNEL_COLD] > 100) {
		testmode_result(2);
		while (1)
			;
	}

	lamp_set_brightness(0);
	lamp_set_color(3000);

	for (int i = 0; i <= 255; i++) {
		lamp_set_brightness(i);
		delay_ms(10);
	}

	delay_ms(1000);

	for (int i = 3000; i <= 6000; i++) {
		lamp_set_color(i);
		delay_ms(1);
	}

	delay_ms(1000);

	for (int i = 255; i >= 0; i--) {
		lamp_set_brightness(i);
		delay_ms(10);
	}

	// off
	pid_set_target(CHANNEL_WARM, 0);
	pid_set_target(CHANNEL_COLD, 0);
	delay_ms(1000);

	// trigger overcurrent (1.4 A)
	pid_set_target(CHANNEL_WARM, 4095);
	pid_set_target(CHANNEL_COLD, 0);
	delay_ms(1000);

	pid_set_target(CHANNEL_WARM, 0);
	pid_set_target(CHANNEL_COLD, 4095);
	delay_ms(1000);

	testmode_result(0xFF);

	// done
	scb_reset_system();
	while (1)
		;
}
