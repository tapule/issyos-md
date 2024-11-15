/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            vdp.h
 * \brief           Control routines for Sega Megadrive/Genesis VDP
 *
 * The VDP (Video Display Processor) is the main graphics processor, wich has
 * two scrollables graphics planes (A and B) and a static plane (W). It is also
 * capable of rendering up to 80 sprites on screen. Its main internal memory is
 * a 64KB used to store graphics and data. It has also a 128B memory for colors
 * called CRAM and a vertical scroll ram or VSRAM where the vertical scroll data
 * is stored.
 *
 * More info:
 * https://www.plutiedev.com/vdp-setup
 * https://www.plutiedev.com/vdp-registers
 */

#ifndef SMD_VDP_H
#define SMD_VDP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           VDP registers
 *
 * The VDP has 24 registers (some of them not used) which control how video
 * hardware behaves. These registers are set by writing a control word to the
 * VDP control port where the high byte is as follow:
 *              | 1| 0| 0|R4|R3|R2|R1|R0|
 * R4-R0 is the register to write, so 0x81 (10000001) will write to the register
 * 0x01 wich is the Mode register 2.
 * Registers 0x06, 0x08, 0x09, 0x0E are not used and are always 0x00
 */
#define SMD_VDP_REG_MODESET_1       (0x8000) /* Mode register #1 */
#define SMD_VDP_REG_MODESET_2       (0x8100) /* Mode register #2 */
#define SMD_VDP_REG_PLANEA_ADDR     (0x8200) /* Plane A data table address */
#define SMD_VDP_REG_WINDOW_ADDR     (0x8300) /* Window data table address */
#define SMD_VDP_REG_PLANEB_ADDR     (0x8400) /* Plane B data table address */
#define SMD_VDP_REG_SPRITE_ADDR     (0x8500) /* Sprite data table address */
#define SMD_VDP_REG_BGCOLOR         (0x8700) /* Background color */
#define SMD_VDP_REG_HBLANK_RATE     (0x8A00) /* HBlank interrupt rate */
#define SMD_VDP_REG_MODESET_3       (0x8B00) /* Mode register #3 */
#define SMD_VDP_REG_MODESET_4       (0x8C00) /* Mode register #4 */
#define SMD_VDP_REG_HSCROLL_ADDR    (0x8D00) /* HScroll data table address */
#define SMD_VDP_REG_AUTOINC         (0x8F00) /* Autoincrement data */
#define SMD_VDP_REG_PLANE_SIZE      (0x9000) /* Plane A and B size */
#define SMD_VDP_REG_WINDOW_XPOS     (0x9100) /* Window X position */
#define SMD_VDP_REG_WINDOW_YPOS     (0x9200) /* Window Y position */
#define SMD_VDP_REG_DMALEN_L        (0x9300) /* DMA length (low) */
#define SMD_VDP_REG_DMALEN_H        (0x9400) /* DMA length (high) */
#define SMD_VDP_REG_DMASRC_L        (0x9500) /* DMA source (low) */
#define SMD_VDP_REG_DMASRC_M        (0x9600) /* DMA source (mid) */
#define SMD_VDP_REG_DMASRC_H        (0x9700) /* DMA source (high) */
/**
 * \brief           Extended KMod VDP registers.
 *
 * Gens KMod adds 3 extended virtual register to the VDP. These registers are
 * really useful on emulators for debugging purpouses.
 */
#define SMD_VDP_REG_KMOD_CONTROL    (0x9D00) /* Emulator control functions */
#define SMD_VDP_REG_KMOD_MESSAGE    (0x9E00) /* Emulator message output  */
#define SMD_VDP_REG_KMOD_TIMER      (0x9F00) /* Emulator timer functions */

/**
 * \brief           Base commands for the control port to do writes to the different VDP's rams
 */
#define SMD_VDP_VRAM_WRITE_CMD      (0x40000000)
#define SMD_VDP_CRAM_WRITE_CMD      (0xC0000000)
#define SMD_VDP_VSRAM_WRITE_CMD     (0x40000010)

/**
 * \brief           Base commands for the control port to do DMA writes to different VDP's rams
 */
#define SMD_VDP_DMA_VRAM_WRITE_CMD  (0x40000080)
#define SMD_VDP_DMA_CRAM_WRITE_CMD  (0xC0000080)
#define SMD_VDP_DMA_VSRAM_WRITE_CMD (0x40000090)

/*
 * Default VDP memory layout
 *  #0000..#BFFF - 1536 tiles
 *  #C000..#CFFF - Plane A (64x32, 4096 Bytes)
 *  #D000..#D000 - Plane W (0x0)
 *  #D000..#DFFF - 128 tiles
 *  #E000..#EFFF - Plane B (64x32, 4096 Bytes)
 *  #F000..#F7FF - 64 Tiles
 *  #F800..#FBBF - HScroll table (960 Bytes)
 *  #FBC0..#FBFF - 2 tiles
 *  #FC00..#FE7F - Sprite table (640 Bytes)
 *  #FE80..#FFFF - 12 tiles
 *  -> 1742 free tiles
 */
/**
 * \brief           Default planes start address in VRAM
 */
#ifndef SMD_VDP_PLANE_A_ADDR
#define SMD_VDP_PLANE_A_ADDR (0xC000)
#endif
#ifndef SMD_VDP_PLANE_B_ADDR
#define SMD_VDP_PLANE_B_ADDR (0xE000)
#endif
#ifndef SMD_VDP_PLANE_W_ADDR
#define SMD_VDP_PLANE_W_ADDR (0xD000)
#endif
#define SMD_PLANE_A SMD_VDP_PLANE_A_ADDR
#define SMD_PLANE_B SMD_VDP_PLANE_B_ADDR
#define SMD_PLANE_W SMD_VDP_PLANE_W_ADDR

