/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: megadrive.c
 * ??
 */

#include "megadrive.h"

static uint16_t ints_status_flag;

void smd_init(void)
{
    /* Initialise the pseudo-random number generator */
    rnd_init();
    /* Initialise the z80 secondary CPU */
    z80_init();
    /* Initialise gamepad ports */
    pad_init();
    /* Initialise the PSG (Programmable Sound Generator) */
    psg_init();
    /* Initialise the graphics  */
    vid_init();
}

inline void smd_ints_enable(void)
{
    __asm__ volatile ("\tandi.w	#0xF8FF, %sr\n");
    ints_status_flag = 1;
}

inline void smd_ints_disable(void)
{
    __asm__ volatile ("\tori.w	#0x700, %sr\n");
    ints_status_flag = 0;    
}

inline uint16_t smd_ints_status(void)
{
    return ints_status_flag;
}