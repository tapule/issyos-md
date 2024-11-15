/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            header.s
 * \brief           Sega Megadrive/Genesis rom header
 */

#include "header.h"

#if SMD_HEADER_SRAM_ENABLED > 0
    #define _HEADER_SRAM_ENABLED "RA"
#else
    #define _HEADER_SRAM_ENABLED "  "
#endif

[[gnu::section(".text.smdheader")]]
const smd_header_t smd_header = {SMD_HEADER_SYS_NAME,
                                 SMD_HEADER_COPYRIGHT,
                                 SMD_HEADER_DOMESTIC_NAME,
                                 SMD_HEADER_OVERSEAS_NAME,
                                 SMD_HEADER_SERIAL,
                                 0,
                                 SMD_HEADER_DEVICES,
                                 0x00000000,
                                 SMD_HEADER_ROM_SIZE - 1,
                                 0x00FF0000,
                                 0x00FFFFFF,
                                 _HEADER_SRAM_ENABLED,
                                 0xF820,
                                 0x200001,
                                 0x20FFFF,
                                 "            ",
                                 SMD_HEADER_NOTES,
                                 SMD_HEADER_REGION,
                                 "             "};
