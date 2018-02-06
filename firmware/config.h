#pragma once

#include <stdint.h>
#include <stdbool.h>

#define BOARD_ID (*(uint32_t*)0x8000008)

#define PROTECT_IMAX 4095
#define PROTECT_VMAX 4095

#define SAFE_COLOR 4500
#define HEARTBEAT_INTERVAL 300

struct config_entry {
	uint16_t key;
	uint16_t len;
	uint8_t data[];
};

#define CONFIG_KEY_TIMELINE 1

void config_init(void);
struct config_entry *config_get(uint16_t key);

struct config_entry *config_new(uint16_t key, uint16_t len);
void config_clear(void);
void config_commit(void);
