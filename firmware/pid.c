#include <stdint.h>

#include "constants.h"
#include "hrtim.h"
#include "pid.h"

#include "systick.h"
#include "usart.h"

struct pid_state {
	uint16_t target;
	uint16_t output;
};

struct pid_state pid_state[NUM_CHANNELS];

void pid_set_target(enum channel channel, uint16_t val)
{
	pid_state[channel].target = val;
}

void pid_run(enum channel channel, uint16_t measurement)
{
	struct pid_state *pid = &pid_state[channel];

	if (pid->target == 0) {
		hrtim_set_channel(channel, 0);
		return;
	}

	// Cannot wrap, as both inputs are 16 bit
	int32_t error = pid->target - measurement;
	int32_t out = pid->output + 2 * error / 10;

	if (out < 0)
		out = 0;

	if (out > 0xFFFF)
		out = 0xFFFF;

	pid->output = out;

	hrtim_set_channel(channel, pid->output + 3000);
}

static void init_pid_state(struct pid_state *pid)
{
	pid->target = 0;
	pid->output = 0;
}

void pid_init(void)
{
	init_pid_state(&pid_state[CHANNEL_WARM]);
	init_pid_state(&pid_state[CHANNEL_COLD]);
}
