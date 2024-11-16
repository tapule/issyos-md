/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            ym2612.c
 * \brief           Control routines for Sega Megadrive/Genesis Yamaha YM2612 FM
 *                  sound chip
 */

#include <stdint.h>
#include "ym2612.h"
#include "memory_map.h"
#include "z80.h"

/**
 * \brief           Wait for the YM2112 to be ready to receive new data
 */
static inline void
smd_ym2612_wait(void) {
    /*
     * Wait while the YM2612 is busy reading bit 7 on 0xA04000.
     * It is recommended to read only from this port as several revisions of
     * this hardware may fail to read status from other port.
     * https://plutiedev.com/blog/20200103
     */
    while (*SMD_YM2612_FM1_ADDRESS_PORT & 0x80) {}
}

/**
 * \brief           Select the register to operate on the FM1 set
 * \param[in]       reg: Register index to write on
 */
static inline void
smd_ym2612_fm1_reg_select(const uint8_t reg) {
    smd_ym2612_wait();
    *SMD_YM2612_FM1_ADDRESS_PORT = reg;
}

/**
 * \brief           Select the register to operate on the FM2 set
 * \param           reg Register index to write on
 */
static inline void
smd_ym2612_fm2_reg_select(const uint8_t reg) {
    smd_ym2612_wait();
    *SMD_YM2612_FM2_ADDRESS_PORT = reg;
}

/**
 * \brief           Write data to the FM1 data port
 * \param[in]       data: Data value to send to the data port
 */
static inline void
smd_ym2612_fm1_data_write(const uint8_t data) {
    smd_ym2612_wait();
    *SMD_YM2612_FM1_DATA_PORT = data;
}

/**
 * \brief           Write data to the FM2 data port
 * \param[in]       data: Data value to send to the data port
 */
static inline void
smd_ym2612_fm2_data_write(const uint8_t data) {
    smd_ym2612_wait();
    *SMD_YM2612_FM2_DATA_PORT = data;
}

/**
 * \brief           Write data to a concrete register index in the FM1 set
 * \param           reg: Register index to write data to
 * \param           data: Data to be written
 */
static inline void
smd_ym2612_fm1_write(const uint8_t reg, const uint8_t data) {
    smd_ym2612_fm1_reg_select(reg);
    smd_ym2612_fm1_data_write(data);
}

/**
 * \brief           Write data to a concrete register index in the FM2 set
 * \param           reg: Register index to write data to
 * \param           data: Data to be written
 */
static inline void
smd_ym2612_fm2_write(const uint8_t reg, const uint8_t data) {
    smd_ym2612_fm2_reg_select(reg);
    smd_ym2612_fm2_data_write(data);
}

void
smd_ym2612_init(void) {
    uint8_t reg;

    smd_z80_bus_request();

    /* Disable DAC */
    smd_ym2612_fm1_write(0x2B, 0x00);

    /* Mute all FM Channels */
    reg = 0x40;
    for (uint8_t i = 0; i < 4; ++i) {
        smd_ym2612_fm1_write(reg, 0x7F);
        smd_ym2612_fm2_write(reg, 0x7F);
        ++reg;
        smd_ym2612_fm1_write(reg, 0x7F);
        smd_ym2612_fm2_write(reg, 0x7F);
        ++reg;
        smd_ym2612_fm1_write(reg, 0x7F);
        smd_ym2612_fm2_write(reg, 0x7F);
        ++reg;
        ++reg;
    }

    /* Enable left and right output for all channels */
    for (uint8_t i = 0; i < 3; ++i) {
        smd_ym2612_fm1_write(0xB4 + i, 0xC0);
        smd_ym2612_fm2_write(0xB4 + i, 0xC0);
    }

    /* Disable LFO */
    smd_ym2612_fm1_write(0x22, 0x00);

    /* Disable timers A and B and set channel 6 to normal mode */
    smd_ym2612_fm1_write(0x27, 0x00);

    /* All key off */
    smd_ym2612_fm1_reg_select(0x28);
    for (uint8_t i = 0; i < 3; ++i) {
        smd_ym2612_fm1_data_write(0x00 + i);
        smd_ym2612_fm1_data_write(0x04 + i);
    }

    smd_z80_bus_release();
}
