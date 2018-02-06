#!/usr/bin/env python3

import sys
import can
import sqlite3
import time
import struct

if len(sys.argv) != 3:
    print("usage: %s bus-interface database" % sys.argv[0])
    sys.exit(1)

channel = sys.argv[1]
database = sys.argv[2]

conn = sqlite3.connect(database)
c = conn.cursor()

class SynchronizationError(Exception):
    pass

def send_msg(id, data):
    msg = can.Message(
        arbitration_id = id << 15,
        data=data
    )
    bus.send(msg)

    ret = bus.recv(10)
    if ret is None:
        raise SynchronizationError('timeout')

    if ret.dlc < 0:
        raise SynchronizationError('invalid reply')

    if ret.data[0] != 0:
        raise SynchronizationError('node sent error %d' % ret.data[0])

def synchronize(id):
    send_msg(id, [0x05, 0x00])

    for row in c.execute('SELECT timestamp, color FROM timelines WHERE lamp = ? ORDER BY timestamp ASC', (id,)):
        timestamp = row[0]
        color = row[1]

        send_msg(id, [0x05, 0x01] + list(struct.pack('<I', timestamp)) + list(struct.pack('<h', color)))

    send_msg(id, [0x05, 0x02])

    c.execute('UPDATE lamps SET last_sync = ? WHERE id = ?', (int(time.time()), id))
    c.execute('INSERT INTO logs(time, lamp, message) VALUES(?,?, "timeline synchronized to device")', (int(time.time()), id))
    conn.commit()

while True:
    for row in c.execute('SELECT id FROM lamps where (updated >= last_sync or (updated not null and last_sync isnull)) and bus == ?', (channel,)):
        id = row[0]

        print ("%d is in need of synchronization" % id)

        bus = can.interface.Bus(channel=channel, bustype="socketcan_native")
        bus.set_filters(can_filters=[
            {
                "can_id": id << 3,
                "can_mask": 0x1fffffff
            }
        ])

        try:
            synchronize(id)
            print ("done")
        except SynchronizationError as e:
            print ("synchronization failed: " + e[0])

        bus.shutdown()

    time.sleep(1)
