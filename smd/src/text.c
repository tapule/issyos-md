/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            text.c
 * \brief           Text rendering functions
 */

#include "text.h"

/**
 * \brief           Base cell used to store text rendering palette and priority
 *
 * Cells in VDP plane format, are words (16 bits) describing what tile to draw
 * and how to draw it according to this format:
 *      PCCVHTTTTTTTTTTT
 *      P: Priority flag
 *      C: Palete select
 *      V: Vertical flip flag
 *      H: Horizontal flip flag
 *      T: Tile index in VRam to drawn
 */
static uint16_t smd_text_base_cell      = 0;

/**
 * \brief           Stores the index of the first tile in the font tileset
 */
static uint16_t smd_text_tileset_index  = 0;

inline void
smd_text_font_set(const uint16_t tileset_index) {
    smd_text_tileset_index = tileset_index;
}

inline void
smd_text_pal_set(const uint16_t palette) {
    /* Sets to 0 the palette bits */
    /* CHECKME: Si palette fuera un enum no sería necesario hacer 0 en estos bits */
    smd_text_base_cell &= ~(0x03 << 13);
    /* Move new palette bits to its positions */
    smd_text_base_cell |= (0x03 & palette) << 13;
}

inline void
smd_text_priority_set(const uint16_t priority) {
    /* Sets to 0 the priority bit */
    smd_text_base_cell &= ~(0x01 << 15);
    /* Move new priority bit to its position */
    smd_text_base_cell |= (0x01 & priority) << 15;
}

uint16_t
smd_text_render(const char *restrict str, uint16_t *restrict dest) {
    uint16_t count;

    count = 0;
    while (*str != '\0') {
        *dest = smd_text_base_cell + (smd_text_tileset_index + *str - 32);
        ++dest;
        ++str;
        ++count;
    }

    return count;
}

uint16_t
smd_text_nrender(const char *restrict str, uint16_t *restrict dest, const uint16_t count) {
    uint16_t i;

    i = count;
    while ((*str != '\0') && (i > 0)) {
        *dest = smd_text_base_cell + (smd_text_tileset_index + (*str - 32));
        ++dest;
        ++str;
        --i;
    }

    return count - i;
}
