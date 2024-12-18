/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            mem_map.h
 * \brief           Sega Megadrive/Genesis memory map I/O definitions
 *
 * The m68k interacts with the VDP, Pads, z80, ROM cartridge, etc. through the
 * system bus via specific memory locations. Each device has its own address or
 * range of addresses where the m68k must write/read to communicate with the
 * particular device. This is known as memory mapped I/O.
 *
 * More info:
 * https://rasterscroll.com/mdgraphics/mega-drive-architecture/
 */

#ifndef SMD_MEMORY_MAP_H
#define SMD_MEMORY_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \brief          Start of ROM region and size (depends on the cartridge)
 */
#define SMD_ROM_ADDRESS             (0x00000000)
#ifndef SMD_ROM_SIZE
    #define SMD_ROM_SIZE            (4 * 1024 * 1024)
#endif

/**
 * \brief           Start of Z80 memory region and size (8KB)
 */
#define SMD_Z80_RAM_ADDRESS         (0xA00000)
#define SMD_Z80_RAM_SIZE            (8 * 1024)

/**
 * \brief           YM2612 memory mapped control ports
 *
 * The internal registers of the FM YM2612 are divided in two sets:
 *  FM1: LFO, Timers, Key On/Off, DAC, FM Channels 1..3
 *  FM2: FM Channels 4..6
 * Each set has its own register address and data ports which has to be written
 * in byte size.
 */
#define SMD_YM2612_FM1_ADDRESS_PORT ((volatile uint8_t *)0xA04000)
#define SMD_YM2612_FM1_DATA_PORT    ((volatile uint8_t *)0xA04001)
#define SMD_YM2612_FM2_ADDRESS_PORT ((volatile uint8_t *)0xA04002)
#define SMD_YM2612_FM2_DATA_PORT    ((volatile uint8_t *)0xA04003)

/**
 * \brief           SMD version port
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
 * \brief           Gamepads memory mapped data ports (DATAx)
 */
#define SMD_PAD_1_DATA_PORT         ((volatile uint8_t *) 0xA10003)
#define SMD_PAD_2_DATA_PORT         ((volatile uint8_t *) 0xA10005)
#define SMD_PAD_EXP_DATA_PORT       ((volatile uint8_t *) 0xA10007)

/**
 * \brief           Gamepads memory mapped control ports (CTRLx)
 */
#define SMD_PAD_1_CTRL_PORT         ((volatile uint8_t *) 0xA10009)
#define SMD_PAD_2_CTRL_PORT         ((volatile uint8_t *) 0xA1000B)
#define SMD_PAD_EXP_CTRL_PORT       ((volatile uint8_t *) 0xA1000D)

/**
 * \brief           Z80 memory mapped control ports
 */
#define SMD_Z80_BUS_PORT            ((volatile uint16_t *) 0xA11100)
#define SMD_Z80_RESET_PORT          ((volatile uint16_t *) 0xA11200)

/**
 * \brief           TMSS (Trademark Security System) memory mapped port
 *
 * TMSS is not available in early models. In models 1+ this must store 'SEGA'
 */
#define SMD_TMSS_PORT               ((volatile uint32_t *) 0xA14000)

/**
 * \brief           VDP memory mapped ports
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
#define SMD_VDP_DATA_PORT_U16       ((volatile uint16_t *) 0xC00000)
#define SMD_VDP_DATA_PORT_U32       ((volatile uint32_t *) 0xC00000)
#define SMD_VDP_CTRL_PORT_U16       ((volatile uint16_t *) 0xC00004)
#define SMD_VDP_CTRL_PORT_U32       ((volatile uint32_t *) 0xC00004)
#define SMD_VDP_HV_COUNTER_PORT     ((volatile uint16_t *) 0xC00008)

/**
 * \brief           PSG port from the m68k side
 */
#define SMD_PSG_PORT                ((volatile uint8_t *) 0xC00011)

/**
 *  \brief          start of RAM region and size
 */
#define SMD_RAM_ADDRESS             (0x00FF0000)
#define SMD_RAM_SIZE                (64 * 1024)

#ifdef __cplusplus
}
#endif

#endif /* SMD_MEMORY_MAP_H */
