/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            mem_utils.c
 * \brief           Basic memory utilities
 */

#include "mem_utils.h"

void
smd_mem_set(void *dest, const uint8_t value, uint16_t size) {
    uint8_t *p = (uint8_t *) dest;

    while (size) {
        *p = value;
        ++p;
        --size;
    }
}

void
smd_mem_copy(void *dest, const void *src, uint16_t size) {
    uint8_t *d = (uint8_t *) dest;
    const uint8_t *s = (const uint8_t *) src;

    while (size) {
        *d = *s;
        ++d;
        ++s;
        --size;
    }
}
