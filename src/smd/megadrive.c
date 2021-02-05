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

void inline smd_interrupts_enable(void)
{
    __asm__ volatile("\tandi.w	#0xF8FF, %sr\n");
}

void inline smd_interrupts_disable(void)
{
    __asm__ volatile("\tori.w	#0x700, %sr\n");
}