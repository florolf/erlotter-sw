TOPDIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

SRC = systick.c hrtim.c adc.c usart.c config.c can.c comp.c pwm_in.c rtc.c pid.c lamp.c
OBJS=$(patsubst %.c,build/$(MODE)/%.o,$(SRC))

CROSS_PREFIX ?= arm-none-eabi-
CC=$(CROSS_PREFIX)gcc
OBJCOPY=$(CROSS_PREFIX)objcopy
SIZE=$(CROSS_PREFIX)size

GIT_REV=$(shell git rev-parse --short=8 HEAD | tr -d '\n' | tac -rs ..)

CFLAGS = -Wall -Wextra -std=gnu99 -I../libopencm3/include -I. -DSTM32F3 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -fno-common -ffreestanding -Og -g -DMODE_$(MODE) -DGIT_REV=0x$(GIT_REV)
LDFLAGS = -lopencm3_stm32f3 -L../libopencm3/lib -mthumb -mcpu=cortex-m4 -nostartfiles -Wl,--gc-sections -L /usr/arm-none-eabi/lib/armv7e-m/fpu -g

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

all: main

main test:
	mkdir -p build/$@
	$(MAKE) build/$@/firmware.bin MODE=$@

build/$(MODE)/firmware.elf: build/$(MODE)/$(MODE).o $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -T linker-$(MODE).ld
	$(SIZE) -B $@

build/$(MODE)/%.bin: build/$(MODE)/%.elf
	$(OBJCOPY) -O binary $^ $@

build/$(MODE)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf build

.PHONY: all clean main test
