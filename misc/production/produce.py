#!/usr/bin/env python3

# adapted from ocd_rpc_example.py in the OpenOCD repository
# original Copyright (C) 2014 Andreas Ortmann (ortmann@finf.uni-hannover.de)
# GPLv3

import socket
import itertools
import sqlite3
import time
import sys
import os
import argparse

parser = argparse.ArgumentParser(description="Manages the flashing of boards")
parser.add_argument('--disallow-new', action="store_true", help="only allow reflashing of known boards")
parser.add_argument('--disallow-old', action="store_true", help="only allow initial flashing of unknown boards")

args = parser.parse_args()

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

def get_id(ocd):
    db = sqlite3.connect('production.db')
    c = db.cursor()

    addr = 0x1FFFF7AC

    uid1 = ocd.readVariable(addr)
    uid2 = ocd.readVariable(addr + 4)
    uid3 = ocd.readVariable(addr + 8)

    if uid1 is None or uid2 is None or uid3 is None:
            print("failed to read subsequent UID components", file=sys.stderr)
            sys.exit(1)

    uid = "%08x-%08x-%08x" % (uid1, uid2, uid3)

    # do we already know the device
    c.execute('SELECT id FROM devices WHERE uid = ?', (uid,))
    row = c.fetchone()

    if row is not None:
        return (False, int(row[0]))

    # new device, create an id
    c.execute('INSERT INTO devices(uid) VALUES(?)', (uid,))

    c.execute('SELECT id FROM devices WHERE uid = ?', (uid,))
    row = c.fetchone()

    db.commit()
    c.close()

    return (True, int(row[0]))

def check_target(ocd):
    var = ocd.readVariable(0x200000000)
    if var is None:
        print("target not present")
        sys.exit(1)

if __name__ == "__main__":
    with OpenOcd() as ocd:
        ocd.send("init")
        ocd.send("reset init")

        check_target(ocd)

        (new, id) = get_id(ocd)
        if new:
            print("Target already known as ID %d" % row[0], file=sys.stderr)
        else:
            print("id is %d" % (id,))

        if args.disallow_new and new == True:
            print("new boards are not allowed with --disallow-new")
            sys.exit(1)

        if args.disallow_old and new == False:
            print("old boards are not allowed with --disallow-old")
            sys.exit(1)

        os.system("perl brand.pl image.bin %d" % (id,))

        ocd.send("program_device ()")
        ocd.send("test ()")

        sys.exit(0)
