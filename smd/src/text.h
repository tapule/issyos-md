/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            text.h
 * \brief           Text rendering functions
 *
 * Provides basic functions to render text to a buffer based on a font tileset
 * in VRAM.
 * It is recommended that font tilesets contain the ASCII glyphs between 32-95
 * (64 glyphs) or the ASCII glyphs between 32-127 (96 glyphs):
 *    !"#$%&'()*+,-./0123456789:;<=>?
 *    \ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_
 *    `abcdefghijklmnopqrstuvwxyz{|}~␡
 */

#ifndef SMD_TEXT_H
#define SMD_TEXT_H

#include <stdint.h>
#include "pal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Set the font tileset starting index in VRAM
 * \param[in]       tileset_index: Starting index of font's tileset in VRAM
 */
void smd_text_font_set(const uint16_t tileset_index);

/**
 * \brief           Set the palette to use for text rendering
 * \param[in]       pal_id: CRam palette index (0..3) to use
 */
void smd_text_pal_set(const smd_pal_id_t pal_id);

/**
 * \brief           Enable the high text rendering drawing priority
 */
void smd_text_priority_enable(void);

/**
 * \brief           Disable the high text rendering drawing priority
 */
void smd_text_priority_disable(void);

/**
 * \brief           Writes an ASCII string as font tiles in a buffer using the
 *                  current text configuration.
 * \param           str: Source ASCII string
 * \param           buf: Destination buffer
 * \return          Total written cells (glyphs) in the buffer
 * \note            Destination buffer must have enough space. It writes until
 *                  the null character ('\0') is found in the source string.
 */
uint16_t smd_text_write(const char *restrict str, uint16_t *restrict buf);

/**
 * \brief           Writes up to count ASCII characters from source string as
 *                  font tiles in a buffer using the current text configuration.
 * \param           str: Source ASCII string
 * \param           buf: Destination buffer
 * \param           count: Number of characters to wirte
 * \return          Total written cells (glyphs) in the buffer
 * \note            Destination buffer must have enough space. It writes until
 *                  the null character ('\0') is found in the source string.
 */
uint16_t smd_text_nwrite(const char *restrict str, uint16_t *restrict buf, const uint16_t count);

#ifdef __cplusplus
}
#endif

#endif /* SMD_TEXT_H */
