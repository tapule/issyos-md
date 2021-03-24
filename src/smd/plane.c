/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: sprite.c
 * ??
 */

#include "plane.h"
#include "dma.h"

inline void plane_clear(const uint16_t plane)
{
    dma_vram_fill(plane, VID_PLANE_TILES << 1, 0x00, 1);
}