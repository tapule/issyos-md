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

/* Sega MegaDrive/Genesis VDP HV counter port */
static volatile uint16_t *const vdp_port_hv_counter = (uint16_t*) 0xC00008;

/* Current seed */
static uint16_t rnd_seed;

void rnd_init(void)
{
    uint16_t rnd_var;

    /* Mix a random generated value with the MegaDrive HV counter */
    rnd_var = (uint16_t) 0xCE52 ^ (uint16_t) (0xCE52 << 9);
    rnd_seed = *vdp_port_hv_counter ^ (*vdp_port_hv_counter >> 7);
    rnd_seed = rnd_seed ^ rnd_var ^ (rnd_var << 13);
}

void rnd_seed_set(uint16_t seed)
{
    /* seed must be a non-zero value */
    if (seed == 0)
    {
        rnd_init();
    }
    rnd_seed = seed;
}

inline uint16_t rnd_seed_get(void)
{
    return rnd_seed;
}

uint16_t rnd_get(void)
{
    /* Xorshift algorithm */
    rnd_seed ^= rnd_seed << 7;
    rnd_seed ^= rnd_seed >> 9;
    rnd_seed ^= rnd_seed << 8;

    return rnd_seed;
}
