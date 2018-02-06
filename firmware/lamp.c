#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "lamp.h"
#include "constants.h"
#include "pid.h"
#include "config.h"
#include "rtc.h"
#include "usart.h"
#include "can.h"
#include "systick.h"

static float alpha;
static uint16_t brightness;
static bool autocolor = false;

bool lamp_off;

static void lamp_update(void)
{
	uint16_t w, c;

	w = alpha * brightness;
	c = (1 - alpha) * brightness;

	if (brightness) {
		if (!w)
			w = 1;
		if (!c)
			c = 1;
	}

	pid_set_target(CHANNEL_WARM, w);
	pid_set_target(CHANNEL_COLD, c);
}

void lamp_set_color(uint16_t color)
{
	if (color > 6000)
		color = 6000;
	else if (color < 3000)
		color = 3000;

	float T = color / 1000.0f;

	/* Use a cubic polynomial to fit the temperature (in kilokelvin,
	 * between 3000K and 6000K) to an alpha value between 0 (cold) and 1
	 * (warm):
	 *
	 *  alpha(1000 * T) = 5.19265976 - 2.33931606 * T + 0.38151256 * T**2 - 0.02268211 * T**3
	 *
	 * We evaluate this using Horner's method for stability.
	 */

	alpha = 5.19265976f + T * (-2.33931606f + T * (0.38151256f - 0.02268211f * T));

	if (alpha < 0.0f)
		alpha = 0.0f;

	if (alpha > 1.0f)
		alpha = 1.0f;

	lamp_update();
}

static uint16_t gamma[256] = {
	0, 1, 1, 1, 1, 2, 2, 2,
	3, 3, 4, 4, 5, 5, 6, 7,
	8, 8, 9, 10, 11, 12, 13, 15,
	16, 17, 18, 20, 21, 23, 24, 26,
	28, 29, 31, 33, 35, 37, 39, 41,
	43, 46, 48, 51, 53, 56, 58, 61,
	64, 67, 70, 73, 76, 79, 82, 86,
	89, 93, 96, 100, 104, 108, 112, 116,
	120, 124, 128, 132, 137, 142, 146, 151,
	156, 161, 166, 171, 176, 181, 187, 192,
	198, 203, 209, 215, 221, 227, 233, 239,
	246, 252, 259, 265, 272, 279, 286, 293,
	300, 307, 315, 322, 330, 337, 345, 353,
	361, 369, 377, 386, 394, 403, 411, 420,
	429, 438, 447, 456, 466, 475, 485, 494,
	504, 514, 524, 534, 544, 555, 565, 576,
	587, 598, 608, 620, 631, 642, 654, 665,
	677, 689, 701, 713, 725, 737, 749, 762,
	775, 788, 800, 814, 827, 840, 853, 867,
	881, 895, 909, 923, 937, 951, 966, 980,
	995, 1010, 1025, 1040, 1055, 1071, 1086, 1102,
	1118, 1134, 1150, 1166, 1182, 1199, 1215, 1232,
	1249, 1266, 1283, 1301, 1318, 1336, 1353, 1371,
	1389, 1407, 1426, 1444, 1463, 1482, 1500, 1519,
	1539, 1558, 1577, 1597, 1617, 1637, 1657, 1677,
	1697, 1718, 1738, 1759, 1780, 1801, 1822, 1843,
	1865, 1887, 1908, 1930, 1953, 1975, 1997, 2020,
	2042, 2065, 2088, 2112, 2135, 2158, 2182, 2206,
	2230, 2254, 2278, 2303, 2327, 2352, 2377, 2402,
	2427, 2452, 2478, 2504, 2529, 2555, 2582, 2608,
	2634, 2661, 2688, 2715, 2742, 2769, 2797, 2824,
	2852, 2880, 2908, 2936, 2965, 2993, 3022, 3051,
};

void lamp_set_brightness(uint8_t luminance)
{
	lamp_off = luminance == 0;

	brightness = gamma[luminance];

	lamp_update();
}

static void lamp_set_color_by_time(uint32_t now)
{
	struct config_entry *p = config_get(CONFIG_KEY_TIMELINE);
	struct color_segment *timeline;
	uint16_t temp = SAFE_COLOR;
	int i;
	int32_t dt, dc;

	if (!p)
		goto out;

	if (p->len != COLOR_TIMELINE_POINTS * sizeof(struct color_segment))
		goto out;

	timeline = (struct color_segment*)p->data;

	if (timeline[0].time != 0)
		goto out;

	for (i = 1; ; i++) {
		if (i == COLOR_TIMELINE_POINTS || timeline[i].time == 0xFFFFFFFF) {
			dt = 24*60*60 - timeline[i-1].time;
			dc = timeline[0].color - timeline[i-1].color;
			break;
		}

		if (now < timeline[i].time) {
			dt = timeline[i].time - timeline[i-1].time;
			dc = timeline[i].color - timeline[i-1].color;
			break;
		}
	}

	if (dt) {
		float m = (float)dc / (float)dt;
		float c = timeline[i-1].color - m*timeline[i-1].time;

		temp = now * m + c;
	} else {
		temp = timeline[i-1].color;
	}

out:
	lamp_set_color(temp);
}

void lamp_trigger_autocolor(void)
{
	if (!autocolor)
		return;

	if (!rtc_isset()) {
		lamp_set_color(SAFE_COLOR);
		return;
	}

	lamp_set_color_by_time(gettimeofday());
}

void tim3_isr(void)
{
	TIM3_SR = ~TIM_SR_UIF;

	lamp_trigger_autocolor();
}

void lamp_start_autocolor(void)
{
	autocolor = true;

	rcc_periph_clock_enable(RCC_TIM3);
	rcc_periph_reset_pulse(RST_TIM3);

	nvic_enable_irq(NVIC_TIM3_IRQ);
	nvic_set_priority(NVIC_TIM3_IRQ, 9 << 4);

	// triggers about once every 10 seconds
	TIM3_PSC = 36000;
	TIM3_ARR = 20000;
	TIM3_CNT = TIM3_ARR - 1;
	TIM3_DIER |= TIM_DIER_UIE;
	TIM3_CR1 |= TIM_CR1_CEN;
}

void lamp_stop_autocolor(void)
{
	autocolor = false;

	TIM3_CR1 &= ~TIM_CR1_CEN;
}