/**
 * \brief           Default sprite and horizontal scroll tables address in VRAM
 */
#ifndef SMD_VDP_SPRITE_TABLE_ADDR
#define SMD_VDP_SPRITE_TABLE_ADDR   (0xFC00)
#endif
#ifndef SMD_VDP_HSCROLL_TABLE_ADDR
#define SMD_VDP_HSCROLL_TABLE_ADDR  (0xF800)
#endif

/**
 * \brief           Default plane sizes in tiles
 */
#ifndef SMD_VDP_PLANE_SIZE
#define SMD_VDP_PLANE_SIZE SMD_VDP_PLANE_SIZE_64X32
#endif
#define SMD_VDP_PLANE_WIDTH  (((SMD_VDP_PLANE_SIZE & 0x03) << 5) + 32)
#define SMD_VDP_PLANE_HEIGTH (((SMD_VDP_PLANE_SIZE & 0x30) << 1) + 32)
#define SMD_VDP_PLANE_TILES  (SMD_VDP_PLANE_WIDTH * SMD_VDP_PLANE_HEIGTH)

/**
 * \brief           Default horizontal and vertical planes scroll mode
 */
#ifndef SMD_VDP_HSCROLL_MODE
#define SMD_VDP_HSCROLL_MODE SMD_VDP_HSCROLL_TILE
#endif
#ifndef SMD_VDP_VSCROLL_MODE
#define SMD_VDP_VSCROLL_MODE SMD_VDP_VSCROLL_PLANE
#endif

/**
 * \brief           Plane scroll modes on Mode register #3
 *                  (vscr | hscr1 | hscr0)
 */
typedef enum smd_vdp_hscroll_mode_t {
    SMD_VDP_HSCROLL_PLANE   = 0x00,
    SMD_VDP_HSCROLL_TILE    = 0x02,
    SMD_VDP_HSCROLL_LINE    = 0x03
} smd_vdp_hscroll_mode_t;

typedef enum smd_vdp_vscroll_mode_t {
    SMD_VDP_VSCROLL_PLANE   = 0x00,
    SMD_VDP_VSCROLL_TILE    = 0x04 /* Vertically it scrolls by 2 tiles, not 1 */
} smd_vdp_vscroll_mode_t;

/**
 * \brief           Plane sizes on Plane sizes register
 *                  (vsz1 | vsz2 | 0 | 0 | hsz1 | hsz2)
 */
typedef enum smd_vdp_plane_size_t {
    SMD_VDP_PLANE_SIZE_32X32    = 0x00,
    SMD_VDP_PLANE_SIZE_32X64    = 0x10,
    SMD_VDP_PLANE_SIZE_32X128   = 0x30,
    SMD_VDP_PLANE_SIZE_64X32    = 0x01,
    SMD_VDP_PLANE_SIZE_64X64    = 0x11,
    SMD_VDP_PLANE_SIZE_128X32   = 0x03
} smd_vdp_plane_size_t;

/*
 * CHECKME: QUIZÁ PODRÍAMOS TENER UN struct smd_context.vblank_flag o similar
 * CHECKME: No habría otra manera de gestionar este flag en lugar de una global?
 */
/**
 * \brief           This flag is set when the vertical blank starts
 */
extern volatile uint8_t smd_vdp_vblank_flag;

/**
 * \brief           Initialises the VDP
 *
 * Before using the VDP, we must setup it first. This function controls the
 * initialisation, registers setup and VRAM/CRAM/VSRAM cleaning process.
 *
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore unless you want to reset the
 *                  devices.
 */
void smd_vdp_init(void);

/**
 * \brief           Turn on the display
 */
void smd_vdp_display_enable(void);

/**
 * \brief           Turn off the display
 */
void smd_vdp_display_disable(void);

/**
 * \brief           Waits until the next vertical blank starts
 * \note            Be aware that this will loop forever if interrupts are disabled
 */
void smd_vdp_vsync_wait(void);

/**
 * \brief           Clear internal VDP video ram
 */
void smd_vdp_vram_clear(void);

/**
 * \brief           Clear internal VDP color ram
 */
void smd_vdp_cram_clear(void);

/**
 * \brief           Clear internal VDP vertical scroll ram
 */
void smd_vdp_vsram_clear(void);

/**
 * \brief           Set the color index (0 - 63) to use as background color
 */
void smd_vdp_background_color_set(const uint8_t index);

/**
 * \brief           Set plane scroll mode for planes A and B
 * \param[in]       hscroll_mode: New horizontal scroll mode
 * \param[in]       vscroll_mode: New vertical scroll mode
 */
void smd_vdp_scroll_mode_set(const smd_vdp_hscroll_mode_t hscroll_mode, const smd_vdp_vscroll_mode_t vscroll_mode);

/**
 * \brief           Set plane size for planes A and B
 * \param           size: New plane size
 */
void smd_vdp_plane_size_set(const smd_vdp_plane_size_t size);

/**
 * \brief           Set the number of bytes to add automatically after read/write operations
 *
 * The VDP has a register where we can set an autoincrement value (in bytes) to
 * add after each ram accesses.
 *
 * \param           increment: Number of bytes to add
 */
void smd_vdp_autoinc_set(const uint8_t increment);

/*
    * TODO:
    smd_vdp_hilightshadow_set

*/

#ifdef __cplusplus
}
#endif

#endif /* SMD_VDP_H */
