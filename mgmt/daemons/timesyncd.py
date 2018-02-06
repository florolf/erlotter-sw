#!/usr/bin/env python3

import sys
import can
import datetime

if len(sys.argv) != 2:
    print("usage: %s bus-interface" % sys.argv[0])
    sys.exit(1)

channel = sys.argv[1]

bus = can.interface.Bus(channel=channel, bustype="socketcan_native")
bus.set_filters(
    can_filters = [
        {
            "can_id": 0x8000000,
            "can_mask": 0x1c000fff
        }
    ]
)

def send_time(dst_id):
    now = datetime.datetime.now()
    time_msg = can.Message(arbitration_id = dst_id, data = [0x02, now.hour, now.minute, now.second])

    bus.send(time_msg)

broadcast = 0x7ff8000

send_time(broadcast)

while True:
    msg = bus.recv(timeout=600)

    if msg is not None:
        src = (msg.arbitration_id >> 13) & 0xFFF
        print("node %d booted up" % src)

        send_time(src << 15)
    else:
        # send regular broadcasts
        send_time(broadcast)
