#pragma once

#define F_SYSCLK (72ULL * 1000 * 1000)
#define F_TIM F_SYSCLK

#define F_HRTIM (2 * F_SYSCLK)
#define F_HRTIM_DLL (32 * F_HRTIM)

enum channel {
	CHANNEL_WARM,
	CHANNEL_COLD,

	NUM_CHANNELS
};
