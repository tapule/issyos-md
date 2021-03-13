/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: header.s
 * Sega Megadrive/Genesis rom header
 * 
 * Comes after the m68k vector table and includes information related to the
 * current game
 *
 * More info:
 * https://www.plutiedev.com/rom-header
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 */

.section .text.smdheader
_smd_header:
    .ascii  "SEGA MEGA DRIVE "      /* 16B - Console name (Must start with "SEGA") */
    .ascii  "TAPULE  2021.DEC"      /* 16B - Copyright and release date */
    /*.ascii  "The Curse of Issyos for Sega MegaDrive/Genesis  "*/ /* 48B - Domestic name */    
    /*.ascii  "The Curse of Issyos for Sega MegaDrive/Genesis  "*/ /* 48B - International name */
    .ascii  "The Is for Sega MegaDrive/Genesis               " /* 48B - Domestic name */    
    .ascii  "The Is for Sega MegaDrive/Genesis               " /* 48B - International name */
    .ascii  "GM TAPULE00-00"        /* 14B - Game type and product code */    
    .word   0                       /* 02B - Checksum */
    .ascii  "J               "      /* 16B - I/O device support */
    .long   0x00000000              /* 04B - Start address of ROM */
    .long   0x003FFFFF              /* 04B - End address of ROM (4MB) */
    .long   0x00FF0000              /* 04B - Start address of RAM */
    .long   0x00FFFFFF              /* 04B - End address of RAM (64KB) */
    .ascii  "  "                    /* 02B - "RA" enable SRAM, "  " disable */
    .word   0xF820                  /* 02B - SRAM writes to odd bytes */
    .long   0x200001                /* 04B - Start address of SRAM */
    .long   0x20FFFF                /* 04B - End address of SRAM (64KB) */
    .ascii  "            "          /* 12B - Modem support */
    .ascii  "https://github.com/tapule/issyos-md     " /* 40B - Notes */    
    .ascii  "JUE             "      /* 16B  - Country codes */
                                    /* ------------------------ */
                                    /* 256B  total */    
