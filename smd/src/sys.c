/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            sys.c
 * \brief           System core routines to manage the Sega Megadrive/Genesis hardware
 */

#include "sys.h"

/**
 * \brief           SMD Version port
 *
 * The version read only port gives information about the Sega Megadrive/Genesis
 * hardware and version:
 *              |MOD|VMOD|DISK|RSV|VER3|VER2|VER1|VER0|
 *  MOD:     0 = Domestic (Japanese) model.
 *           1 = Overseas (US/European) model.
 *  VMOD:    0 = NTSC mode (7.67MHz CPU clock)
 *           1 = PAL mode (7.60MHz CPU clock)
 *  DISK:    0 = Expansion unit connected (MegaCD).
 *           1 = Expansion unit not connected
 *  RSV:     Reserved. Not used
 *  VER3..0: Megadrive/Genesis version
 */
#define SMD_VERSION_PORT            ((volatile uint8_t *)0xA10001)
#define SMD_VERSION_PORT_MOD_FLAG   (0b10000000)
#define SMD_VERSION_PORT_VMOD_FLAG  (0b01000000)

/**
 * \brief           Current status of interrups (true: enabled, false: disabled)
 */
static bool smd_ints_status_flag;

inline void
smd_ints_enable(void) {
    __asm__ volatile("\tandi.w	#0xF8FF, %sr\n");
    smd_ints_status_flag = true;
}

inline void
smd_ints_disable(void) {
    __asm__ volatile("\tori.w	#0x700, %sr\n");
    smd_ints_status_flag = false;
}

inline bool
smd_ints_status(void) {
    return smd_ints_status_flag;
}

inline bool
smd_is_pal(void) {
    return *SMD_VERSION_PORT & SMD_VERSION_PORT_VMOD_FLAG;
}

inline bool
smd_is_japanese(void) {
    return !(*SMD_VERSION_PORT & SMD_VERSION_PORT_MOD_FLAG);
}
