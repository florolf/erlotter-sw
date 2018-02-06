#!/bin/bash

while true
do
	if [ ! -e /sys/class/net/can0 ]
	then
		sleep 10
		continue
	fi

	if ip link show dev can0 | grep -q "<NOARP,ECHO>"
	then
		echo "upping can0"
		ip link set dev can0 up type can bitrate 50000
	fi

	sleep 10
done
