/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: ym2612.c
 * Control routines for Sega Megadrive/Genesis Yamaha YM2612 FM sound chip
 */

#include <stdint.h>
#include "ym2612.h"
#include "z80.h"

/*
 * The internal refisters of the FM YM2612 are divided in two sets:
 *  FM1: LFO, Timers, Key On/Off, DAC, FM Channels 1..3
 *  FM2: FM Channels 4..6
 * Each set has its own register address and data ports which has to be written
 * in byte size.
 * These ports are accessed through memory location 0xA04000..0xA04003 from the
 * m68k side.
 */
#define SMD_YM2612_FM1_PORT_ADDRESS ((volatile uint8_t *) 0xA04000)
#define SMD_YM2612_FM1_PORT_DATA    ((volatile uint8_t *) 0xA04001)
#define SMD_YM2612_FM2_PORT_ADDRESS ((volatile uint8_t *) 0xA04002)
#define SMD_YM2612_FM2_PORT_DATA    ((volatile uint8_t *) 0xA04003)
// CHECKME: No debería poner los registros con defines en lugar de magic nums??


/**
 * @brief Waits the YM2112 to be ready to receive new data
 */
inline void smd_ym2612_wait(void)
{
    /*
     * Wait while the YM2612 is busy reading bit 7 on 0xA04000.
     * It is recommended to read only from this port as several revisions of
     * this hardware may fail to read status from other port.
     * https://plutiedev.com/blog/20200103
     */
    while(*SMD_YM2612_FM1_PORT_ADDRESS & 0X80)
    {
    }
}

/**
 * @brief Writes the regiter index to operate with to the FM1 address port
 *
 * @param reg Register index to write
 */
inline void smd_ym2612_fm1_addr_write(const uint8_t reg)
{
    smd_ym2612_wait();
    *SMD_YM2612_FM1_PORT_ADDRESS = reg;
}

/**
 * @brief Writes data to the FM1 data port
 *
 * @param data Data value to send to the data port
 */
inline void smd_ym2612_fm1_data_write(const uint8_t data)
{
    smd_ym2612_wait();
    *SMD_YM2612_FM1_PORT_DATA = data;
}

/**
 * @brief Writes the regiter index to operate with to the FM2 address port
 *
 * @param reg Register index to write
 */
inline void smd_ym2612_fm2_addr_write(const uint8_t reg)
{
    smd_ym2612_wait();
    *SMD_YM2612_FM2_PORT_ADDRESS = reg;
}

/**
 * @brief Writes data to the FM2 data port
 *
 * @param data Data value to send to the data port
 */
inline void smd_ym2612_fm2_data_write(const uint8_t data)
{
    smd_ym2612_wait();
    *SMD_YM2612_FM2_PORT_DATA = data;
}

/**
 * @brief Writes data to a concrete register index in the FM1 set
 *
 * @param reg Register index to write data to
 * @param data Data to be written
 */
inline void smd_ym2612_fm1_write(const uint8_t reg, const uint8_t data)
{
    smd_ym2612_fm1_addr_write(reg);
    smd_ym2612_fm1_data_write(data);
}

/**
 * @brief Writes data to a concrete register index in the FM2 set
 *
 * @param reg Register index to write data to
 * @param data Data to be written
 */
inline void smd_ym2612_fm2_write(const uint8_t reg, const uint8_t data)
{
    smd_ym2612_fm2_addr_write(reg);
    smd_ym2612_fm2_data_write(data);
}

void smd_ym2612_init(void)
{
    uint16_t i;
    uint8_t reg;

    smd_z80_bus_request();

    /* Disable DAC */
    smd_ym2612_fm1_write(0x2B, 0x00);

    /* Mute all FM Channels */
    reg = 0x40;
    for (i = 0; i < 4; i++)
    {
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
    for (i = 0; i < 3; ++i)
    {
        smd_ym2612_fm1_write(0xB4 + i, 0xC0);
        smd_ym2612_fm2_write(0xB4 + i, 0xC0);
    }

    /* Disable LFO */
    smd_ym2612_fm1_write(0x22, 0x00);

    /* Disable timers A and B and set channel 6 to normal mode */
    smd_ym2612_fm1_write(0x27, 0x00);

    /* All key off */
    smd_ym2612_fm1_addr_write(0x28);
    for (i = 0; i < 3; ++i)
    {
        smd_ym2612_fm1_data_write(0x00 + i);
        smd_ym2612_fm1_data_write(0x04 + i);
    }

    smd_z80_bus_release();
}