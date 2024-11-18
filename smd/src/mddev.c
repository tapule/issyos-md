/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            mddev.c
 * \brief           MDdev common entry point
 */

#include "mddev.h"

#if 0
void
smd_init(void) {
    /* Initialises the z80 secondary CPU */
    smd_z80_init();
    /* Initialises gamepad ports */
    smd_pad_init();
    /* Initialises the PSG (Programmable Sound Generator) */
    smd_psg_init();
    /* Initialises the Yamaha YM2612 FM sound chip */
    smd_ym2612_init();
    /* Initialises the XGM based sound engine */
    smd_xgm_init();
    /* Initialises the graphics  */
    smd_vdp_init();
    /* Initialises the pseudo-random number generator */
    smd_rnd_init();
    /* Initialises the DMA system  */
    smd_dma_init();
    /* Initialises the palette system  */
    smd_pal_init();
    /* Initialises the palette system  */
    smd_spr_init();
}
#endif
