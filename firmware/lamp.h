#pragma once

#include <stdint.h>

struct color_segment {
	uint32_t time;
	uint16_t color;
};

#define COLOR_TIMELINE_POINTS 20

extern bool lamp_off;

void lamp_set_color(uint16_t color);
void lamp_set_brightness(uint8_t brightness);

void lamp_start_autocolor(void);
void lamp_stop_autocolor(void);
void lamp_trigger_autocolor(void);
