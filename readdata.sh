#!/bin/bash

# ready device for reading, clears buffers
stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

# read data, doesn't stop until killed
(cat < /dev/ttyACM0) > powerdata.csv
