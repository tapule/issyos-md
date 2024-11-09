/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            rand.c
 * \brief           Pseudo-Random Numbers Generation functions
 */

#include "rand.h"
#include "vdp.h"

/**
 * \brief           Stores the lastest (current) calculated seed
 */
static uint16_t smd_rnd_seed;

void
smd_rnd_init(void) {
    uint16_t smd_rnd_var;

    /* CHECKME: Seems that this is not so random */
    /* Mix a random generated value with the MegaDrive HV counter */
    smd_rnd_var = (uint16_t) 0xCE52 ^ (uint16_t) (0xCE52 << 9);
    smd_rnd_seed = *SMD_VDP_PORT_HV_COUNTER ^ (*SMD_VDP_PORT_HV_COUNTER >> 7);
    smd_rnd_seed = smd_rnd_seed ^ smd_rnd_var ^ (smd_rnd_var << 13);
}

void
smd_rnd_seed_set(const uint16_t seed) {
    /* seed must be a non-zero value */
    if (seed == 0) {
        smd_rnd_init();
    }
    smd_rnd_seed = seed;
}

inline uint16_t
smd_rnd_seed_get(void) {
    return smd_rnd_seed;
}

uint16_t
smd_rnd_get(void) {
    /* Xorshift algorithm */
    smd_rnd_seed ^= smd_rnd_seed << 7;
    smd_rnd_seed ^= smd_rnd_seed >> 9;
    smd_rnd_seed ^= smd_rnd_seed << 8;

    return smd_rnd_seed;
}
