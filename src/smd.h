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

#define SMD_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#define SMD_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#define SMD_HEADER_DOMESTIC_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_HEADER_OVERSEAS_NAME "The Curse of Issyos for Sega MegaDrive/Genesis  "
#define SMD_HEADER_SERIAL "GM TAPULE00-00"
#define SMD_HEADER_DEVICES "J               "
#define SMD_HEADER_ROM_SIZE (2 * 1024 * 1024)
#define SMD_HEADER_SRAM_ENABLED 0
#define SMD_HEADER_NOTES "https://github.com/tapule/issyos-md     "
#define SMD_HEADER_REGION "JUE"


#include "../smd/src/boot/boot.h"
#include "../smd/src/boot/header.h"
#include "../smd/src/boot/handlers.h"

#include "../smd/src/z80.h"

#endif /* SMD_H */