/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: config.h
 * Configuration parameters
 *
 * Diferent configuration preferences for the Sega MegaDrive/Genesis hardware.
 * Here you can configure how the hardware sould start and behave
 * 
 */

#ifndef MEGADRIVE_CONFIG_H
#define MEGADRIVE_CONFIG_H

#include "video.h"

/* 
 * Video configuration default values
 */
/* Plane start address in VRAM */
#define VID_PLANE_A_ADDR 0xC000
#define VID_PLANE_B_ADDR 0xE000
#define VID_PLANE_W_ADDR 0xD000
/* Sprite and horizontal scroll tables address in VRAM */
#define VID_SPRITE_TABLE_ADDR  0xFC00
#define VID_HSCROLL_TABLE_ADDR 0xF800
/* Horizontal and vertical plane scroll modes */
#define VID_HSCROLL_MODE VID_HSCROLL_TILE
#define VID_VSCROLL_MODE VID_VSCROLL_PLANE 
/* Planes size */
#define VID_PLANE_SIZE VID_PLANE_SIZE_64X32 



#endif // MEGADRIVE_CONFIG_H
