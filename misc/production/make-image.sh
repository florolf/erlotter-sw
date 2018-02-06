#!/bin/bash

set -eo pipefail

pad() {
	local src="$1"
	local dst="$2"
	local target_size="$3"

	local size=$(stat -c %s "$src")
	local delta=$(( $target_size - $size ))

	if [[ $delta -lt 0 ]]
	then
		echo "file $src too big, expected less than or equal to $target_size, got $size"
		exit 1
	fi

	cp "$src" "$dst"
	head -c $delta /dev/zero >> "$dst"
}

if [ ! -f ../../boot/boot.bin ]
then
	echo "missing boot.bin"
	exit 1
fi

if [ ! -f ../../firmware/build/main/firmware.bin ]
then
	echo "missing firmware.bin"
	exit 1
fi

if [ ! -f config.bin ]
then
	echo "missing config.bin"
	exit 1
fi

pad ../../boot/boot.bin boot.bin $(( 2 * 1024 ))
pad ../../firmware/build/main/firmware.bin firmware.bin $(( 12 * 1024 ))
pad config.bin config-padded.bin $(( 2 * 1024 ))

cat boot.bin firmware.bin config-padded.bin > image.bin
