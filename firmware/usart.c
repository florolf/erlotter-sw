#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "usart.h"

void usart_init(void)
{
	rcc_periph_clock_enable(RCC_USART1);
	rcc_periph_reset_pulse(RST_USART1);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO12);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO12);

	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
	gpio_set_af(GPIOB, GPIO_AF7, GPIO6 | GPIO7);

	USART1_BRR = 313; // 36 MHz / 313 = ~115200 Bd
	USART1_CR1 |= USART_CR1_TE | USART_CR1_RE;
	USART1_CR1 |= USART_CR1_UE;
}

void usart_putc(char c)
{
	usart_send_blocking(USART1, c);
}

int16_t usart_recvc(void)
{
	if (!(USART1_ISR & USART_ISR_RXNE))
		return -1;

	return USART1_RDR & USART_RDR_MASK;
}

void usart_puts(const char *s)
{
	while (*s) {
		if (*s == '\n')
			usart_putc('\r');

		usart_putc(*s);

		s++;
	}
}

void usart_print_int(int32_t i)
{
	char buf[16];
	char *p = buf + sizeof(buf) - 1;
	int neg = 0;

	if (i < 0) {
		neg = 1;
		i = -i;
	}

	*p-- = 0;

	do {
		*p-- = '0' + (i % 10);
		i /= 10;
	} while(i);

	if (neg)
		usart_putc('-');

	usart_puts(p + 1);
}

void usart_print_hex(uint32_t i)
{
	char buf[9];
	int idx;

	buf[8] = 0;

	for (idx = 7; idx >= 0; idx--) {
		uint8_t nibble = i & 0xF;

		if (nibble >= 10)
			buf[idx] = 'A' + nibble - 10;
		else
			buf[idx] = '0' + nibble;

		i >>= 4;
	}

	usart_puts(buf);
}
