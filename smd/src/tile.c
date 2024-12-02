/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: tile.c
 * Functions to manage tiles in VRAM
 */

#include "tile.h"
#include "dma.h"

inline void
smd_tile_load(const smd_dma_transfer_ft dma_func, const void *restrict src, const uint16_t index, const uint16_t count){
    /*
     * A real tile offset in VRAM is its index * 32 and the amount of words to
     * transfer would be count (in tiles) * 16
     */
    dma_func(&(smd_dma_transfer_t){
        .src = (void *) src,
        .dest = index << 5,
        .size = count << 4,
        .inc = 2,
        .type = SMD_DMA_VRAM_TRANSFER
    });
}
