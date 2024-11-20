/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            sys.h
 * \brief           System core routines to boot and manage the Sega
 *                  Megadrive/Genesis hardware
 *
 * More info:
 * https://www.eeeguide.com/register-architecture-of-68000-microprocessor/
 * https://www.plutiedev.com/rom-header
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 */

/* TODO: Este módulo está en desarrollo. Faltan muchas funciones por implementar. */

#ifndef SMD_SYS_H
#define SMD_SYS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           SMD framework version
 */
#define SMD_VERSION 0.1

/**
 * \brief           Console name. Must start with the string "SEGA" (16 chars)
 */
#ifndef SMD_SYS_HEADER_SYS_NAME
    #define SMD_SYS_HEADER_SYS_NAME "SEGA MEGA DRIVE "
#endif

/**
 * \brief           Copyright and release date (16 chars)
 */
#ifndef SMD_SYS_HEADER_COPYRIGHT
    #define SMD_SYS_HEADER_COPYRIGHT "TAPULE  2024.UNK"
#endif

/**
 * \brief           Game title (Domestic). Usually in uppercase (48 chars)
 */
#ifndef SMD_SYS_HEADER_DOMESTIC_NAME
    #define SMD_SYS_HEADER_DOMESTIC_NAME "SMD sample domestic name                        "
#endif

/**
 * \brief           Game title (Overseas). Usually in uppercase (48 chars)
 */
#ifndef SMD_SYS_HEADER_OVERSEAS_NAME
    #define SMD_SYS_HEADER_OVERSEAS_NAME "SMD sample overseas name                        "
#endif

/**
 * \brief           Game type and product code. Use the format "GM SERIAL-REVISION" (14 chars)
 */
#ifndef SMD_SYS_HEADER_SERIAL
    #define SMD_SYS_HEADER_SERIAL "GM  SMDSMP0-00"
#endif

/**
 * \brief           I/O device support (16 chars)
 *                  "J" for 3 buttons controller
 *                  "J6" for 3 and 6 buttons controllers
 */
#ifndef SMD_SYS_HEADER_DEVICES
    #define SMD_SYS_HEADER_DEVICES "J               "
#endif

/**
 * \brief           ROM size in bytes
 */
#ifndef SMD_SYS_HEADER_ROM_SIZE
    #define SMD_SYS_HEADER_ROM_SIZE (2 * 1024 * 1024)
#endif

/**
 * \brief           SRAM enabled (0 = FALSE, 1 = TRUE)
 */
#ifndef SMD_SYS_HEADER_SRAM_ENABLED
    #define SMD_SYS_HEADER_SRAM_ENABLED (0)
#endif

/**
 * \brief           40 chars length field for extra information
 */
#ifndef SMD_SYS_HEADER_NOTES
    #define SMD_SYS_HEADER_NOTES "                                        "
#endif

/**
 * \brief           List of supported regions (3 chars)
 *                  "J"  Japan
 *                  "U"  Americas
 *                  "E"  Europe
 */
#ifndef SMD_SYS_HEADER_REGION
    #define SMD_SYS_HEADER_REGION "JUE"
#endif

/**
 * \brief           Sega Megadrive/Genesis rom header
 *
 * The rom header comes after the m68k vector table and includes information
 * related to the current game.
 */
typedef struct smd_sys_header_t {
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
} smd_sys_header_t;

/**
 * \brief           Real Rom Header to put in the game
 */
extern const smd_sys_header_t smd_sys_header;

/**
 * \brief           Enable system interrupts
 *
 * The m68k provides 7 levels of interrupts were 7 has the highest priority and
 * 0 the lowest. For the Sega Megadrive/Genesis the most important interrupts
 * are:
 *  Level 6: Vertical retrace interrupt
 *  Level 4: Horizontal retrace interrupt
 *  Level 2: External interrupt
 * This function enables all of them
 */
void smd_ints_enable(void);

/**
 * \brief           Disable system interrupts
 */
void smd_ints_disable(void);

/**
 * \brief           Get interrupts status
 * \return          true if interrupts are enabled, false otherwise
 */
bool smd_ints_status(void);

/**
 * \brief           Check if the system is using PAL or NTSC video mode
 *
 * The Sega Megadrive/Genesis can run in PAL mode with a clock of 7.60 MHz or in
 * NTSC with a clock of 7.67 MHz.
 *
 * \return          true if the system is in PAL mode, otherwise it is in NTSC
 */
bool smd_is_pal(void);

/**
 * \brief           Check if the system is running in Japanese (Domestic) mode
 *
 * The Sega Megadrive/Genesis can run in Domestic mode (Japanese) or in Overseas
 * mode (US or European).
 *
 * \return          true if the system is running in Japanese mode, otherwise it
 *                  is in US/European mode
 */
bool smd_is_japanese(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_SYS_H */
