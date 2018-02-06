#pragma once

void adc_init(void);

#ifdef MODE_test
#include <stdint.h>
#include "constants.h"

extern volatile uint32_t adc_last_sample[NUM_CHANNELS];
#endif
