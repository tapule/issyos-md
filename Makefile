# SPDX-License-Identifier: MIT
#
# This file is part of The Curse of Issyos MegaDrive port.
# Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
# Github: https://github.com/tapule

#**
# \file             Makefile
# \brief            Project compiler makefile script
#

# Default paths for our toolchain
TOOLCHAIN  ?= toolchain
TOOLCHAINBIN  := $(TOOLCHAIN)/bin
TOOLSBIN := $(TOOLCHAIN)/bin

# Some ANSI terminal color codes
COLOR_RESET      = $'\033[0m
COLOR_RED        = $'\033[1;31;49m
COLOR_GREEN      = $'\033[1;32;49m
COLOR_YELLOW     = $'\033[1;33;49m
COLOR_BLUE       = $'\033[1;34;49m
COLOR_MAGENTA    = $'\033[1;35;49m
COLOR_CYAN       = $'\033[1;36;49m
COLOR_WHITE      = $'\033[1;37;49m

# GCC environment
CC   := $(TOOLCHAINBIN)/m68k-elf-gcc
AS   := $(TOOLCHAINBIN)/m68k-elf-as
LD   := $(TOOLCHAINBIN)/m68k-elf-ld
NM   := $(TOOLCHAINBIN)/m68k-elf-nm
GDB  := $(TOOLCHAINBIN)/m68k-elf-gdb
OBJC := $(TOOLCHAINBIN)/m68k-elf-objcopy

# Tools
# TODO: Include tools to manage resources etc.
# BINTOS  = $(TOOLSBIN)/bintos
BINTOC  = $(TOOLSBIN)/bintoc
BLASTEM = $(TOOLCHAIN)/blastem/blastem

# Some files needed are in a versioned directory
GCC_VER := $(shell $(CC) -dumpversion)

# Need the LTO plugin so NM can dump our symbol table
PLUGIN   = $(TOOLCHAIN)/libexec/gcc/m68k-elf/$(GCC_VER)
LTO_SO   = liblto_plugin.so

# Includes: Local + GCC (+ Newlib, uncomment to use it)
INCS     = -Isrc -Ires
INCS    += -I$(TOOLCHAIN)/lib/gcc/m68k-elf/$(GCC_VER)/include
#INCS   += -I$(TOOLCHAIN)/m68k-elf/include

# Libraries: GCC (+ Newlib, uncomment to use it)
LIBS     = -L$(TOOLCHAIN)/lib/gcc/m68k-elf/$(GCC_VER) -lgcc
#LIBS    += -L$(TOOLCHAIN)/m68k-elf/lib -lnosys

# Default base flags
CCFLAGS  = -m68000 -Wall -Wextra -Wpedantic -Wno-unused-local-typedefs -std=c23 -ffreestanding
ASFLAGS  = -m68000 --register-prefix-optional
LDFLAGS  = -T smd/smd.ld -nostdlib

# Extra flags set by debug or release target as needed
EXFLAGS  =

# Sources
CSRC  = $(shell find src/ -type f -name '*.c')
# SSRC  = $(shell find src/ -type f -name '*.s')

# Assets
ASSSRC  = $(shell find build/assets/ -type f -name '*.c')

# Objets files
OBJS    = $(CSRC:.c=.o)
#OBJS   += $(SSRC:.s=.o)
OBJS   += $(ASSSRC:.c=.o)
OUTOBJS = $(addprefix build/obj/, $(OBJS))

# ASM listings
ASM    = $(CSRC:.c=.lst)
OUTASM = $(addprefix build/obj/, $(ASM))

.PHONY: all release asm debug

all: release

# Release target including optimizations
release: EXFLAGS  = -O3 -fno-web -fno-gcse -fno-unit-at-a-time -fshort-enums
release: EXFLAGS += -fomit-frame-pointer -flto -fuse-linker-plugin
release: EXFLAGS += -fno-unwind-tables -DNDEBUG
# release: EXFLAGS += -Wno-shift-negative-value -Wno-main -Wno-unused-parameter -fno-builtin
release: build/bin/rom.bin build/obj/symbol.txt

# Debug target, enables GDB tracing for Blastem, GensKMod, etc.
debug: EXFLAGS = -g -Og -DDEBUG
debug: build/bin/rom.bin build/obj/symbol.txt

# ASM output target. Generates asm listings
asm: EXFLAGS  = -O3 -fno-web -fno-gcse -fno-unit-at-a-time -fshort-enums
asm: EXFLAGS += -fomit-frame-pointer -fuse-linker-plugin
asm: EXFLAGS += -fno-unwind-tables -DNDEBUG
asm: $(OUTASM)

build/bin/rom.elf: $(OUTOBJS)
	@echo "$(COLOR_GREEN)>> Building ELF...$(COLOR_RESET)"
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(LDFLAGS) $(OUTOBJS) $(LIBS)

build/bin/rom.bin: build/bin/rom.elf
	@echo "$(COLOR_GREEN)>> Stripping ELF header...$(COLOR_RESET)"
	@mkdir -p $(dir $@)
	@$(OBJC) -O binary $< build/bin/unpad.bin
	@echo "$(COLOR_GREEN)>> Padding rom file...$(COLOR_RESET)"
	@dd if=build/bin/unpad.bin of=$@ bs=8192 conv=sync
	@rm -f build/bin/unpad.bin

build/obj/%.o: %.c
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) $(EXFLAGS) $(INCS) -c $< -o $@

build/obj/%.o: %.s
	@echo "AS $<"
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

build/obj/%.lst: %.c
	@echo "$(COLOR_GREEN)>> Exporting ASM listings...$(COLOR_RESET)"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) $(EXFLAGS) $(INCS) -S -c $< -o $@
#	@$(CC) $(CCFLAGS) $(EXFLAGS) $(INCS) -S -fverbose-asm -c $< -o $@

# This generates a symbol table that is very helpful in debugging crashes,
# even with an optimized release build!
# Cross reference symbol.txt with the addresses displayed in the crash handler
build/obj/symbol.txt: build/bin/rom.bin
	@echo "$(COLOR_GREEN)>> Exporting symbol table...$(COLOR_RESET)"
	$(NM) --plugin=$(PLUGIN)/$(LTO_SO) -n build/bin/rom.elf > build/obj/symbol.txt

.PHONY: run drun clean

run: release
	@echo "$(COLOR_YELLOW)> Running...$(COLOR_RESET)"
	@$(BLASTEM) build/bin/rom.bin

drun: debug
	@echo "$(COLOR_GREEN)> Running debugger...$(COLOR_RESET)"
#	@$(GDB) -ex "target remote | $(BLASTEM) build/bin/rom.bin -D" build/bin/rom.elf
#	@gdbgui --gdb-cmd="$(GDB) -ex \"target remote | $(BLASTEM) build/bin/rom.bin -D\" build/bin/rom.elf" build/bin/rom.elf
	@mame megadriv -debug -cart build/bin/rom.bin

clean:
	@echo "$(COLOR_MAGENTA)> Cleaning project...$(COLOR_RESET)"
	@rm -rf build/obj
	@rm -f build/bin/rom.elf build/bin/unpad.bin build/bin/rom.bin
