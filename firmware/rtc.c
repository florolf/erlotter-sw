#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <stm/rtc.h>

#include "rtc.h"

#define DEC2BCD(x) ( ((x/10) << 4) | (x % 10) )
#define BCD2DEC(x) ( (((x >> 4) & 0x0F) * 10) + (x & 0x0F) )

void rtc_set(uint8_t h, uint8_t m, uint8_t s)
{
	if (h > 23 || m > 59 || s > 59)
		return;

	// unlock rtc
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// enable initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF))
		;

	/*
	 * The clock input is HSE/32 = 250 kHz. We want the async prescaler to
	 * be as high as possible, as per the reference manual. To get to 1 Hz
	 * at the output, we first divide by 124, yielding 2 kHz and then by
	 * 2000.
	 */

	RTC->PRER =  123 << RTC_PRER_PREDIV_A_Pos |
	            1999 << RTC_PRER_PREDIV_S_Pos;

	RTC->TR = DEC2BCD(h) << RTC_TR_HU_Pos |
	          DEC2BCD(m) << RTC_TR_MNU_Pos |
	          DEC2BCD(s) << RTC_TR_SU_Pos;

	/* we don't need the date, but to detect an initialized RTC after
	 * reset, we set it to a non-null value */
	RTC->DR = 0x00168211;

	RTC->ISR &= ~RTC_ISR_INIT;

	// lock rtc
	RTC->WPR = 0;

	while (!(RTC->ISR & RTC_ISR_RSF))
		;
}

uint32_t gettimeofday(void)
{
	uint32_t ts;

	ts = RTC->TR;

	return BCD2DEC(ts >> RTC_TR_SU_Pos) +
	       60 * (BCD2DEC(ts >> RTC_TR_MNU_Pos) +
	             60 * BCD2DEC(ts >> RTC_TR_HU_Pos));
}

bool rtc_isset(void)
{
	return RTC->ISR & RTC_ISR_INITS;
}

void rtc_init(void)
{
	rcc_periph_clock_enable(RCC_PWR);

	// make backup domain writable
	PWR_CR |= PWR_CR_DBP;
	while (!(PWR_CR & PWR_CR_DBP))
		;

	// enable RTC
	RCC_BDCR = 0x3 << 8; // use HSE/32 as clock
	RCC_BDCR |= RCC_BDCR_RTCEN;
}
