#!/usr/bin/env python3

import sys
import can
import sqlite3

if len(sys.argv) != 3:
    print("usage: %s bus-interface database" % sys.argv[0])
    sys.exit(1)

channel = sys.argv[1]
database = sys.argv[2]

bus = can.interface.Bus(channel=channel, bustype="socketcan_native")
bus.set_filters(
    can_filters = [
        {
            "can_id": 0x8000000,
            "can_mask": 0x1c000fff
        },
        {
            "can_id": 0x8000001,
            "can_mask": 0x1c000fff
        }
    ]
)

conn = sqlite3.connect(database)
c = conn.cursor()

def log_msg(can_msg, log_msg):
    src = (can_msg.arbitration_id >> 13) & 0xFFF
    ts = int(round(can_msg.timestamp))

    c.execute('INSERT INTO logs(time, lamp, message) VALUES (?,?,?)',
              (ts, src, log_msg))

    conn.commit()

while True:
    msg = bus.recv()

    event = msg.arbitration_id & 0xFFF

    if event == 0:
        log_msg(msg, "node rebooted")
    elif event == 1:
        if msg.dlc != 1:
            continue

        if msg.data[0] & 0x01:
            log_msg(msg, "overvoltage fault")
        if msg.data[0] & 0x02:
            log_msg(msg, "overcurrent (warm channel) fault")
        if msg.data[0] & 0x04:
            log_msg(msg, "overcurrent (cold channel) fault")
