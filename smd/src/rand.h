/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
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

#ifdef __cplusplus
}
#endif

#endif /* SMD_RAND_H */
