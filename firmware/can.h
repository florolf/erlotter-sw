#pragma once

#include <stdint.h>

struct can_msg {
	uint32_t id;
	uint8_t len;
	uint8_t data[8];
};

#define CAN_DST(id) (((id) >> 15) & 0xFFF)
#define CAN_SRC(id) (((id) >>  3) & 0xFFF)
#define CAN_PORT(id) ((id) & 0x7)
#define CAN_IS_DIRECT(id) ((id) >> 27 == 0)
#define CAN_ID_DIRECT(src, dst, port) ((uint32_t)(dst & 0xFFF) << 15 | (uint32_t)(src & 0xFFF) << 3 | (port & 0x7))
#define CAN_ID_LOCAL_EVENT(src, event) (1ul << 27 | (uint32_t)(src & 0xFFF) << 13 | (event & 0xFFF))

#define CAN_ID_SWAP(id) id = (CAN_ID_DIRECT(CAN_DST(id), CAN_SRC(id), CAN_PORT(id)))

void can_if_init(void);
void can_recv(struct can_msg *msg);
int can_send(struct can_msg *msg);
