#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/flash.h>

#define TIMEOUT 100000
#define TX_TIMEOUT (TIMEOUT * 2)
#define APP_START 0x8000800
#define APP_END (0x8000800 + 12 * 1024)
#define CHIP_TYPE 0

const volatile uint32_t board_id __attribute__((section(".board_id"))) = 0;

// Jenkins One-at-a-Time hash with additive seed
static uint32_t hash(uint8_t *data, uint32_t seed, uint32_t len)
{
	uint32_t hash, i;

	hash = seed;

	for (i = 0; i < len; i++) {
		hash += data[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

static void init_clocks(void)
{
	RCC_CR |= RCC_CR_HSEON;
	while (!(RCC_CR & RCC_CR_HSERDY))
		;

	RCC_CFGR |= RCC_CFGR_SW_HSE;

	RCC_AHBENR |= RCC_AHBENR_IOPAEN | RCC_AHBENR_IOPBEN;
	RCC_APB1ENR |= RCC_APB1ENR_CANEN;
}

static void set_id(uint32_t id)
{
	// set receive filter
	CAN_FMR(BX_CAN1_BASE) |= CAN_FMR_FINIT;

	// filter 0: our ID
	// filter 1: broadcast (0x1FFFFFFF)
	CAN_FS1R(BX_CAN1_BASE) |= 0x3; // 32bit filters
	CAN_FiR1(BX_CAN1_BASE, 0) = id << CAN_RIxR_EXID_SHIFT | CAN_RIxR_IDE;
	CAN_FiR2(BX_CAN1_BASE, 0) = 0xFFFFFFFF;
	CAN_FiR1(BX_CAN1_BASE, 1) = 0x10000000 << CAN_RIxR_EXID_SHIFT | CAN_RIxR_IDE;
	CAN_FiR2(BX_CAN1_BASE, 1) = 0xFFFFFFFF;
	CAN_FA1R(BX_CAN1_BASE) |= 0x3; // enable them

	CAN_FMR(BX_CAN1_BASE) &= ~CAN_FMR_FINIT;

	// set transmit address
	CAN_TI0R(BX_CAN1_BASE) = id << CAN_TIxR_EXID_SHIFT | CAN_TIxR_IDE;
}

static void generate_id(uint32_t seed)
{
	uint32_t id;

	do {
		id = hash((uint8_t*)DESIG_UNIQUE_ID_BASE, seed, 12);
		seed++;
	} while (id == 0);

	id &=  0xFFFFFFF;
	id |= 0x10000000;

	set_id(id);
}

static void init_can(void)
{
	// configure ports
	GPIOB_MODER |= GPIO_MODE(8, GPIO_MODE_AF) |
		       GPIO_MODE(9, GPIO_MODE_AF);

	GPIOB_AFRH |= GPIO_AFR(0, GPIO_AF9) |
	              GPIO_AFR(1, GPIO_AF9);

	// configure CAN
	CAN_MCR(BX_CAN1_BASE) &= ~CAN_MCR_SLEEP;
	CAN_MCR(BX_CAN1_BASE) |= CAN_MCR_INRQ | CAN_MCR_ABOM;

	while (!(CAN_MSR(BX_CAN1_BASE) & CAN_MSR_INAK))
		;

	// BPR = 10 -> Tq = 125 ns at 8MHz -> 50 kbaud
	CAN_BTR(BX_CAN1_BASE) = CAN_BTR_SJW_1TQ | CAN_BTR_TS1_13TQ | CAN_BTR_TS2_2TQ | 9;

	set_id(board_id | 0x10000000);

	CAN_MCR(BX_CAN1_BASE) &= ~CAN_MCR_INRQ;
	while ((CAN_MSR(BX_CAN1_BASE) & CAN_MSR_INAK))
		;
}

/* We have no .data section, so these cannot be initialized */
uint32_t h, l;
uint8_t len;
uint8_t *p;

static void do_tx(void)
{
	uint32_t timeout = TX_TIMEOUT;

	CAN_TDT0R(BX_CAN1_BASE) &= ~CAN_TDTxR_DLC_MASK;
	CAN_TDT0R(BX_CAN1_BASE) |= len;
	CAN_TDH0R(BX_CAN1_BASE) = h;
	CAN_TDL0R(BX_CAN1_BASE) = l;
	CAN_TI0R(BX_CAN1_BASE) |= CAN_TIxR_TXRQ;

	while ((CAN_TI0R(BX_CAN1_BASE) & CAN_TIxR_TXRQ) && (timeout-- > 0))
		;
}

static int do_rx()
{
	if (!(CAN_RF0R(BX_CAN1_BASE) & CAN_RF0R_FMP0_MASK))
		return 0;

	len = CAN_RDT0R(BX_CAN1_BASE) & CAN_RDTxR_DLC_MASK;
	h = CAN_RDH0R(BX_CAN1_BASE);
	l = CAN_RDL0R(BX_CAN1_BASE);

	CAN_RF0R(BX_CAN1_BASE) |= CAN_RF0R_RFOM0;
	while(CAN_RF0R(BX_CAN1_BASE) & CAN_RF0R_RFOM0)
		;

	return 1;
}

static void exec(uint32_t sp, uint32_t ip)
{
	if (sp)
		__asm__ volatile ("msr msp, %0" : : "r" (sp));

	((void(*)())ip)();
}

static __attribute__ ((noreturn)) void start_app(void)
{
	FLASH_CR |= FLASH_CR_LOCK;

	exec(
		*(volatile uint32_t*)APP_START,
		*(volatile uint32_t*)(APP_START+4)
	);

	while (1)
		;
}

static void wait_busy(void)
{
	__asm__ volatile ("nop");

	while ((FLASH_SR & FLASH_SR_BSY))
		;

	FLASH_SR |= FLASH_SR_EOP;
}

static void erase(void)
{
	if (p < (uint8_t*)APP_START)
		return;

	if (p >= (uint8_t*)APP_END)
		return;

	FLASH_CR |= FLASH_CR_PER;
	FLASH_AR = (uint32_t)p;
	FLASH_CR |= FLASH_CR_STRT;

	wait_busy();

	FLASH_CR &= ~FLASH_CR_PER;
}

static void read(uint8_t cnt)
{
	len = 8;

	while (cnt) {
		l = *(uint32_t*)p;
		p += 4;
		cnt--;

		if (cnt) {
			h = *(uint32_t*)p;
			p += 4;
			cnt--;
		} else
			len = 4;

		do_tx();
	}
}

static void hash_range(uint32_t range)
{
	l = hash(p, 0, range);
	len = 4;

	do_tx();
}

static void write_flash(uint16_t w)
{
	if (p < (uint8_t*)APP_START)
		return;

	if (p >= (uint8_t*)APP_END)
		return;

	FLASH_CR |= FLASH_CR_PG;

	*(uint16_t*)p = w;
	p += 2;

	wait_busy();

	FLASH_CR &= ~FLASH_CR_PG;
}

static void write(uint8_t cnt, uint16_t w1, uint16_t w2, uint16_t w3)
{
	if (cnt >= 1)
		write_flash(w1);

	if (cnt >= 2)
		write_flash(w2);

	if (cnt >= 3)
		write_flash(w3);
}

void main(void)
{
	uint32_t reentry_magic = *(uint32_t*) 0x20000000;
	bool is_coldboot = reentry_magic != 0xdeadb007;

	if (is_coldboot) {
		init_clocks();
		init_can();
	} else {
		set_id(board_id | 0x10000000);
	}

	p = (uint8_t*)APP_START;
	l = 0x07B0E9ED;
	h = GIT_REV;
	len = 8;

	do_tx();

	if (is_coldboot) {
		bool got_cmd = false;
		for (int i = 0; i < TIMEOUT; i++) {
			if (do_rx())
				got_cmd = true;
		}

		if (!got_cmd)
			start_app();
	} else {
		while (!do_rx())
			;
	}

	FLASH_KEYR = FLASH_KEYR_KEY1;
	FLASH_KEYR = FLASH_KEYR_KEY2;

	while (1) {
		uint32_t out = 0;
		uint8_t outlen = 1;

		switch (l & 0xFF) {
			// management
			case 0x00: // get chip type
				out = CHIP_TYPE;
				break;
			case 0x01: // regenerate CAN id
				generate_id(h);
				break;
			case 0x02: // reset
				SCB_AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_SYSRESETREQ;
				while(1)
					;

				break;

			// memory operations
			case 0x10: // set pointer
				p = (uint8_t*)h;
				break;
			case 0x11: // read data
				read(h);
				break;
			case 0x12: // hash range
				hash_range(h);
				break;
			case 0x13: // write mem
				*(uint32_t*)p = h;
				p += 4;
				break;

			// flash operations
			case 0x20: // erase pages
				erase();
				break;
			case 0x21: // write pages
				write(len / 2 - 1, (l >> 16) & 0xFFFF, h & 0xFFFF, (h >> 16) & 0xFFFF);
				break;

			// execution
			case 0x30: //start application
				start_app();
				break;
			case 0x31: // execute at point
				exec(0, (uint32_t)p);
				break;
			default:
				out = 1;
				break;
		}

		l = out;
		len = outlen;
		do_tx();

		while (!do_rx())
			;
	}
}

extern char _estack;

/*
 * Here, we cheat a little: As *all* other interrupts shouldn't occur anyway
 * (and we don't have a sane way to handle them here even if they did), we
 * don't provide vectors for them, saving a lot of space.
 */

void (*nvic_table[])() __attribute__((section(".nvic_table"))) = {
	(void (*)()) &_estack,
	main,                   //-6: Reset (0x00000004),
};
