/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            mddev.h
 * \brief           MDdev common entry point
 *
 * This header includes all the needed resources to start building your Sega
 * Megadrive/Genesis homebrew.
 * You must include it in your proyects.
 */

#ifndef SMD_H
#define SMD_H

#define SMD_VERSION 0.1

//#include "boot/vectors.h"
//#include "boot/interrupts.h"
//#include "boot/boot.h"

#include "dma.h"
#include "fix32.h"
#include "kdebug.h"
#include "memory.h"
#include "pad.h"
#include "pal.h"
#include "plane.h"
#include "psg.h"
#include "rand.h"
#include "sprite.h"
#include "sys.h"
#include "text.h"
#include "tile.h"
#include "vdp.h"
#include "xgm.h"
#include "ym2612.h"
#include "z80.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialize the Sega Megadrive/Genesis hardware
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore.
 */
void smd_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_H */
