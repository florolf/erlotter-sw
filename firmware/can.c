#include <string.h>

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/can.h>

#include "can.h"
#include "config.h"
#include "systick.h"
#include "rtc.h"
#include "lamp.h"

/*
  Identifier format:

   2    2   2   1   1
   8    4   0   6   2   8   4    0
   00 dddddddddddd sssssssssss ppp directed frame
   01 iiiiiiiiiiii iiiiiiiiiii iii event frame
   01 0sssssssssss siiiiiiiiii iii  local event frame
   01 1iiiiiiiiiii iiiiiiiiiii iii  global event frame
   1. ............. ............ ... bootloader frame

   d: destination (12 bit)
   s: source (12 bit)
   p: port (3 bit)

   i: event id (27 bit)

   Only extended identfiers are used.
   The all-ones destination is a broadcast.
*/

#define CAN_TX_TIMEOUT 1

#define EINVAL -1
#define ENOSYS -2
#define ENOMEM -3

// avoid collision with libopencm3
void can_if_init(void)
{
	rcc_periph_clock_enable(RCC_CAN);
	rcc_periph_reset_pulse(RST_CAN);

	// set up LINK LED
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);
	gpio_set(GPIOB, GPIO5);

	// set up CAN_RX/TX
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8 | GPIO9);
	gpio_set_af(GPIOB, GPIO_AF9, GPIO8 | GPIO9);

	// configure CAN
	CAN_MCR(BX_CAN1_BASE) &= ~CAN_MCR_SLEEP;
	CAN_MCR(BX_CAN1_BASE) |= CAN_MCR_INRQ | CAN_MCR_ABOM;

	while (!(CAN_MSR(BX_CAN1_BASE) & CAN_MSR_INAK))
		;

	// BPR = 45 -> Tq = 125 ns at 36MHz -> 50 kbaud
	CAN_BTR(BX_CAN1_BASE) = CAN_BTR_SJW_1TQ | CAN_BTR_TS1_13TQ | CAN_BTR_TS2_2TQ | 44;

	// enable reception interrupt
	CAN_IER(BX_CAN1_BASE) |= CAN_IER_FMPIE0;
	nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ);
	nvic_set_priority(NVIC_HRTIM_FLT_IRQ, 10 << 4);

	// set receive filters
	CAN_FMR(BX_CAN1_BASE) |= CAN_FMR_FINIT;
	CAN_FS1R(BX_CAN1_BASE) |= 0x3; // 32bit filters

	// filter 0: broadcast
	CAN_FiR1(BX_CAN1_BASE, 0) = 0x07ff8000 << CAN_RIxR_EXID_SHIFT | CAN_RIxR_IDE;
	CAN_FiR2(BX_CAN1_BASE, 0) = 0x1fff8000;
	CAN_FA1R(BX_CAN1_BASE) |= 1 << 0; // enable it

	// filter 1: our ID
	CAN_FiR1(BX_CAN1_BASE, 1) = ((BOARD_ID & 0xFFF) << 15) << CAN_RIxR_EXID_SHIFT | CAN_RIxR_IDE;
	CAN_FiR2(BX_CAN1_BASE, 1) = 0x1fff8000;
	CAN_FA1R(BX_CAN1_BASE) |= 1 << 1;

	// set transmit address
	CAN_TI0R(BX_CAN1_BASE) = BOARD_ID << CAN_TIxR_EXID_SHIFT | CAN_TIxR_IDE;

	CAN_FMR(BX_CAN1_BASE) &= ~CAN_FMR_FINIT;

	CAN_MCR(BX_CAN1_BASE) &= ~CAN_MCR_INRQ;
	while ((CAN_MSR(BX_CAN1_BASE) & CAN_MSR_INAK))
		;
}

void can_recv(struct can_msg *msg)
{
	uint32_t h, l;
	while (!(CAN_RF0R(BX_CAN1_BASE) & CAN_RF0R_FMP0_MASK))
		;

	msg->len = CAN_RDT0R(BX_CAN1_BASE) & CAN_RDTxR_DLC_MASK;
	msg->id = CAN_RI0R(BX_CAN1_BASE) >> 3;

	h = CAN_RDH0R(BX_CAN1_BASE);
	l = CAN_RDL0R(BX_CAN1_BASE);
	memcpy(&msg->data[0], &l, 4);
	memcpy(&msg->data[4], &h, 4);

	CAN_RF0R(BX_CAN1_BASE) |= CAN_RF0R_RFOM0;
	while (CAN_RF0R(BX_CAN1_BASE) & CAN_RF0R_RFOM0)
		;
}

int can_send(struct can_msg *msg)
{
	uint32_t timeout = ticks + CAN_TX_TIMEOUT * HZ;
	uint32_t h, l;

	memcpy(&l, &msg->data[0], 4);
	memcpy(&h, &msg->data[4], 4);

	CAN_TDT0R(BX_CAN1_BASE) &= ~CAN_TDTxR_DLC_MASK;
	CAN_TDT0R(BX_CAN1_BASE) |= msg->len;
	CAN_TDH0R(BX_CAN1_BASE) = h;
	CAN_TDL0R(BX_CAN1_BASE) = l;
	CAN_TI0R(BX_CAN1_BASE) = (msg->id << 3) | CAN_TIxR_IDE;

	CAN_TI0R(BX_CAN1_BASE) |= CAN_TIxR_TXRQ;

	while (CAN_TI0R(BX_CAN1_BASE) & CAN_TIxR_TXRQ) {
		if (time_after(ticks, timeout)) {
			CAN_TSR(BX_CAN1_BASE) |= CAN_TSR_ABRQ0;
			while (CAN_TSR(BX_CAN1_BASE) & CAN_TSR_ABRQ0)
				;

			return -1;
		}
	}

	return 0;
}

