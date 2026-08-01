@echo off
avr-gcc -mmcu=atmega328p -DF_CPU=16000000 -Wall -Os main.c -o temp.elf
avr-objcopy -O ihex temp.elf temp.hex
avrdude -c arduino -p m328p -P COM5 -b 115200 -U flash:w:temp.hex:i
del temp.elf
del temp.hex

