/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            smd.h
 * \brief           sdfsdfsdfsd
 *
 * The secondary
 * sdfsdfsdfsdf
 *
 * More info:
 * sdfsdfsdfsdfsdf
 */

#ifndef TCIMD_SMD_H
#define TCIMD_SMD_H

#define SMD_ROM_SIZE    (2 * 1024 * 1024)

/* Header configuration */
#define SMD_SYS_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#define SMD_SYS_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#define SMD_SYS_HEADER_DOMESTIC_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_SYS_HEADER_OVERSEAS_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_SYS_HEADER_SERIAL "GM TAPULE00-00"
#define SMD_SYS_HEADER_DEVICES "J               "
#define SMD_SYS_HEADER_ROM_SIZE SMD_ROM_SIZE
#define SMD_SYS_HEADER_SRAM_ENABLED 0
#define SMD_SYS_HEADER_NOTES "https://github.com/tapule/issyos-md     "
#define SMD_SYS_HEADER_REGION "JUE"
/* VDP configuration */
#define SMD_VDP_PLANE_A_ADDR 0xC000
#define SMD_VDP_PLANE_B_ADDR 0xE000
#define SMD_VDP_PLANE_W_ADDR 0xD000
#define SMD_VDP_SPRITE_TABLE_ADDR 0xFC00
#define SMD_VDP_HSCROLL_TABLE_ADDR 0xF800
#define SMD_VDP_PLANE_SIZE SMD_VDP_PLANE_SIZE_64X32
#define SMD_VDP_HSCROLL_MODE SMD_VDP_HSCROLL_TILE
#define SMD_VDP_VSCROLL_MODE SMD_VDP_VSCROLL_PLANE

#include "../smd/src/memory_map.h"
#include "../smd/src/sys.h"
#include "../smd/src/handlers.h"
#include "../smd/src/null_data.h"
#include "../smd/src/xgm_drv.h"
#include "../smd/src/dma.h"
#include "../smd/src/fix32.h"
#include "../smd/src/kdebug.h"
#include "../smd/src/memory.h"
#include "../smd/src/pad.h"
#include "../smd/src/pal.h"
#include "../smd/src/plane.h"
#include "../smd/src/psg.h"
#include "../smd/src/rand.h"
#include "../smd/src/sprite.h"
#include "../smd/src/text.h"
#include "../smd/src/tile.h"
#include "../smd/src/vdp.h"
#include "../smd/src/xgm.h"
#include "../smd/src/ym2612.h"
#include "../smd/src/z80.h"

#endif /* TCIMD_SMD_H */