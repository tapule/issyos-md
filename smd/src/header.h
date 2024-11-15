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
 *
 * The rom header comes after the m68k vector table and includes information
 * related to the current game.
 *
 * More info:
 * https://www.plutiedev.com/rom-header
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 *
 */
#ifndef SMD_HEADER_H
#define SMD_HEADER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Rom header data
 */
typedef struct smd_header_t {
    char system_name[16];       /**< 16B - Console name (Must start with "SEGA") */
    char copyright[16];         /**< 16B - Copyright and release date */
    char domestic_name[48];     /**< 48B - Domestic name */
    char overseas_name[48];     /**< 48B - International name */
    char serial[14];            /**< 14B - Game type and product code */
    uint16_t checksum;          /**< 02B - Checksum */
    char device_support[16];    /**< 16B - I/O device support */
    uint32_t rom_start_addr;    /**< 04B - Start address of ROM */
    uint32_t rom_end_addr;      /**< 04B - End address of ROM (4MB) */
    uint32_t ram_start_addr;    /**< 04B - Start address of RAM */
    uint32_t ram_end_addr;      /**< 04B - End address of RAM (64KB) */
    char sram_available[2];     /**< 02B - "RA" enable SRAM, "  " disable */
    uint16_t sram_type;         /**< 02B - SRAM writes to odd bytes */
    uint32_t sram_start_addr;   /**< 04B - Start address of SRAM */
    uint32_t sram_end_addr;     /**< 04B - End address of SRAM (64KB) */
    char modem_info[12];        /**< 12B - Modem information */
    char notes[40];             /**< 40B - Notes */
    char region[3];             /**< 16B  - Country codes */
    char reserved[13];          /**< Reserved, pad with spaces */
                                /*    ------------------------ */
                                /*    256B  total */
} smd_header_t;

/**
 * \brief           Console name. Must start with the string "SEGA" (16 chars)
 */
#ifndef SMD_HEADER_SYS_NAME
    #define SMD_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#endif

/**
 * \brief           Copyright and release date (16 chars)
 */
#ifndef SMD_HEADER_COPYRIGHT
    #define SMD_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#endif

/**
 * \brief           Game title (Domestic). Usually in uppercase (48 chars)
 */
#ifndef SMD_HEADER_DOMESTIC_NAME
    #define SMD_HEADER_DOMESTIC_NAME "SMD sample domestic name                        "
#endif

/**
 * \brief           Game title (Overseas). Usually in uppercase (48 chars)
 */
#ifndef SMD_HEADER_OVERSEAS_NAME
    #define SMD_HEADER_OVERSEAS_NAME "SMD sample overseas name                        "
#endif

/**
 * \brief           Game type and product code. Use the format "GM SERIAL-REVISION" (14 chars)
 */
#ifndef SMD_HEADER_SERIAL
    #define SMD_HEADER_SERIAL "GM  SMDSMP0-00"
#endif

/**
 * \brief           I/O device support (16 chars)
 *                  "J" for 3 buttons controller
 *                  "J6" for 3 and 6 buttons controllers
 */
#ifndef SMD_HEADER_DEVICES
    #define SMD_HEADER_DEVICES "J               "
#endif

/**
 * \brief           ROM size in bytes
 */
#ifndef SMD_HEADER_ROM_SIZE
    #define SMD_HEADER_ROM_SIZE (2 * 1024 * 1024)
#endif

/**
 * \brief           SRAM enabled (0 = FALSE, 1 = TRUE)
 */
#ifndef SMD_HEADER_SRAM_ENABLED
    #define SMD_HEADER_SRAM_ENABLED (0)
#endif

/**
 * \brief           40 chars length field for extra information
 */
#ifndef SMD_HEADER_NOTES
    #define SMD_HEADER_NOTES "                                        "
#endif

/**
 * \brief           List of supported regions (3 chars)
 *                  "J"  Japan
 *                  "U"  Americas
 *                  "E"  Europe
 */
#ifndef SMD_HEADER_REGION
    #define SMD_HEADER_REGION "JUE"
#endif

/**
 * \brief           Real Rom Header to put in the game
 */
extern const smd_header_t smd_header;

#ifdef __cplusplus
}
#endif

#endif /* SMD_HEADER_H */
