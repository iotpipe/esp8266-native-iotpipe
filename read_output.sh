#!/bin/bash

stty -F /dev/ttyUSB0 raw
#stty -F /dev/ttyUSB0 9600
#stty -F /dev/ttyUSB0 38400
stty -F /dev/ttyUSB0 115200
#stty -F /dev/ttyUSB0 19200
cat /dev/ttyUSB0
