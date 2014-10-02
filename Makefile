PROGRAM    = autones

ISPTOOL	   = arduino
ISPPORT	   = /dev/ttyACM0
ISPSPEED   = 115200
MCU_TARGET = atmega328p

CC = avr-g++
AVRDUDE = avrdude
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

CFLAGS = -Wall -O2 -DF_CPU=16000000UL

all: isp clean

isp: $(PROGRAM).hex
	sudo avrdude -V -F -c $(ISPTOOL) -p $(MCU_TARGET) -P $(ISPPORT) -b $(ISPSPEED) -U flash:w:$<

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.elf: %.cpp
	$(CC) $(CFLAGS) -mmcu=$(MCU_TARGET) -I flash:w:$(PROGRAM).hex -o $@ $<

%.objdump: %.elf
	$(OBJDUMP) -d $< > $@

clean:
	rm -rf *.objdump *.elf *.hex

.PHONY: all clean isp
