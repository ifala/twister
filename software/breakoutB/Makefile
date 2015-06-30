PROJECT=test
SOURCES=$(PROJECT).c
MMCU=attiny44
F_CPU = 8000000

CFLAGS=-mmcu=$(MMCU) -Wall -Os -DF_CPU=$(F_CPU)

$(PROJECT).hex: $(PROJECT).out
	avr-objcopy -j .text -O ihex $(PROJECT).out $(PROJECT).c.hex;\
   avr-size --mcu=$(MMCU) --format=avr $(PROJECT).out
 
$(PROJECT).out: $(SOURCES)
	avr-gcc $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p t44 -c usbtiny -U flash:w:$(PROJECT).c.hex

program-fuses: $(PROJECT).hex
	avrdude -p t44 -c usbtiny -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
