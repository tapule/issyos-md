/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/* TODO: Esta API debería ser revisada y mejorada. La idea es buena, pero no se ha diseñado del todo bien */

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
 * \param[in]       palette: CRam palette index (0..3) to use
 * FIXME: Esto debería ser un enum definido en pal.h
 */
void smd_text_pal_set(const uint16_t palette);

/**
 * \brief           Set the drawing priority to use for text rendering
 * \param           priority: Drawing priority (0 low priority, 1 high priority)
 */
/*
 * CHECKME: La prioridad solamente puede ser alta o baja, no tiene sentido declarar
 * la función de esta manera. Seguramente sería mejor declarar:
 *  smd_text_priority_enable(void)
 *  smd_text_priority_disable(void)
 * O algo similar.
 */
void smd_text_priority_set(const uint16_t priority);

/**
 * \brief           Writes an ASCII string as font tiles in a buffer using the
 *                  current text configuration.
 * \param           str: Source ASCII string
 * \param           dest: Destination buffer
 * \return          Total written cells (glyphs) in the buffer
 * \note            Destination buffer must have enough space. It writes until
 *                  the null character ('\0') is found in the source string.
 */
/* CHECKME: Render?? No renderiza nada */
uint16_t smd_text_render(const char *restrict str, uint16_t *restrict dest);

/**
 * \brief           Writes up to count ASCII characters from source string as
 *                  font tiles in a buffer using the current text configuration.
 * \param           str: Source ASCII string
 * \param           dest: Destination buffer
 * \param           count: Number of characters to wirte
 * \return          Total written cells (glyphs) in the buffer
 * \note            Destination buffer must have enough space. It writes until
 *                  the null character ('\0') is found in the source string.
 */
/* CHECKME: Render?? No renderiza nada */
uint16_t smd_text_nrender(const char *restrict str, uint16_t *restrict dest, const uint16_t count);

#ifdef __cplusplus
}
#endif

#endif /* SMD_TEXT_H */
