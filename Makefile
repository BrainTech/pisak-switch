# Two first steps of WinAVR cross-compiler toolchain are used here, and to put
# the file in the flash we use Micronucleus
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = micronucleus

# Configuration for the attiny85, which is the main part of the Wattuino Nanite 85
CFLAGS = -Wall -Os -Iusbdrv -mmcu=attiny85
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = --run

# The list of object files for the firmware
OBJECTS = usbdrv/usbdrv.o usbdrv/oddebug.o usbdrv/usbdrvasm.o main.o

# Building the firmware for attiny85
all: main.hex $(CMDLINE)

# By using command "make flash" the firmware is uploaded to Nanite
flash: main.hex
	$(DUDE) $(DUDEFLAGS) $<

# Removing the files existing after compilation
clean:
	$(RM) *.o *.hex *.elf usbdrv/*.o

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Making main.elf file from object files
main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Recompiling .o files after changing the config
$(OBJECTS): usbdrv/usbconfig.h

# Making object .o files from .c files
%.o: %.c	
	$(CC) $(CFLAGS) -c $< -o $@

# Making object .o files from assembler .S files
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
