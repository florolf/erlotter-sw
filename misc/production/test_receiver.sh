#!/bin/bash

source venv/bin/activate
python test_receiver.py can0 | ts | tee -a log
