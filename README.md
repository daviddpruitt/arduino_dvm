# arduino_dvm

Code for an Arduino based dvm and plotting system

#### dvmsketch.ino
Code for an Arduino DVM based on an LM10 amplifier and hall effect current sensor.
Output is in CSV form. Analog I/O pains are paired so that pin 0/1 is a single channel 
with pin 0 being voltage and pin 1 being current. Everything is scaled appropriately 
in the code but it's best to measure and calibrate, these things are never accurate. 
The system takes the average of 1000 samples on each pin to smooth out values and avoid 
spikes.

#### readdata.sh
Simple script that reads data from a USB based arduino device

#### powerchart.html
D3 based chart that plots power data over time, basically reads a csv file. First line
in CSV file becomes header and labels for data being plotted

#### d3.legend.js
d3 library for legends. May not need this but included just in case
