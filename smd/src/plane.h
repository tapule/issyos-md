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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CHECKME: Why there is no enum for planes in this unit???
 * CHECKME: Why there is no typedef for cells??
 * CHECKME: Some of these functions should draw cells instead of tiles. This will
 *          allow us to use from simple tile indexes to full configured cells
 * CHECKME: Maybe we can do IOC in draw functions using a dma concrete function
 *          instead of having one function for each case, for example:
 *          smd_plane_hline_draw(plane, *tiles, x, y, length, defer);
 *          smd_plane_hline_draw_fast(plane, *tiles, x, y, length);
 *          -> smd_plane_row_draw(smd_dma_func, plane, *tiles, x, y, length);
 * CHECKME: Maybe we can use an struct in drawing functions instead of so much params
 *          struct smd_plane_operation {
 *              plane
 *              *tiles
 *              point (vect2d_t)
 *              union {
 *                  length
 *                  width
 *              }
 *              height
 *          }
 *          -> smd_plane_row_draw(smd_dma_func, *smd_plane_operation);
 *              smd_plane_hline_draw(SMD_PLANE_A, text, 2, 4, size, false);
 *              smd_plane_row_draw(smd_dma_vram_transfer, &(smd_plane_operation) {
 *                  .plane = SMD_PLANE_A,
 *                  .tiles = text,
 *                  .point.x = 2,
 *                  .point.y = 4,
 *                  .length = size
 *              });
 */

/**
 * \brief           Configure a plane cell tile with all its draw properties
 * \param[in]       tile_index: VRam index of tile to drawn
 * \param[in]       palette: CRam palette index (0..3)
 * \param[in]       h_flip: Horizontal flip property (0 no flip, 1 flip horizontally)
 * \param[in]       v_flip: Vertical flip property (0 no flip, 1 flip vertically)
 * \param[in]       priority: Drawing priority (0 low priority, 1 high priority)
 * \return          Plane cell with all the properties configured in
 */
uint16_t smd_plane_cell_config(const uint16_t tile_index, const uint16_t palette, const uint16_t h_flip,
                           const uint16_t v_flip, const uint16_t priority);

/**
 * \brief           Clear an entire VDP plane
 * \param[in]       plane: Plane to clear
 * \note            This function clears the plane immediately. Use it wisely
 *                  with the display off or in the vertical blank, otherwise you
 *                  can get some glitches.
 */
void smd_plane_clear(const uint16_t plane);

/**
 * \brief           Draw a rectangle of tiles in a concrete position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tile: Tile index or a full cell tile config to use as fill value
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       width: Rectangle width in tiles
 * \param[in]       height: Rectangle height in tiles
 * \note            This function draws the plane immediately. Use it wisely
 *                  with the display off or in the vertical blank, otherwise you
 *                  can get some glitches.
 */
void smd_plane_rect_fill(const uint16_t plane, const uint16_t tile, const uint16_t x, const uint16_t y,
                     const uint16_t width, const uint16_t height);

/**
 * \brief           Draw a tile in a concrete position of a plane
 * \param           plane: Destination plane where the tile should be drawn
 * \param           tile: Tile index or a full cell tile config
 * \param           x: Plane horizontal position in cells
 * \param           y: Plane vertical position in cells
 * \note            This function draws the tile immediately. Use it wisely with
 *                  the display off or in the vertical blank, otherwise you can
 *                  get some glitches.
 */
void smd_plane_tile_draw(const uint16_t plane, const uint16_t tile, const uint16_t x, const uint16_t y);

/**
 * \brief           Draw a horizontal line of tiles in a concrete position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       length: Line of tiles length
 * \param[in]       defer: True to enqueue the operation, false to do it directly
 */
/* CHECKME: smd_plane_hline_draw -> smd_plane_row_draw */
void smd_plane_hline_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                      const uint16_t length, const bool defer);

/**
 * \brief           Draws faster a horizontal line of tiles in a position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       length: Line of tiles length
 * \note            This function is meant to use RAM as tile's data source. To
 *                  use it from ROM, make sure to check 128kB boundaries. It also
 *                  draws the line immediately. Use it wisely with the display
 *                  off or in the vertical blank, otherwise you will get some
 *                  glitches.
 */
void smd_plane_hline_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                           const uint16_t length);

/**
 * \brief           Draw a vertical line of tiles in a concrete position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       length: Line of tiles length
 * \param[in]       defer: True to enqueue the operation, false to do it directly
 */
/* CHECKME: smd_plane_vline_draw -> smd_plane_column_draw */
void smd_plane_vline_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                      const uint16_t length, const bool defer);

/**
 * \brief           Draw faster a vertical line of tiles in a position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       length: Line of tiles length
 * \note            This function is meant to use RAM as tile's data source. To
 *                  use it from ROM, make sure to check 128kB boundaries. It also
 *                  draws the line immediately. Use it wisely with the display
 *                  off or in the vertical blank, otherwise you will get some
 *                  glitches.
 */
void smd_plane_vline_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                           const uint16_t length);

/**
 * \brief           Draw a rectangle of tiles in a concrete position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       width: Rectangle width in tiles
 * \param[in]       height: Rectangle height in tiles
 * \param[in]       defer: True to enqueue the operation, false to do it directly
 */
void smd_plane_rect_draw(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                     const uint16_t width, const uint16_t height, const bool defer);

/**
 * \brief           Draw faster a rectangle of tiles in a concrete position of a plane
 * \param[in]       plane: Destination plane where tiles should be drawn
 * \param[in]       tiles: Source tiles indexes or full cells tiles configurations
 * \param[in]       x: Plane horizontal position in cells
 * \param[in]       y: Plane vertical position in cells
 * \param[in]       width: Rectangle width in tiles
 * \param[in]       height: Rectangle height in tiles
 * \note            This function is meant to use RAM as tile's data source. To
 *                  use it from ROM, make sure to check 128kB boundaries. It also
 *                  draws the line immediately. Use it wisely with the display
 *                  off or in the vertical blank, otherwise you will get some
 *                  glitches.
 */
void smd_plane_rect_draw_fast(const uint16_t plane, const uint16_t *restrict tiles, const uint16_t x, const uint16_t y,
                          const uint16_t width, const uint16_t height);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PLANE_H */
