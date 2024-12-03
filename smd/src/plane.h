/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            plane.h
 * \brief           VDP's planes drawing functions
 *
 * VDP's planes are tables stored in VRam with cells that define what patterns
 * (aka tiles) should be drawn on screen. Each cell in these tables is a word
 * (16 bits) describing drawing properties and VRAM tile index to draw according
 * to this format:
 *      PCCVHTTTTTTTTTTT
 *      P: Priority flag
 *      C: Palete select
 *      V: Vertical flip flag
 *      H: Horizontal flip flag
 *      T: Tile index in VRam to drawn
 *
 * More info:
 * https://blog.bigevilcorporation.co.uk/2012/03/23/sega-megadrive-4-hello-world/
 */

#ifndef SMD_PLANE_H
#define SMD_PLANE_H

#include <stdint.h>
#include "vdp.h"
#include "dma.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Availables planes in the VDP
 * \note            Map each plane with its starting address in VRAM which lets us
 *                  write directly in each plane
 */
typedef enum smd_plane_t {
    SMD_PLANE_A = SMD_VDP_PLANE_A_ADDR,
    SMD_PLANE_B = SMD_VDP_PLANE_B_ADDR,
    SMD_PLANE_W = SMD_VDP_PLANE_W_ADDR
} smd_plane_t;

/**
 * \brief           Convenient alias for a complete plane cell
 */
typedef uint16_t smd_plane_cell;

/**
 * \brief           Plane cell drawing properties description
 */
typedef struct smd_plane_cell_desc_t {
    uint16_t tile_index;        /**< Tile index in VRam to drawn */
    uint8_t palette;            /**< Palete select */
    uint8_t h_flip;             /**< Horizontal flip flag */
    uint8_t v_flip;             /**< Vertical flip flag */
    uint8_t priority;           /**< Priority flag */
} smd_plane_cell_desc_t;

/**
 * \brief           Plane drawing operation description
 */
typedef struct smd_plane_draw_desc_t {
    smd_plane_t plane;          /**< Destination drawing plane */
    /**
     * \brief           Cell or cells buffer to draw depending on the function
     */
    union {
        smd_plane_cell cell;    /**< Unique cell used in cell_draw and rect_fill */
        smd_plane_cell *cells;  /**< Cells buffer to draw by other functions */
    };
    uint16_t x;                 /**< Plane horizontal drawing position in cells */
    uint16_t y;                 /**< Plane vertical drawing position in cells */
    /**
     * \brief           Sizes of drawing operation
     */
    union {
        uint16_t length;        /**< Row or column drawing length */
        /**
         * \brief       Rect drawing dimensions
         */
        struct {
            uint16_t width;     /**< Rect drawing width */
            uint16_t height;    /**< Rect drawing height */
        };
    };
} smd_plane_draw_desc_t;

/**
 * \brief           Build a plane cell with all its drawing properties
 * \param[in]       cell_desc: Cell description data
 * \return          Plane cell with all the properties configured in
 */
smd_plane_cell smd_plane_cell_make(const smd_plane_cell_desc_t *restrict cell_desc);

/**
 * \brief           Clear an entire VDP plane using DMA
 * \param[in]       plane: Plane to clear
 * \note            This function clears the plane immediately. Use it wisely
 *                  with the display off or in the vertical blank, otherwise you
 *                  can get some glitches.
 */
void smd_plane_clear(const smd_plane_t plane);

/**
 * \brief           Draw a cell in a concrete plane position
 * \param[in]       draw_desc: Cell drawing operation description
 * \note            This function draws the cell immediately whitouth dma. Use
 *                  it wisely with the display off or in the vertical blank,
 *                  otherwise you can get some glitches.
 */
void smd_plane_cell_draw(const smd_plane_draw_desc_t *restrict draw_desc);

/**
 * \brief           Draw a row of cells in a concrete plane position using DMA
 * \param[in]       dma_func: DMA function to use in the operation. It must be one of
 *                      smd_dma_transfer
 *                      smd_dma_transfer_fast
 *                      smd_dma_transfer_enqueue
 * \param[in]       draw_desc: Row drawing operation description
 */
void smd_plane_row_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc);

/**
 * \brief           Draw a column of cells in a concrete plane position using DMA
 * \param[in]       dma_func: DMA function to use in the operation. It must be one of
 *                      smd_dma_transfer
 *                      smd_dma_transfer_fast
 *                      smd_dma_transfer_enqueue
 * \param[in]       draw_desc: Column drawing operation description
 */
void smd_plane_column_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc);

/**
 * \brief           Draw a rect of cells in a concrete plane position using DMA
 * \param[in]       dma_func: DMA function to use in the operation. It must be one of
 *                      smd_dma_transfer
 *                      smd_dma_transfer_fast
 *                      smd_dma_transfer_enqueue
 * \param[in]       draw_desc: Rect drawing operation description
 */
void smd_plane_rect_draw(const smd_dma_transfer_ft dma_func, const smd_plane_draw_desc_t *restrict draw_desc);

/**
 * \brief           Fill a rectangle with a cell in a concrete plane position using DMA
 * \param[in]       draw_desc: Fill drawing operation description
 * \note            This function draws the plane immediately. Use it wisely
 *                  with the display off or in the vertical blank, otherwise you
 *                  can get some glitches.
 */
/* CHECKME: This function uses an internal buffer, so we can't do dma enqueue here. I don't really like this */
void smd_plane_rect_fill(const smd_plane_draw_desc_t *restrict draw_desc);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PLANE_H */
