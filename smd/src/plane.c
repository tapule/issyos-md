/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            plane.c
 * \brief           VDP's planes drawing functions
 */

#include "plane.h"
#include "dma.h"
#include "memory_map.h"
#include "vdp.h"

// inline uint16_t
// smd_plane_cell_config(const uint16_t tile_index, const uint8_t palette, const uint8_t h_flip, const uint8_t v_flip,
//                   const uint8_t priority) {
//     return (priority << 15) | (palette << 13) | (v_flip << 12) | (h_flip << 11) | tile_index;
// }
inline smd_plane_cell
smd_plane_cell_make(const smd_plane_cell_desc_t *restrict cell_desc) {
    return (cell_desc->priority << 15) | (cell_desc->palette << 13) | (cell_desc->v_flip << 12) |
           (cell_desc->h_flip << 11) | cell_desc->tile_index;
}

inline void
smd_plane_clear(const smd_plane_t plane) {
    smd_dma_vram_fill(plane, SMD_VDP_PLANE_TILES << 1, 0x00, 1);
}

void
smd_plane_rect_fill(const smd_plane_t plane, const uint16_t tile, const uint16_t x, const uint16_t y,
                    const uint16_t width, const uint16_t height) {
    uint16_t tile_row[SMD_VDP_PLANE_WIDTH];

    /* Setup tile buffer */
    for (uint16_t i = 0; i < width; ++i) {
        tile_row[i] = tile;
    }
    /* Draws rows in the plane */
    for (uint16_t i = 0; i < height; ++i) {
        /* x, y, i and SMD_VDP_PLANE_WIDTH are in tiles, convert to words */
        smd_dma_transfer_fast(&(smd_dma_transfer_t){.src = &tile_row,
                                                    .dest = plane + ((x + ((y + i) * SMD_VDP_PLANE_WIDTH)) << 1),
                                                    .size = width,
                                                    .inc = 2,
                                                    .type = SMD_DMA_VRAM_TRANSFER});
    }
}

void
smd_plane_tile_draw(const smd_plane_t plane, const uint16_t tile, const uint16_t x, const uint16_t y) {
    uint16_t vram_addr;

    /* It doesn't make sense to use DMA for only one tile. Write it directly  */
    vram_addr = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1);
    *SMD_VDP_CTRL_PORT_U32 = (((uint32_t)(SMD_VDP_VRAM_WRITE_CMD)) | (((uint32_t)(vram_addr) & 0x3FFF) << 16)
                              | ((uint32_t)(vram_addr) >> 14));
    *SMD_VDP_DATA_PORT_U16 = tile;
}

void
smd_plane_row_draw(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                     const uint16_t length, const bool defer) {
    if (defer) {
        smd_dma_transfer_enqueue(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                                       .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                                       .size = length,
                                                       .inc = 2,
                                                       .type = SMD_DMA_VRAM_TRANSFER});
    } else {
        smd_dma_transfer(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                               .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                               .size = length,
                                               .inc = 2,
                                               .type = SMD_DMA_VRAM_TRANSFER});
    }
}

void
smd_plane_row_draw_fast(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                          const uint16_t length) {
    smd_dma_transfer_fast(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                                .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                                .size = length,
                                                .inc = 2,
                                                .type = SMD_DMA_VRAM_TRANSFER});
}

void
smd_plane_column_draw(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                     const uint16_t length, const bool defer) {
    if (defer) {
        smd_dma_transfer_enqueue(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                                       .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                                       .size = length,
                                                       .inc = SMD_VDP_PLANE_WIDTH << 1,
                                                       .type = SMD_DMA_VRAM_TRANSFER});
    } else {
        smd_dma_transfer(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                               .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                               .size = length,
                                               .inc = SMD_VDP_PLANE_WIDTH << 1,
                                               .type = SMD_DMA_VRAM_TRANSFER});
    }
}

void
smd_plane_column_draw_fast(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                          const uint16_t length) {
    smd_dma_transfer_fast(&(smd_dma_transfer_t){.src = (uint16_t *)tiles,
                                                .dest = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1),
                                                .size = length,
                                                .inc = SMD_VDP_PLANE_WIDTH << 1,
                                                .type = SMD_DMA_VRAM_TRANSFER});
}

void
smd_plane_rect_draw(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                    const uint16_t width, const uint16_t height, const bool defer) {
    if (defer) {
        for (uint16_t row = 0; row < height; ++row) {
            smd_dma_transfer_enqueue(
                &(smd_dma_transfer_t){.src = (uint16_t *)tiles + (row * width),
                                      .dest = plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1),
                                      .size = width,
                                      .inc = 2,
                                      .type = SMD_DMA_VRAM_TRANSFER});
        }
    } else {
        for (uint16_t row = 0; row < height; ++row) {
            smd_dma_transfer(&(smd_dma_transfer_t){.src = (uint16_t *)tiles + (row * width),
                                                   .dest = plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1),
                                                   .size = width,
                                                   .inc = 2,
                                                   .type = SMD_DMA_VRAM_TRANSFER});
        }
    }
}

void
smd_plane_rect_draw_fast(const smd_plane_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                         const uint16_t width, const uint16_t height) {
    for (uint16_t row = 0; row < height; ++row) {
        smd_dma_transfer_fast(&(smd_dma_transfer_t){.src = (uint16_t *)tiles + (row * width),
                                                    .dest = plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1),
                                                    .size = width,
                                                    .inc = 2,
                                                    .type = SMD_DMA_VRAM_TRANSFER});
    }
}
