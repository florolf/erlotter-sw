#include <stdint.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "systick.h"
#include "constants.h"

volatile uint32_t ticks = 0;

void systick_init(void)
{
	systick_set_frequency(HZ, F_SYSCLK);

	systick_interrupt_enable();
	systick_counter_enable();
	nvic_set_priority(NVIC_SYSTICK_IRQ, 0 << 4);
}

void delay_ms(uint16_t ms)
{
	delay_ticks(ms * (HZ / 1000));
}

void delay_ticks(uint16_t cnt)
{
	uint32_t timeout = ticks + cnt;

	while(time_before(ticks, timeout))
		;
}

void sys_tick_handler(void)
{
	ticks++;
}
