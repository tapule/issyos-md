/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            null_data.h
 * \brief           256 zero bytes of data usable for any purpose
 *
 * This 256 bytes array is also aligned to 256 bytes boundary so it can be used
 * as silent sample for the XGM sound driver.
 */

#ifndef SMD_NULL_DATA_H
#define SMD_NULL_DATA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NULL_DATA_SIZE 256

extern const uint8_t smd_null_data[NULL_DATA_SIZE];

#ifdef __cplusplus
}
#endif

#endif /* SMD_NULL_DATA_H */
