/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            rand.h
 * \brief           Pseudo-Random Numbers Generation functions
 *
 * Implementation of pseudo-random number generation using Xorshift algorithm
 * by George Marsaglia
 *
 * More info:
 * http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
 */

#ifndef SMD_RAND_H
#define SMD_RAND_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialise the pseudo-random number generator with a random seed
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore.
 */
void smd_rnd_init(void);

/**
 * \brief           Sets a custom seed for prng
 *
 * Setting a custom seed lets you repeat the sequence of numbers generated
 *
 * \param[in]       seed: Custom seed
 */
void smd_rnd_seed_set(const uint16_t seed);

/**
 * \brief           Get the current seed
 * \return          The current seed used to generate pseudo-random numbers
 */
uint16_t smd_rnd_seed_get(void);

/**
 * \brief           Generate a new random number
 * \return          The new random number
 */
uint16_t smd_rnd_get(void);

/*
    TODO: With rng there are some utility functions that we can implement
    uint16_t rnd_get_max(uint16_t max): Get a random number in the range [0..max) (rnd_get() % max)
    uint16_t rnd_get_range(uint16_t min, max): Get a random number in the range [min..max] (min + rnd_get(max - min))
    bool rnd_get_bool(): Get a random boolean ((rnd_get() & 1) == 1)
    int16_t rnd_get_sign(): Get a random signt (rnd_get_bool() ? 1 : -1)
    uint16_t rnd_choose2(uint16_t a, b): Choose between a and b randomly (rnd_get(2) ? b : a)
    uint16_t rnd_choose3(uint16_t a, b, c): (switch rnd_get(3) { 0: a, 1: b, 3: default: c})
        rnd_choose4(uint16_t a, b, c, d), rnd_choose5(uint16_t a, b, c, d, e), etc.
    bool rnd_chance(uint8_t percent): Get a random number with a percent probability (???)
    ------------------
    fix32_t rnd_get_fix32(): Generate a random fixed num??
    fix16_t rnd_get_fix16(): Generate a random fixed num??
*/

#ifdef __cplusplus
}
#endif

#endif /* SMD_RAND_H */
