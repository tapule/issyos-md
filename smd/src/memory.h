/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            memory.h
 * \brief           Basic memory utilities
 */

#ifndef SMD_MEMORY_H
#define SMD_MEMORY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Fill a memory area with a constant value
 * \param[in]       dest: Destination memory address
 * \param[in]       value: Value used to fill the memory area
 * \param[in]       size: Amount of bytes to fill
 */
void mem_set(void *dest, const uint8_t value, uint16_t size);

/**
 * \brief           Copy a memory area from src to dest
 * \param[in]       dest: Destination memory address
 * \param[in]       src: Source data
 * \param[in]       size: Amount of bytes to copy
 */
void mem_copy(void *dest, const void *src, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* SMD_MEMORY_H */
