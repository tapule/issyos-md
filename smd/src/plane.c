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
#include "mem_map.h"
#include "vdp.h"

inline smd_plane_cell
smd_plane_cell_make(const smd_plane_cell_desc_t *restrict cell_desc) {
    return (cell_desc->priority << 15) | (cell_desc->palette << 13) | (cell_desc->v_flip << 12)
           | (cell_desc->h_flip << 11) | cell_desc->tile_index;
}

inline void
smd_plane_clear(const smd_plane_t plane) {
    smd_dma_vram_fill(plane, SMD_VDP_PLANE_TILES << 1, 0x00, 1);
}

void
smd_plane_cell_draw(const smd_plane_draw_desc_t *restrict draw_desc) {
    uint16_t vram_addr;

    /* It doesn't make sense to use DMA for only one tile. Write it directly  */
    vram_addr = draw_desc->plane + ((draw_desc->x + (draw_desc->y * SMD_VDP_PLANE_WIDTH)) << 1);
    *SMD_VDP_CTRL_PORT_U32 = (((uint32_t)(SMD_VDP_VRAM_WRITE_CMD)) | (((uint32_t)(vram_addr) & 0x3FFF) << 16)
                              | ((uint32_t)(vram_addr) >> 14));
    *SMD_VDP_DATA_PORT_U16 = draw_desc->cell;
}

void
smd_plane_row_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc) {

    dma_func(
        &(smd_dma_transfer_t){.src = (uint16_t *)draw_desc->cells,
                              .dest = draw_desc->plane + ((draw_desc->x + (draw_desc->y * SMD_VDP_PLANE_WIDTH)) << 1),
                              .size = draw_desc->length,
                              .inc = 2,
                              .type = SMD_DMA_VRAM_TRANSFER});
}

void
smd_plane_column_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc) {
    dma_func(
        &(smd_dma_transfer_t){.src = (uint16_t *) draw_desc->cells,
                            .dest = draw_desc->plane + ((draw_desc->x + (draw_desc->y * SMD_VDP_PLANE_WIDTH)) << 1),
                            .size = draw_desc->length,
                            .inc = SMD_VDP_PLANE_WIDTH << 1,
                            .type = SMD_DMA_VRAM_TRANSFER});
}

void
smd_plane_rect_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc) {
    for (uint16_t row = 0; row < draw_desc->height; ++row) {
        dma_func(
            &(smd_dma_transfer_t){.src = (uint16_t *) draw_desc->cells + (row * draw_desc->width),
                                    .dest = draw_desc->plane + ((draw_desc->x + ((draw_desc->y + row) * SMD_VDP_PLANE_WIDTH)) << 1),
                                    .size = draw_desc->width,
                                    .inc = 2,
                                    .type = SMD_DMA_VRAM_TRANSFER});
    }
}

void
smd_plane_rect_fill(const smd_plane_draw_desc_t *restrict draw_desc) {
    uint16_t tile_row[SMD_VDP_PLANE_WIDTH];

    /* Setup tile buffer */
    for (uint16_t i = 0; i < draw_desc->width; ++i) {
        tile_row[i] = draw_desc->cell;
    }
    /* Draws rows in the plane */
    for (uint16_t i = 0; i < draw_desc->height; ++i) {
        /* x, y, i and SMD_VDP_PLANE_WIDTH are in tiles, convert to words */
        smd_dma_transfer_fast(&(smd_dma_transfer_t){.src = &tile_row,
                                        .dest = draw_desc->plane + ((draw_desc->x + ((draw_desc->y + i) * SMD_VDP_PLANE_WIDTH)) << 1),
                                        .size = draw_desc->width,
                                        .inc = 2,
                                        .type = SMD_DMA_VRAM_TRANSFER});
    }
}
