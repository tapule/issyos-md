/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            unpack.h
 * \brief           Decompressors for 68000
 *
 * Blah
 *
 * More info:
 */

#ifndef SMD_UNPACK_H
#define SMD_UNPACK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void smd_unpack_slz(const uint8_t *in, uint8_t *out);

void smd_unpack_zx0(const uint8_t *in, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SMD_UNPACK_H */
