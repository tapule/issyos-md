/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            pal.h
 * \brief           Palette routines
 *
 * The Sega Megadrive/Genesis VDP can handle up to four palettes of 16 colors
 * each. The first color of each palette is used as transparency, unless used as
 * background color.
 * Tiles and sprites use this palettes to render its pixels.
 * Palettes are stored in the VDP's internal CRAM where 32 bytes are used for
 * each (2 bytes per color). Colors are stored in BGR format were each component
 * is a hexadecimal digit:
 *       0x0BGR  ->  0000 BBB0 GGG0 RRR0
 * Only even numbers can be used (i.e. 02468ACE).
 * There is no need to write an entire palette, you can write individual colors
 * too.
 *
 * More info:
 * https://www.plutiedev.com/tiles-and-palettes
 */

#ifndef SMD_PAL_H
#define SMD_PAL_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Palette identifiers
 */
#define SMD_PAL_0       (0) /* Palette 0 - Colors 0..15 */
#define SMD_PAL_1       (1) /* Palette 1 - Colors 16..31 */
#define SMD_PAL_2       (2) /* Palette 2 - Colors 32..47 */
#define SMD_PAL_3       (3) /* Palette 3 - Colors 48..64 */

/**
 * \brief            Palettes CRAM starting indexes
 */
#define SMD_PAL_0_INDEX (0)  /* Colors 0..15 */
#define SMD_PAL_1_INDEX (16) /* Colors 16..31 */
#define SMD_PAL_2_INDEX (32) /* Colors 32..47 */
#define SMD_PAL_3_INDEX (48) /* Colors 48..64 */

/**
 * \brief           Initialize the palette system
 *
 * We use two internal palette buffers (primary and alternate) to store color
 * values and to do operations (fades, palette swap). This function initialises
 * the needed data to manage these buffers.
 *
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore.
 */
void smd_pal_init(void);

/**
 * \brief           Set colors in the primary internal color buffer
 * \param[in]       index: Position in the buffer were the color copy starts (0..63)
 * \param[in]       count: Number of colors to copy (1..64)
 * \param[in]       colors: Source color data
 * \note            No boundary checks are done in the input parameters, keep
 *                  them safe.
 */
void smd_pal_primary_set(const uint16_t index, uint16_t count, const uint16_t *restrict colors);

/**
 * \brief           Set colors in the alternate internal color buffer
 * \param[in]       index: Position in the buffer were the color copy starts (0..63)
 * \param[in]       count: Number of colors to copy (1..64)
 * \param[in]       colors: Source color dasmd_pal_primary_set(16, 16, font00_pal);
 * \note            No boundary checks are done in the input parameters, keep
 *                  them safe.
 */
void smd_pal_alternate_set(const uint16_t index, uint16_t count, const uint16_t *restrict colors);

/**
 * \brief           Set colors directly in CRAM using DMA, bypassing the internal buffers
 * \param[in]       index: Position in the buffer were the color copy starts (0..63)
 * \param[in]       count: Number of colors to copy (1..64)
 * \param[in]       colors: Source color data
 * \note            No boundary checks are done in the input parameters, keep
 *                  them safe.
 */
void smd_pal_cram_set(const uint16_t index, const uint16_t count, const uint16_t *restrict colors);

/**
 * \brief           Swap the internal color buffers
 */
void smd_pal_swap(void);

/**
 * \brief           Start a fade operation from the primary to alternate color buffers
 * \param[in]       speed: Speed in frames between color fade updates
 */
void smd_pal_fade(const uint16_t speed);

/**
 * \brief           Advances the current color fade operation one step
 * \return          true if fade operation still running, false if it ended
 */
bool smd_pal_fade_step(void);

/**
 * \brief           Stop the current running fade operation
 */
void smd_pal_fade_stop(void);

/**
 * \brief           Waits for a runnig fade operation to finish
 */
void smd_pal_fade_wait(void);

/**
 * \brief           Tell if there is a color fade operation running
 * \return          true if there is a fade operation running, false otherwhise
 */
bool smd_pal_is_fading(void);

/**
 * \brief           Update internal status and upload the primary buffer to CRAM
 * \note            This function updates CRAM, you should call it every frame
 *                  normally after waiting for the vertical blank (see
 *                  smd_vdp_vsync_wait) or whenever you need to upload your
 *                  palettes to CRAM.
 */
void smd_pal_update(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PAL_H */
