/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            z80.c
 * \brief           Control routines for the Zilog Z80 CPU
 */

#include "z80.h"

/**
 * \brief           Z80 control ports
 */
#define SMD_Z80_BUS_PORT    ((volatile uint16_t *) 0xA11100)
#define SMD_Z80_RESET_PORT  ((volatile uint16_t *) 0xA11200)

/**
 * \brief           Estimated reset wait time borrowed from Sik z80 tutorial.
 */
#define SMD_Z80_RESET_WAIT  (32)

/**
 * \brief           Z80 memory pointer and size (8KB)
 */
#define SMD_Z80_RAM_ADDRESS ((uint8_t *) 0xA00000)
#define SMD_Z80_RAM_SIZE    (8 * 1024)



static void
smd_z80_ram_clear(void) {
    /* We need a 0 byte, not a word */
    const uint8_t zero = 0;

    uint8_t *dest = SMD_Z80_RAM_ADDRESS;

    /* We must access the Z80 RAM using bytes, words won't work */
    for (uint16_t i = 0; i < SMD_Z80_RAM_SIZE; ++i) {
        *dest = zero;
        ++dest;
    }
}

void
smd_z80_init(void) {
    smd_z80_bus_request();
    smd_z80_ram_clear();
    /* Reset the z80 and let it run */
    smd_z80_reset();
    smd_z80_bus_release();
}

void
smd_z80_reset(void) {
    /* Assert the z80 reset line */
    *SMD_Z80_RESET_PORT = 0x000;
    /* We need to wait a while until the reset is done. */
    for (uint8_t i = 0; i < SMD_Z80_RESET_WAIT; ++i) {}
    /* Release the z80 reset line */
    *SMD_Z80_RESET_PORT = 0x100;
}

void
smd_z80_bus_request(void) {
    /* Request the bus */
    *SMD_Z80_BUS_PORT = 0x100;
    /* If there is a reset process, force it to end now */
    *SMD_Z80_RESET_PORT = 0x100;
    /* The bus is busy until it retuns a 0x100 so we wait for it */
    while (*SMD_Z80_BUS_PORT & 0x100) {}
}

inline void
smd_z80_bus_request_fast(void) {
    *SMD_Z80_BUS_PORT = 0x100;
}

inline void
smd_z80_bus_release(void) {
    *SMD_Z80_BUS_PORT = 0x000;
}

bool
smd_z80_is_bus_free(void) {
    if (*SMD_Z80_BUS_PORT & 0x100) {
        return true;
    }
    return false;
}

void
smd_z80_data_load(const uint8_t *restrict src, const uint16_t dest, uint16_t size) {
    /* Copy data to the correct offset in bytes */
    volatile uint8_t *ram_dest = SMD_Z80_RAM_ADDRESS + dest;

    while (size > 0) {
        *ram_dest = *src;
        ++ram_dest;
        ++src;
        --size;
    }
}

// void smd_z80_program_load(const uint8_t *restrict src, uint16_t size)
// {
//     smd_z80_bus_request();

//     /* Copy program to the start of z80 internal RAM */
//     smd_z80_data_load(src, 0, size);

//     smd_z80_reset();
//     smd_z80_bus_release();
// }
