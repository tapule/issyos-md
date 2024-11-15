/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            ports.h
 * \brief           Sega Megadrive/Genesis memory map ports definition
 *
 *
 * More info:
 *
 */

#ifndef SMD_PORTS_H
#define SMD_PORTS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief
 *      Define start of ROM region
 */
#define SMD_ROM_ADDR                (0x00000000)

/**
 * \brief           Z80 memory pointer and size (8KB)
 */
#define SMD_Z80_RAM_ADDRESS         ((uint8_t *) 0xA00000)
#define SMD_Z80_RAM_SIZE            (8 * 1024)

/**
 * \brief           YM2612 control ports
 *
 * The internal registers of the FM YM2612 are divided in two sets:
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

/**
 * \brief           SMD Version port
 *
 * The version read only port gives information about the Sega Megadrive/Genesis
 * hardware and version:
 *              |MOD|VMOD|DISK|RSV|VER3|VER2|VER1|VER0|
 *  MOD:     0 = Domestic (Japanese) model.
 *           1 = Overseas (US/European) model.
 *  VMOD:    0 = NTSC mode (7.67MHz CPU clock)
 *           1 = PAL mode (7.60MHz CPU clock)
 *  DISK:    0 = Expansion unit connected (MegaCD).
 *           1 = Expansion unit not connected
 *  RSV:     Reserved. Not used
 *  VER3..0: Megadrive/Genesis version
 */
#define SMD_VERSION_PORT            ((volatile uint8_t *) 0xA10001)

/**
 * \brief           Gamepads data ports (DATAx)
 */
#define SMD_PAD_1_DATA_PORT         ((volatile uint8_t *) 0xA10003)
#define SMD_PAD_2_DATA_PORT         ((volatile uint8_t *) 0xA10005)
#define SMD_PAD_EXP_DATA_PORT       ((volatile uint8_t *) 0xA10007)

/**
 * \brief           Gamepads control ports (CTRLx)
 */
#define SMD_PAD_1_CTRL_PORT         ((volatile uint8_t *) 0xA10009)
#define SMD_PAD_2_CTRL_PORT         ((volatile uint8_t *) 0xA1000B)
#define SMD_PAD_EXP_CTRL_PORT       ((volatile uint8_t *) 0xA1000D)

/**
 * \brief           Z80 control ports
 */
#define SMD_Z80_BUS_PORT            ((volatile uint16_t *) 0xA11100)
#define SMD_Z80_RESET_PORT          ((volatile uint16_t *) 0xA11200)

#define SMD_TMSS_PORT               ((volatile uint32_t *) 0xA14000)

/**
 * \brief           VDP ports
 *
 * There are 3 ports to talk with the VDP. These ports can be accessed as 16 or
 * 32 bits.
 * To work with the VDP we need to write commands to the control port and if we
 * want to write or read vram, cram or vsram we must write the address to/from
 * the data port.
 * The control port is also the VDP's status register, so it can be read to get
 * this information:
 *              | *| *| *| *| *| *| FE| FF|
 *              |VI|SO|SC|OD|VB|HB|DMA|PAL|
 *  FE: 1 = FIFO is empty.
 *  FF: 1 = FIFO is full.
 *  VI: 1 = Vertical interrupt occurred.
 *  SO: 1 = Sprite limit has been hit on current scanline
 *          17+ in 256 pixel wide mode
 *          21+ in 320 pixel wide mode.
 *  SC: 1 = Collision happened between non-zero pixels in two sprites. Used for
 *          pixel-accurate collision detection.
 *  OD: 1 = Odd frame displayed in interlaced mode
 *  VB: 1 = Vertical blank in progress.
 *  HB: 1 = Horizontal blank in progress.
 *  DMA: 1 = DMA in progress.
 *  PAL: 1 = PAL system
 *       0 = NTSC system.
 */
#define SMD_VDP_PORT_DATA_W         ((volatile uint16_t *) 0xC00000)
#define SMD_VDP_PORT_DATA_L         ((volatile uint32_t *) 0xC00000)
#define SMD_VDP_PORT_CTRL_W         ((volatile uint16_t *) 0xC00004)
#define SMD_VDP_PORT_CTRL_L         ((volatile uint32_t *) 0xC00004)
#define SMD_VDP_PORT_HV_COUNTER     ((volatile uint16_t *) 0xC00008)

/**
 * \brief           PSG port from the m68k side
 */
#define SMD_PSG_PORT                ((volatile uint8_t *) 0xC00011)

/**
 *  \brief
 *      Define start of RAM region
 */
#define SMD_RAM_ADDR                (0x00FF0000)

#ifdef __cplusplus
}
#endif

#endif /* SMD_PORTS_H */