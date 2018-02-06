#pragma once

#include <stdint.h>

void usart_init(void);
void usart_putc(char c);
void usart_puts(const char *s);
void usart_print_int(int32_t i);
void usart_print_hex(uint32_t i);
int16_t usart_recvc(void);
