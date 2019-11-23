MARSDEV ?= ${HOME}/mddev/mars
MARSBIN  = $(MARSDEV)/m68k-elf/bin
TOOLSBIN = $(MARSDEV)/bin
JAVA     = java

CC   = $(MARSBIN)/m68k-elf-gcc
AS   = $(MARSBIN)/m68k-elf-as
LD   = $(MARSBIN)/m68k-elf-ld
NM   = $(MARSBIN)/m68k-elf-nm
OBJC = $(MARSBIN)/m68k-elf-objcopy

BINTOS   = $(TOOLSBIN)/bintos 
RESCOMP  = $(JAVA) -jar $(TOOLSBIN)/rescomp.jar
XGMTOOL  = $(TOOLSBIN)/xgmtool
PCMTORAW = $(TOOLSBIN)/pcmtoraw
WAVTORAW = $(TOOLSBIN)/wavtoraw
SIZEBND  = $(TOOLSBIN)/sizebnd

GCC_VER := $(shell $(CC) -dumpversion)
PLUGIN   = $(MARSDEV)/m68k-elf/libexec/gcc/m68k-elf/$(GCC_VER)

INCS    = -I$(MARSDEV)/m68k-elf/include -Isrc -Ires -Iinc
LIBS    = -L$(MARSDEV)/m68k-elf/lib -L$(MARSDEV)/m68k-elf/lib/gcc/m68k-elf/$(GCC_VER) -lmd -lgcc
CCFLAGS = -m68000 -Wall -Wextra -O3 -std=gnu99 -c -fomit-frame-pointer -fno-builtin \
		  -flto -fuse-linker-plugin -fno-web -fno-gcse -fno-unit-at-a-time
ASFLAGS = -m68000 --register-prefix-optional
LDFLAGS = -T $(MARSDEV)/ldscripts/sgdk.ld -nostdlib

BOOT_SS   = $(wildcard src/boot/*.s)
BOOT_OBJS = $(BOOT_SS:.s=.o)

RESS = $(wildcard res/*.res)
CS   = $(wildcard src/*.c)
SS   = $(wildcard src/*.s)

OBJS  = $(RESS:.res=.o)
OBJS += $(CS:.c=.o)
OBJS += $(SS:.s=.o)

.PHONY: all clean
.SECONDARY: bin/rom.elf

all: bin/rom.bin bin/symbol.txt

bin/symbol.txt: bin/rom.bin
	$(NM) --plugin=$(PLUGIN)/liblto_plugin.so -n bin/rom.elf > bin/symbol.txt

src/boot/sega.o: src/boot/rom_head.bin
	$(AS) $(ASFLAGS) src/boot/sega.s -o $@

bin/%.bin: bin/%.elf
	$(OBJC) -O binary $< bin/temp.bin
	dd if=bin/temp.bin of=$@ bs=8K conv=sync

bin/%.elf: $(OBJS) $(BOOT_OBJS)
	$(CC) $(LDFLAGS) $(BOOT_OBJS) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CCFLAGS) $(INCS) -c $< -o $@

%.o: %.s 
	$(AS) $(ASFLAGS) $< -o $@

%.s: %.res
	$(RESCOMP) $< $@

src/boot/rom_head.o: src/boot/rom_head.c
	$(CC) -m68000 -Wall -Wextra -std=c99 -c -fno-builtin $(INCS) $< -o $@

src/boot/rom_head.bin: src/boot/rom_head.o
	$(LD) $(LDFLAGS) --oformat binary -o $@ $<


clean:
	rm -f $(OBJS)
	rm -f bin/temp.bin bin/rom.bin bin/rom.elf bin/symbol.txt
	rm -f src/boot/sega.o src/boot/rom_head.o src/boot/rom_head.bin

