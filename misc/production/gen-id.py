#!/usr/bin/env python3

# adapted from ocd_rpc_example.py in the OpenOCD repository
# original Copyright (C) 2014 Andreas Ortmann (ortmann@finf.uni-hannover.de)
# GPLv3

import socket
import itertools
import sqlite3
import time
import sys

def strToHex(data):
    return map(strToHex, data) if isinstance(data, list) else int(data, 16)

class OpenOcd:
    COMMAND_TOKEN = '\x1a'
    def __init__(self, verbose=False):
        self.verbose = verbose
        self.tclRpcIp       = "127.0.0.1"
        self.tclRpcPort     = 6666
        self.bufferSize     = 4096

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def __enter__(self):
        self.sock.connect((self.tclRpcIp, self.tclRpcPort))
        return self

    def __exit__(self, type, value, traceback):
        try:
            self.send("exit")
        finally:
            self.sock.close()

    def send(self, cmd):
        """Send a command string to TCL RPC. Return the result that was read."""
        data = (cmd + OpenOcd.COMMAND_TOKEN).encode("utf-8")
        if self.verbose:
            print("<- ", data)

        self.sock.send(data)
        return self._recv()

    def _recv(self):
        """Read from the stream until the token (\x1a) was received."""
        data = bytes()
        while True:
            chunk = self.sock.recv(self.bufferSize)
            data += chunk
            if bytes(OpenOcd.COMMAND_TOKEN, encoding="utf-8") in chunk:
                break

        if self.verbose:
            print("-> ", data)

        data = data.decode("utf-8").strip()
        data = data[:-1] # strip trailing \x1a

        return data

    def readVariable(self, address):
        raw = self.send("ocd_mdw 0x%x" % address).split(": ")
        return None if (len(raw) < 2) else strToHex(raw[1])

    def writeVariable(self, address, value):
        assert value is not None
        self.send("mww 0x%x 0x%x" % (address, value))

if __name__ == "__main__":
    db = sqlite3.connect('production.db')
    c = db.cursor()

    with OpenOcd() as ocd:
        ocd.send("halt")

        addr = 0x1FFFF7AC

        first = True
        while True:
            uid1 = ocd.readVariable(addr)

            if uid1 is not None:
                break

            if first:
                print("Target not present, waiting", file=sys.stderr)
                first = False

            time.sleep(1)

        uid2 = ocd.readVariable(addr + 4)
        uid3 = ocd.readVariable(addr + 8)

        ocd.send("resume")

        if uid2 is None or uid3 is None:
                print("failed to read subsequent UID components", file=sys.stderr)
                sys.exit(1)

        uid = "%08x-%08x-%08x" % (uid1, uid2, uid3)

        # do we already know the device
        c.execute('SELECT id FROM devices WHERE uid = ?', (uid,))
        row = c.fetchone()

        if row is not None:
            print("Target already known as ID %d" % row[0], file=sys.stderr)
            print("%d" % row[0])
            sys.exit(0)

        # new device, create an id
        c.execute('INSERT INTO devices(uid) VALUES(?)', (uid,))

        c.execute('SELECT id FROM devices WHERE uid = ?', (uid,))
        row = c.fetchone()

        print("%d" % row[0])

        db.commit()
        c.close()

        sys.exit(0)
