# SPDX-License-Identifier: MIT
#
# This file is part of The Curse of Issyos MegaDrive port.
# Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
# Github: https://github.com/tapule

#**
# \file             Makefile
# \brief

# SPDX-License-Identifier: MIT
#
# MDDev development kit
# Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
# Github: https://github.com/tapule/mddev
#
# Makefile
# Project compiler makefile script
#

# Default paths for our toolchain
TOOLCHAIN     ?= ../../toolchain
TOOLCHAINBIN  := $(TOOLCHAIN)/bin
TOOLSBIN      := $(TOOLCHAIN)/bin

# Z80 Assembler to build XGM driver
ASMZ80  := $(TOOLSBIN)/sjasm

# Tools
BINTOC  := $(TOOLSBIN)/bintoc
MKDIR   := mkdir -p
RM      := rm -f

# Default base flags
Z80FLAGS = -isrc

# Extra flags set by debug or release target as needed
EXFLAGS  =

.PHONY: all release xgm

all: release

# Release target
release: xgm

# %.o80: %.s80
#	$(ASMZ80) $(Z80FLAGS) $< $@ obj/z80out.lst
#
# %.s: %.o80
#	$(BINTOS) $<

# This objective compiles the XGM z80 sound driver and converts it to C sources
xgm:
	@echo "-> Building XGM sound driver..."
	@mkdir -p obj/src/xgm
	$(ASMZ80) $(Z80FLAGS) src/z80_xgm.s80 obj/src/xgm/xgm_drv.o80 obj/xgm_drv.lst
	$(BINTOC) -s obj/src/xgm/xgm_drv.o80 -d ../src -t u8
# 	$(BINTOC) -s src/xgm/stop_xgm.bin -d src -t u8

clean:
	@echo "-> Cleaning project..."
	@rm -rf obj

cleanall:
	@echo "-> Cleaning XGM sound driver..."
	@rm -rf obj
	@rm -f ../src/xgm_drv.h ../src/xgm_drv.c
