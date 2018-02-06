#pragma once

#include <stdint.h>

#include "constants.h"

void hrtim_init(void);
void hrtim_set_channel(enum channel channel, uint16_t value);
