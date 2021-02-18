/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: rand.c
 * Control routines for Pseudo-Random Numbers Generation
 */

#include "rand.h"
#include "vdp.h"

/* Current seed */
static uint16_t current_seed;

void rnd_init(void)
{
    uint16_t rnd_var;

    /* Mix a random generated value with the MegaDrive HV counter */
    rnd_var = (uint16_t) 0xCE52 ^ (uint16_t) (0xCE52 << 9);
    current_seed = *VDP_PORT_HV_COUNTER ^ (*VDP_PORT_HV_COUNTER >> 7);
    current_seed = current_seed ^ rnd_var ^ (rnd_var << 13);
}

void rnd_seed_set(uint16_t seed)
{
    /* seed must be a non-zero value */
    if (seed == 0)
    {
        rnd_init();
    }
    current_seed = seed;
}

inline uint16_t rnd_seed_get(void)
{
    return current_seed;
}

uint16_t rnd_get(void)
{
    /* Xorshift algorithm */
    current_seed ^= current_seed << 7;
    current_seed ^= current_seed >> 9;
    current_seed ^= current_seed << 8;

    return current_seed;
}
