/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            plane.c
 * \brief           VDP's planes drawing functions
 */

#include "plane.h"
#include "dma.h"
#include "vdp.h"

inline uint16_t
smd_plane_cell_config(const uint16_t tile_index, const uint16_t palette, const uint16_t h_flip, const uint16_t v_flip,
                  const uint16_t priority) {
    return (priority << 15) | (palette << 13) | (v_flip << 12) | (h_flip << 11) | tile_index;
}

inline void
smd_plane_clear(const uint16_t plane) {
    smd_dma_vram_fill(plane, SMD_VDP_PLANE_TILES << 1, 0x00, 1);
}

void
smd_plane_rect_fill(const uint16_t plane, const uint16_t tile, const uint16_t x, const uint16_t y, const uint16_t width,
                const uint16_t height) {
    uint16_t tile_row[SMD_VDP_PLANE_WIDTH];

    /* Setup tile buffer */
    for (uint16_t i = 0; i < width; ++i) {
        tile_row[i] = tile;
    }
    /* Draws rows in the plane */
    for (uint16_t i = 0; i < height; ++i) {
        /* x, y, i and SMD_VDP_PLANE_WIDTH are in tiles, convert to words */
        smd_dma_vram_transfer_fast(&tile_row, plane + ((x + ((y + i) * SMD_VDP_PLANE_WIDTH)) << 1), width, 2);
    }
}

void
smd_plane_tile_draw(const uint16_t plane, const uint16_t tile, const uint16_t x, const uint16_t y) {
    uint16_t vram_addr;

    /* It doesn't make sense to use DMA for only one tile. Write it directly  */
    vram_addr = plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1);
    *SMD_VDP_PORT_CTRL_L = (((uint32_t)(SMD_VDP_VRAM_WRITE_CMD)) | (((uint32_t)(vram_addr) & 0x3FFF) << 16)
                            | ((uint32_t)(vram_addr) >> 14));
    *SMD_VDP_PORT_DATA_W = tile;
}

void
smd_plane_hline_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                 const uint16_t length, const bool defer) {
    if (defer) {
        smd_dma_queue_vram_transfer(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length, 2);
    } else {
        smd_dma_vram_transfer(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length, 2);
    }
}

void
smd_plane_hline_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                      const uint16_t length) {
    smd_dma_vram_transfer_fast(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length, 2);
}

void
smd_plane_vline_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                 const uint16_t length, const bool defer) {
    if (defer) {
        smd_dma_queue_vram_transfer(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length,
                                SMD_VDP_PLANE_WIDTH << 1);
    } else {
        smd_dma_vram_transfer(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length, SMD_VDP_PLANE_WIDTH << 1);
    }
}

void
smd_plane_vline_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                      const uint16_t length) {
    smd_dma_vram_transfer_fast(tiles, plane + ((x + (y * SMD_VDP_PLANE_WIDTH)) << 1), length, SMD_VDP_PLANE_WIDTH << 1);
}

void
smd_plane_rect_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                const uint16_t width, const uint16_t height, const bool defer) {
    if (defer) {
        for (uint16_t row = 0; row < height; ++row) {
            smd_dma_queue_vram_transfer(tiles + (row * width), plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1),
                                    width, 2);
        }
    } else {
        for (uint16_t row = 0; row < height; ++row) {

            smd_dma_vram_transfer(tiles + (row * width), plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1), width, 2);
        }
    }
}

void
smd_plane_rect_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                     const uint16_t width, const uint16_t height) {
    for (uint16_t row = 0; row < height; ++row) {
        smd_dma_vram_transfer_fast(tiles + (row * width), plane + ((x + ((y + row) * SMD_VDP_PLANE_WIDTH)) << 1), width, 2);
    }
}
