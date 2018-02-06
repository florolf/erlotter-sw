#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/stm32/flash.h>

#include "config.h"

// flash base + 2K bootloader + 12K main firmware + 2K config
#define CONFIG_FLASH_START ((uint8_t*)(0x8000000 + 2048 + 1024 * 12))
#define CONFIG_SIZE 2048

static uint8_t config_shadow[CONFIG_SIZE];

static uint32_t get_checksum(void)
{
	uint32_t checksum = 0;
	uint32_t *p = (uint32_t*)&config_shadow[8];
	int n = (CONFIG_SIZE - 8) / 4;

	while (n--)
		checksum += *p++;

	return ~checksum;
}

void config_clear(void)
{
	uint32_t checksum;

	memset(config_shadow, 0, CONFIG_SIZE);

	memcpy(&config_shadow[0], "conf", 4);

	// add terminator
	*((uint16_t*)&config_shadow[8]) = 0xFFFF;

	checksum = get_checksum();
	memcpy(&config_shadow[4], &checksum, 4);

	config_commit();
}

void config_init(void)
{
	uint32_t checksum;

	memcpy(config_shadow, CONFIG_FLASH_START, CONFIG_SIZE);

	/* If there's no magic or the checksum doesn't match, initialize a clean
	 * config state */

	checksum = get_checksum();

	if (!((memcmp(&config_shadow[0], "conf", 4) == 0) &&
	      (memcmp(&config_shadow[4], &checksum, 4) == 0)))
		config_clear();
}

static struct config_entry *_config_get(uint16_t key)
{
	uint8_t *p = &config_shadow[8];

	// limit the iteration so we don't fault because of invalid data
	while (p - config_shadow < CONFIG_SIZE) {
		struct config_entry *e = (struct config_entry*)p;

		// did we reach the end?
		if (e->key == 0xFFFF)
			return e;

		// found it?
		if (e->key == key)
			return e;

		p += sizeof(struct config_entry) + e->len;
	}

	return NULL;
}

struct config_entry *config_get(uint16_t key)
{
	struct config_entry *e;

	e = _config_get(key);

	if (e && e->key == 0xFFFF)
		return NULL;

	return e;
}

struct config_entry *config_new(uint16_t key, uint16_t len)
{
	struct config_entry *e, *term;

	e = _config_get(key);

	// no space left
	if (!e)
		return NULL;

	// entry exists
	if (e->key != 0xFFFF)
		return NULL;

	// we need to be able to fit in a config header, the data and a new termination entry
	if (CONFIG_SIZE - (uint32_t)((char*)e - (char*)config_shadow) <
	    2 * sizeof(struct config_entry) + len)
		return NULL;

	e->key = key;
	e->len = len;

	term = (struct config_entry*)((uint8_t*)e + sizeof(struct config_entry) + len);
	term->key = 0xFFFF;

	return e;
}

static void flash_wait_busy(void)
{
	__asm__ volatile ("nop");

	while ((FLASH_SR & FLASH_SR_BSY))
		;

	FLASH_SR |= FLASH_SR_EOP;
}

static void flash_erase(uint16_t *p)
{
	FLASH_CR |= FLASH_CR_PER;
	FLASH_AR = (uint32_t)p;
	FLASH_CR |= FLASH_CR_STRT;

	flash_wait_busy();

	FLASH_CR &= ~FLASH_CR_PER;
}

static void flash_write(uint16_t *p, uint16_t data)
{
	FLASH_CR |= FLASH_CR_PG;
	*p = data;

	flash_wait_busy();

	FLASH_CR &= ~FLASH_CR_PG;
}

void config_commit(void)
{
	uint32_t checksum;
	uint16_t *src = (uint16_t*)config_shadow;
	uint16_t *dst = (uint16_t*)CONFIG_FLASH_START;
	int n = CONFIG_SIZE / 2;

	// recalculate checksum
	checksum = get_checksum();
	memcpy(&config_shadow[4], &checksum, 4);

	// unlock flash programming
	FLASH_KEYR = FLASH_KEYR_KEY1;
	FLASH_KEYR = FLASH_KEYR_KEY2;

	flash_erase(dst);

	while (n--) {
		flash_write(dst, *src);

		src++;
		dst++;
	}

	// lock flash
	FLASH_CR |= FLASH_CR_LOCK;
}
