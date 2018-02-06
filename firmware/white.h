#pragma once

enum white_channel {
	CHANNEL_WARM,
	CHANNEL_COLD
};

void white_init(void);
void white_set_channel(enum white_channel channel, uint16_t val);
