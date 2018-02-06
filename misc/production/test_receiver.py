#!/usr/bin/env python3

import sys
import can

if len(sys.argv) != 2:
    print("usage: %s bus-interface" % sys.argv[0])
    sys.exit(1)

channel = sys.argv[1]

bus = can.interface.Bus(channel=channel, bustype="socketcan_native")

def logmsg(msg):
    print(msg)
    sys.stdout.flush()

while True:
    msg = bus.recv()

    if msg.arbitration_id & 0x10000000:
        logmsg("got bootloader frame from id %d" % (msg.arbitration_id & 0xfffffff))
        continue

    if msg.arbitration_id & 0x8000000 == 0:
        continue

    event = msg.arbitration_id & 0xFFF
    src = (msg.arbitration_id >> 13) & 0xFFF

    if event == 0:
        logmsg ("[% 3d] got bootup: \"%s\"" % (src,bytes(msg.data[:msg.dlc]).decode("ascii")))
    elif event == 1:
        if msg.dlc != 1:
            continue

        if msg.data[0] & 0x01:
            logmsg("[% 3d] overvoltage fault" % (src,))
        if msg.data[0] & 0x02:
            logmsg("[% 3d] overcurrent (warm channel) fault" % (src,))
        if msg.data[0] & 0x04:
            logmsg("[% 3d] overcurrent (cold channel) fault" % (src,))
    elif event == 42:
        if msg.data[0] == 1:
            logmsg("[% 3d] mismatched PWM: %d" % (src, msg.data[1]))
        elif msg.data[0] == 0xFF:
            logmsg("[% 3d] testmode end" % (src,))
        else:
            logmsg("[% 3d] unknown test event code %d" % (src,msg.data[0]))
    elif event == 3:
            logmsg("[% 3d] heartbeat" % (src,))
    else:
        logmsg("[% 3d] unknown event code %d" % (src,event))
