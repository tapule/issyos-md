/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: header.s
 * Sega Megadrive/Genesis rom header
 *
 * Comes after the m68k vector table and includes information related to the
 * current game.
 *
 * More info:
 * https://www.plutiedev.com/rom-header
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 *
 */
#ifndef SMD_HEADER_H
#define SMD_HEADER_H

// Console name. Must start with the string "SEGA" (16 chars)
#ifndef SMD_HEADER_SYS_NAME
    #define SMD_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#endif

// Copyright and release date (16 chars)
#ifndef SMD_HEADER_COPYRIGHT
    #define SMD_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#endif

// Game title (Domestic). Usually in uppercase (48 chars)
#ifndef SMD_HEADER_DOMESTIC_NAME
    #define SMD_HEADER_DOMESTIC_NAME "SMD sample domestic name                        "
#endif

// Game title (Overseas). Usually in uppercase (48 chars)
#ifndef SMD_HEADER_OVERSEAS_NAME
    #define SMD_HEADER_OVERSEAS_NAME "SMD sample overseas name                        "
#endif

// Game type and product code. Use the format "GM SERIAL-REVISION" (14 chars)
#ifndef SMD_HEADER_SERIAL
    #define SMD_HEADER_SERIAL "GM  SMDSMP0-00"
#endif

// I/O device support (16 chars)
// "J" for 3 buttons controller
// "J6" for 3 and 6 buttons controllers
#ifndef SMD_HEADER_DEVICES
    #define SMD_HEADER_DEVICES "J               "
#endif

// ROM size in bytes
#ifndef SMD_HEADER_ROM_SIZE
    #define SMD_HEADER_ROM_SIZE (2 * 1024 * 1024)
#endif

// SRAM enabled (0 = FALSE, 1 = TRUE)
#ifndef SMD_HEADER_SRAM_ENABLED
    #define SMD_HEADER_SRAM_ENABLED 0
#endif

// 40 chars length field for extra information
#ifndef SMD_HEADER_NOTES
    #define SMD_HEADER_NOTES "                                        "
#endif

// List of supported regions (3 chars)
// "J"  Japan
// "U"  Americas
// "E"  Europe
#ifndef SMD_HEADER_REGION
    #define SMD_HEADER_REGION "JUE"
#endif

#endif /* SMD_HEADER_H */