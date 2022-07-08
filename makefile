CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LFLAGS= -nostdlib -T stm32f446xx.ld -Wl,-Map=$(OUTPUT_FOLDER)/final.map

#VPATH = ./app/

OUTPUT_FOLDER = build

all:main.o startup_stm32f446xx.o rcc.o timer.o gpio.o spi.o lcd162a.o final.elf

main.o:app/main/main.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

startup_stm32f446xx.o:app/startup/startup_stm32f446xx.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

rcc.o:app/rcc/rcc.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

timer.o:app/timer/timer.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

gpio.o:app/gpio/gpio.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

spi.o:app/spi/spi.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

lcd162a.o:app/LCD162A/LCD162A.c
	$(CC) $(CFLAGS) -o $(OUTPUT_FOLDER)/$@ $^

final.elf:main.o startup_stm32f446xx.o rcc.o timer.o gpio.o lcd162a.o
	$(CC) $(LFLAGS) $(OUTPUT_FOLDER)/*.o  -o $(OUTPUT_FOLDER)/$@

clean-windows:
	@del /s *.o *.elf *.map

load:
	openocd -f board/stm32f4discovery.cfg