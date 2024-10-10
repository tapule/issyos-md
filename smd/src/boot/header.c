/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: header.c
 * Sega Megadrive/Genesis rom header
 */

#include "header.h"
#include <stdint.h>

#if SMD_HEADER_SRAM_ENABLED > 0
#define _HEADER_SRAM_ENABLED "RA"
#else
#define _HEADER_SRAM_ENABLED "  "
#endif

typedef struct
{
    char system_name[16];       /* 16B - Console name (Must start with "SEGA") */
    char copyright[16];         /* 16B - Copyright and release date */
    char domestic_name[48];     /* 48B - Domestic name */
    char overseas_name[48];     /* 48B - International name */
    char serial[14];            /* 14B - Game type and product code */
    uint16_t checksum;          /* 02B - Checksum */
    char device_support[16];    /* 16B - I/O device support */
    uint32_t rom_start_addr;    /* 04B - Start address of ROM */
    uint32_t rom_end_addr;      /* 04B - End address of ROM (4MB) */
    uint32_t ram_start_addr;    /* 04B - Start address of RAM */
    uint32_t ram_end_addr;      /* 04B - End address of RAM (64KB) */
    char sram_available[2];     /* 02B - "RA" enable SRAM, "  " disable */
    uint16_t sram_type;         /* 02B - SRAM writes to odd bytes */
    uint32_t sram_start_addr;   /* 04B - Start address of SRAM */
    uint32_t sram_end_addr;     /* 04B - End address of SRAM (64KB) */
    char modem_info[12];        /* 12B - Modem information */
    char notes[40];             /* 40B - Notes */
    char region[3];             /* 16B  - Country codes */
    char reserved[13];          /* Reserved, pad with spaces */
                                /* ------------------------ */
                                /* 256B  total */
} smd_header_t;

[[gnu::section(".text.smdheader")]]
const smd_header_t smd_header = {
    SMD_HEADER_SYS_NAME,
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
    "             "
};
