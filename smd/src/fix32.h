/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            fix32.h
 * \brief           Fixed point type in 16.16 format and basic operations
 */

#ifndef SMD_FIX32_H
#define SMD_FIX32_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Debería poner también el fix16 como 8:8

typedef int32_t fix32_t;

/**
 * \brief           How many fractional bits has our fix32 type
 */
#define FIX32_FRACT_BITS      (16)
#define FIX32_ONE             (1 << FIX32_FRACT_BITS)
/**
 * \brief           Masks for fractional and integer parts
 */
#define FIX32_FRACT_MASK      (FIX32_ONE - 1)
#define FIX32_INT_MASK        (FIX32_FRACT_MASK << FIX32_FRACT_BITS)

/**
 * \brief           Conversion routines
 */
#define fix32_from_int(val)   (((fix32_t)(val)) << FIX32_FRACT_BITS)
#define fix32_from_float(val) ((fix32_t)((val) * FIX32_ONE))
#define fix32_to_int(fix)     ((int32_t)((fix) >> FIX32_FRACT_BITS))

/**
 * \brief           Get fractional and integer parts from a fix32
 */
#define fix32_get_fract(fix)  ((fix) & FIX32_FRACT_MASK)
/* CHECKME: ¿No se podría hacer esto simplemente casteando a int16_t? */
#define fix32_get_int(fix)    (((fix) & FIX32_INT_MASK) >> FIX32_FRACT_BITS)

/**
 * \brief           Non direct operations
 * \note            Be aware that there is no overflow checks
 */
#define fix32_mul(x, y)       (((x) * (y)) >> FIX32_FRACT_BITS)
#define fix32_div(x, y)       (((x) << FIX32_FRACT_BITS) / (y))

#ifdef __cplusplus
}
#endif

#endif /* SMD_FIX32_H */
