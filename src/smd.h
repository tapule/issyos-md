/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: smd.h
 * sdfsdfsdfsd
 *
 * The secondary
 * sdfsdfsdfsdf
 *
 * More info:
 * sdfsdfsdfsdfsdf
 */

#ifndef SMD_H
#define SMD_H

#define SMD_ROM_SIZE    (2 * 1024 * 1024)

/* Header configuration */
#define SMD_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#define SMD_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#define SMD_HEADER_DOMESTIC_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_HEADER_OVERSEAS_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_HEADER_SERIAL "GM TAPULE00-00"
#define SMD_HEADER_DEVICES "J               "
#define SMD_HEADER_ROM_SIZE SMD_ROM_SIZE
#define SMD_HEADER_SRAM_ENABLED 0
#define SMD_HEADER_NOTES "https://github.com/tapule/issyos-md     "
#define SMD_HEADER_REGION "JUE"
/* VDP configuration */
#define SMD_VDP_PLANE_A_ADDR 0xC000
#define SMD_VDP_PLANE_B_ADDR 0xE000
#define SMD_VDP_PLANE_W_ADDR 0xD000
#define SMD_VDP_SPRITE_TABLE_ADDR 0xFC00
#define SMD_VDP_HSCROLL_TABLE_ADDR 0xF800
#define SMD_VDP_PLANE_SIZE SMD_VDP_PLANE_SIZE_64X32
#define SMD_VDP_HSCROLL_MODE SMD_VDP_HSCROLL_TILE
#define SMD_VDP_VSCROLL_MODE SMD_VDP_VSCROLL_PLANE

#include "../smd/src/header.h"
#include "../smd/src/handlers.h"
#include "../smd/src/z80.h"
#include "../smd/src/ym2612.h"
#include "../smd/src/xgm.h"
#include "../smd/src/xgm_drv.h"
#include "../smd/src/vdp.h"
#include "../smd/src/tile.h"
#include "../smd/src/text.h"
#include "../smd/src/sys.h"
#include "../smd/src/sprite.h"
#include "../smd/src/rand.h"
#include "../smd/src/psg.h"

#endif /* SMD_H */