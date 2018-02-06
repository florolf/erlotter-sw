#pragma once

#include <stdint.h>
#include "constants.h"

void pid_init(void);

void pid_set_target(enum channel channel, uint16_t val);
void pid_run(enum channel, uint16_t measurement);
