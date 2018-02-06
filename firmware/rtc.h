#pragma once

#include <stdint.h>

void rtc_init(void);
bool rtc_isset(void);
void rtc_set(uint8_t h, uint8_t m, uint8_t s);
uint32_t gettimeofday(void);
