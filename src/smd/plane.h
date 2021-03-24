/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: plane.h
 * ??
 *
 * ??
 *
 * More info:
 * 
 */

#ifndef PLANE_H
#define PLANE_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* Plane convenient defines */
#define PLANE_A VID_PLANE_A_ADDR
#define PLANE_B VID_PLANE_B_ADDR
#define PLANE_W VID_PLANE_W_ADDR

/**
 * @brief Clears an entire VDP plane
 * 
 * @param plane 
 */
void plane_clear(const uint16_t plane);

void plane_tile_draw(const uint16_t plane, const uint16_t tile,
                     const uint16_t x, const uint16_t y, const bool defer);


/*
    * TODO:
    
    plane_clear
        Clears an entire map
        It uses fill, no need to enqueue
    plane_tile_draw
        Draws a tile in a plane position
        The operation can be deferred with a parameter (sent to queue)
    plane_hline_draw
        Draws a horizontal line of tiles in a plane
        The operation can be deferred with a parameter (sent to queue)
    plane_vline_draw
        Draws a vertical line of tiles in a plane
        The operation can be deferred with a parameter (sent to queue)
    plane_rect_draw
        Draws a rectangle of tiles in a plane
        The operation can be deferred with a parameter (sent to queue)
    plane_rect_fill
        Fills a rectangle in a plane with a concrete tile
        The operation can be deferred with a parameter (sent to queue)
    
    * Notes:
    It make sense to have alternative fast version of these functions to be used
    when you know the source of tiles.    

*/
#endif /* PLANE_H */
