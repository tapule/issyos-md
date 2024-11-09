/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            tile.h
 * \brief           Functions to manage tiles in VRAM
 *
 * The Sega Megadrive/Genesis VDP uses patterns (aka tiles) as the basic image
 * unit. A tile is a square 8x8 pixel image that can have up to 16 colors (one
 * of them is transparency). It is used as a base to draw everything: Sprites,
 * Backgrounds, Foregrounds, etc.
 * A tile uses 32 bytes of memory where each pixel is represented by 4 bits (1
 * hexadecimal digit).
 *
 * More info:
 * https://www.plutiedev.com/tiles-and-palettes
 * https://blog.bigevilcorporation.co.uk/2012/03/23/sega-megadrive-4-hello-world
 */

/* CHECKME: ¿Porqué no hay funciones que usen la cola dma o un parámetro defer? */

#ifndef SMD_TILE_H
#define SMD_TILE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Load tiles to VRAM using DMA
 * \param[in]       src: Source tiles address on RAM/ROM space
 * \param[in]       index: Destination tile index on VRAM
 * \param[in]       count: Amount of tiles to load to VRAM
 */
void smd_tile_load(const void *restrict src, const uint16_t index, const uint16_t count);

/**
 * \brief           Load tiles to VRAM using DMA without checks
 * \param[in]       src: Source tiles address on RAM/ROM space
 * \param[in]       index: Destination tile index on VRAM
 * \param[in]       count: Amount of tiles to load to VRAM
 * \note            This function is meant to use RAM as tile's data source. To
 *                  use it from ROM, make sure to check 128kB boundaries.
 */
void smd_tile_load_fast(const void *restrict src, const uint16_t index, const uint16_t count);

#ifdef __cplusplus
}
#endif

#endif /* SMD_TILE_H */