static int can_cmd_reset(struct can_msg *msg)
{
	if (msg->len != 8 || memcmp(&msg->data[1], "resetme", 7))
		return EINVAL;

	scb_reset_system();

	return 0; // not reached
}

static int can_cmd_ping(struct can_msg *msg)
{
	(void) msg;

	return 0;
}

static int can_cmd_set_time(struct can_msg *msg)
{
	if (msg->len != 4)
		return EINVAL;

	rtc_set(msg->data[1], msg->data[2], msg->data[3]);
	lamp_trigger_autocolor();

	return 0;
}

static int can_cmd_set_brightness(struct can_msg *msg)
{
	if (msg->len != 2)
		return EINVAL;

	lamp_set_brightness(msg->data[1]);

	return 0;
}

static int can_cmd_set_color(struct can_msg *msg)
{
	if (msg->len != 3)
		return EINVAL;

	uint16_t temp = (msg->data[1] << 8) | msg->data[2];

	if (temp) {
		lamp_stop_autocolor();

		lamp_set_color(temp);
	} else
		lamp_start_autocolor();

	return 0;
}

static int can_cmd_set_timeline(struct can_msg *msg)
{
	static struct color_segment timeline_buf[COLOR_TIMELINE_POINTS];
	static int i = 0;

	// timeline clear
	if (msg->len == 2 && msg->data[1] == 0x00) {
		memset(timeline_buf, 0, sizeof(timeline_buf));
		i = 0;

		return 0;
	}

	// timeline append
	else if (msg->len == 8 && msg->data[1] == 0x01) {
		if (i >= COLOR_TIMELINE_POINTS)
			return EINVAL;

		timeline_buf[i].time = *(uint32_t*)&msg->data[2];
		timeline_buf[i].color = *(uint16_t*)&msg->data[6];
		i++;

		return 0;
	}

	// timeline commit
	else if (msg->len == 2 && msg->data[1] == 0x02) {
		struct config_entry *cfg;

		// need to terminate if the timeline is not full
		if (i < COLOR_TIMELINE_POINTS) {
			timeline_buf[i].time = 0xFFFFFFFF;
		}

		cfg = config_get(CONFIG_KEY_TIMELINE);
		if (!cfg)
			cfg = config_new(CONFIG_KEY_TIMELINE, COLOR_TIMELINE_POINTS * sizeof(struct color_segment));

		if (!cfg || cfg->len < COLOR_TIMELINE_POINTS * sizeof(struct color_segment))
			return ENOMEM;

		lamp_stop_autocolor();
		memcpy(cfg->data, timeline_buf, COLOR_TIMELINE_POINTS * sizeof(struct color_segment));

		config_commit();
		lamp_start_autocolor();

		return 0;
	}

	return ENOSYS;
}

static int can_cmd_clear_config(struct can_msg *msg)
{
	if (msg->len != 8 || memcmp(&msg->data[1], "clearme", 7))
		return EINVAL;

	config_clear();

	return 0;
}

static void exec(uint32_t sp, uint32_t ip)
{
	if (sp)
		__asm__ volatile ("msr msp, %0" : : "r" (sp));

	((void(*)())ip)();
}

static int can_cmd_enter_bootloader(struct can_msg *msg)
{
	if (msg->len != 8 || memcmp(&msg->data[1], "bootldr", 7))
		return EINVAL;

	cm_disable_interrupts();

	*(uint32_t*)0x20000000 = 0xdeadb007;
	exec(
		*(uint32_t*)0x8000000, // initial SP
		*(uint32_t*)0x8000004 // reset vector
	);

	return 0; // never reached
}

void usb_lp_can1_rx0_isr(void)
{
	struct can_msg msg;
	int ret;

	can_recv(&msg);

	if (!CAN_IS_DIRECT(msg.id) || CAN_SRC(msg.id) != 0)
		return;

	if (msg.len == 0)
		return;

	gpio_toggle(GPIOB, GPIO5);

	switch (msg.data[0]) {
		case 0x00:
			ret = can_cmd_reset(&msg);
			break;
		case 0x01:
			ret = can_cmd_ping(&msg);
			break;
		case 0x02:
			ret = can_cmd_set_time(&msg);
			break;
		case 0x03:
			ret = can_cmd_set_brightness(&msg);
			break;
		case 0x04:
			ret = can_cmd_set_color(&msg);
			break;
		case 0x05:
			ret = can_cmd_set_timeline(&msg);
			break;
		case 0x06:
			ret = can_cmd_clear_config(&msg);
			break;
		case 0x07:
			ret = can_cmd_enter_bootloader(&msg);
			break;
		default:
			ret = ENOSYS;
			break;
	}

	if (CAN_DST(msg.id) == 0xFFF)
		return;

	CAN_ID_SWAP(msg.id);

	if (ret <= 0) {
		msg.len = 1;
		msg.data[0] = -ret;
	}

	can_send(&msg);
}
