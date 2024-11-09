/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            psg.c
 * \brief           Control routines for Sega Megadrive/Genesis PSG
 */

#include <stdint.h>
#include "psg.h"

/**
 * \brief           PSG port from the m68k side
 */
#define SMD_PSG_PORT ((volatile uint8_t *) 0xC00011)

void
smd_psg_init(void) {
    /* Do silence in all 4 channels */
    for (uint16_t i = 0; i < 4; ++i) {
        /* Set volume (attenuation) to 15 which is silence */
        *SMD_PSG_PORT = 0x90 | (i << 5) | 0x0F;

        /*
         * Set frecuency (pitch) to 0
         * Changing the pitch requires writing two bytes to the PSG port
         */
        *SMD_PSG_PORT = 0x80 | (i << 5) | 0x00;
        *SMD_PSG_PORT = 0x00;
    }
}