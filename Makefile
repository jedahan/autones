CC = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

PROGRAM    = autones

ISPTOOL	   = arduino
ISPPORT	   = /dev/ttyACM0
ISPSPEED   = 115200
MCU_TARGET = atmega328p

ISPFLASH = avrdude -V -F -c $(ISPTOOL) -p $(MCU_TARGET) -P $(ISPPORT) -b $(ISPSPEED) -U flash:w:$(PROGRAM).hex

OBJ       = $(PROGRAM).o
OPTIMIZE  = -O2

CFLAGS = -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=16000000UL -I flash:w:$(PROGRAM).hex

all: $(PROGRAM).hex

isp: $(TARGET)
	$(ISPFLASH)

%.elf: 
	$(CC) $(CFLAGS) -o $@ $(PROGRAM).cpp

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.objdump: %.elf
	$(OBJDUMP) -d $< > $@

clean:
	rm -rf *.objdump *.elf *.hex

