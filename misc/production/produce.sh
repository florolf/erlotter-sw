#!/bin/bash

while true
do
	python3 produce.py "$@"

	echo "press enter"
	read
	clear
done
