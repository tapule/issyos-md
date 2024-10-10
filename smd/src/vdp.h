/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: vdp.h
 * Constants and definitions for Sega Megadrive/Genesis VDP
 *
 * Diferent VDP's constants and definitions shared across various modules
 *
 */

#ifndef SMD_VDP_H
#define SMD_VDP_H

#include <stdint.h>
/*
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
#define SMD_VDP_PORT_DATA_W     ((volatile uint16_t *) 0xC00000)
#define SMD_VDP_PORT_DATA_L     ((volatile uint32_t *) 0xC00000)
#define SMD_VDP_PORT_CTRL_W     ((volatile uint16_t *) 0xC00004)
#define SMD_VDP_PORT_CTRL_L     ((volatile uint32_t *) 0xC00004)
#define SMD_VDP_PORT_HV_COUNTER ((volatile uint16_t *) 0xC00008)

/*
 * The VDP has 24 registers (some of them not used) which control how video
 * hardware behaves. These registers are set by writing a control word to the
 * VDP control port where the high byte is as follow:
 *              | 1| 0| 0|R4|R3|R2|R1|R0|
 * R4-R0 is the register to write, so 0x81 (10000001) will write to the register
 * 0x01 wich is the Mode register 2.
 * Registers 0x06, 0x08, 0x09, 0x0E are not used and are always 0x00
 */
#define SMD_VDP_REG_MODESET_1       0x8000      /* Mode register #1 */
#define SMD_VDP_REG_MODESET_2       0x8100      /* Mode register #2 */
#define SMD_VDP_REG_PLANEA_ADDR     0x8200      /* Plane A data table address */
#define SMD_VDP_REG_WINDOW_ADDR     0x8300      /* Window data table address */
#define SMD_VDP_REG_PLANEB_ADDR     0x8400      /* Plane B data table address */
#define SMD_VDP_REG_SPRITE_ADDR     0x8500      /* Sprite data table address */
#define SMD_VDP_REG_BGCOLOR         0x8700      /* Background color */
#define SMD_VDP_REG_HBLANK_RATE     0x8A00      /* HBlank interrupt rate */
#define SMD_VDP_REG_MODESET_3       0x8B00      /* Mode register #3 */
#define SMD_VDP_REG_MODESET_4       0x8C00      /* Mode register #4 */
#define SMD_VDP_REG_HSCROLL_ADDR    0x8D00      /* HScroll data table address */
#define SMD_VDP_REG_AUTOINC         0x8F00      /* Autoincrement data */
#define SMD_VDP_REG_PLANE_SIZE      0x9000      /* Plane A and B size */
#define SMD_VDP_REG_WINDOW_XPOS     0x9100      /* Window X position */
#define SMD_VDP_REG_WINDOW_YPOS     0x9200      /* Window Y position */
#define SMD_VDP_REG_DMALEN_L        0x9300      /* DMA length (low) */
#define SMD_VDP_REG_DMALEN_H        0x9400      /* DMA length (high) */
#define SMD_VDP_REG_DMASRC_L        0x9500      /* DMA source (low) */
#define SMD_VDP_REG_DMASRC_M        0x9600      /* DMA source (mid) */
#define SMD_VDP_REG_DMASRC_H        0x9700      /* DMA source (high) */
/*
 * Extended KMod VDP registers.
 * Gens KMod adds 3 extended virtual register to the VDP. These registers are
 * really useful on emulators for debugging purpouses.
 */
#define SMD_VDP_REG_KMOD_CONTROL    0x9D00      /* Emulator control functions */
#define SMD_VDP_REG_KMOD_MESSAGE    0x9E00      /* Emulator message output  */
#define SMD_VDP_REG_KMOD_TIMER      0x9F00      /* Emulator timer functions */

/*
 * Base commands for the control port to do writes to the different VDP's rams
 */
#define SMD_VDP_VRAM_WRITE_CMD      0x40000000
#define SMD_VDP_CRAM_WRITE_CMD      0xC0000000
#define SMD_VDP_VSRAM_WRITE_CMD     0x40000010

/*
 * Base commands for the control port to do DMA writes to different VDP's rams
 */
#define SMD_VDP_DMA_VRAM_WRITE_CMD      0x40000080
#define SMD_VDP_DMA_CRAM_WRITE_CMD      0xC0000080
#define SMD_VDP_DMA_VSRAM_WRITE_CMD     0x40000090

/*
 * Video configuration default values
 *
 *  VDP memory layout:
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
/* Planes start address in VRAM */
#ifndef SMD_VDP_PLANE_A_ADDR
    #define SMD_VDP_PLANE_A_ADDR 0xC000
#endif
#ifndef SMD_VDP_PLANE_B_ADDR
    #define SMD_VDP_PLANE_B_ADDR 0xE000
#endif
#ifndef SMD_VDP_PLANE_W_ADDR
    #define SMD_VDP_PLANE_W_ADDR 0xD000
#endif
#define SMD_PLANE_A SMD_VDP_PLANE_A_ADDR
#define SMD_PLANE_B SMD_VDP_PLANE_B_ADDR
#define SMD_PLANE_W SMD_VDP_PLANE_W_ADDR

/* Sprite and horizontal scroll tables address in VRAM */
#ifndef SMD_VDP_SPRITE_TABLE_ADDR
    #define SMD_VDP_SPRITE_TABLE_ADDR 0xFC00
#endif
#ifndef SMD_VDP_HSCROLL_TABLE_ADDR
    #define SMD_VDP_HSCROLL_TABLE_ADDR 0xF800
#endif

/* Plane sizes in tiles */
#ifndef SMD_VDP_PLANE_SIZE
    #define SMD_VDP_PLANE_SIZE SMD_VDP_PLANE_SIZE_64X32
#endif
#define SMD_VDP_PLANE_WIDTH (((SMD_VDP_PLANE_SIZE & 0x03) << 5) + 32)
#define SMD_VDP_PLANE_HEIGTH (((SMD_VDP_PLANE_SIZE & 0x30) << 1) + 32)
#define SMD_VDP_PLANE_TILES (SMD_VDP_PLANE_WIDTH * SMD_VDP_PLANE_HEIGTH)

/* Horizontal and vertical default plane scroll modes */
#ifndef SMD_VDP_HSCROLL_MODE
    #define SMD_VDP_HSCROLL_MODE SMD_VDP_HSCROLL_TILE
#endif
#ifndef SMD_VDP_VSCROLL_MODE
    #define SMD_VDP_VSCROLL_MODE SMD_VDP_VSCROLL_PLANE
#endif

/* Plane scroll modes on Mode register #3 (vscr | hscr1 | hscr0) */
typedef enum
{
    SMD_VDP_HSCROLL_PLANE = 0x00,
    SMD_VDP_HSCROLL_TILE  = 0x02,
    SMD_VDP_HSCROLL_LINE  = 0x03
} smd_vdp_hscroll_mode_t;

typedef enum
{
    SMD_VDP_VSCROLL_PLANE = 0x00,
    SMD_VDP_VSCROLL_TILE  = 0x04    /* Vertically it scrolls by 2 tiles, not 1 */
} smd_vdp_vscroll_mode_t;

/* Plane sizes on Plane sizes register (vsz1 | vsz2 | 0 | 0 | hsz1 | hsz2) */
typedef enum
{
    SMD_VDP_PLANE_SIZE_32X32  = 0x00,
    SMD_VDP_PLANE_SIZE_32X64  = 0x10,
    SMD_VDP_PLANE_SIZE_32X128 = 0x30,
    SMD_VDP_PLANE_SIZE_64X32  = 0x01,
    SMD_VDP_PLANE_SIZE_64X64  = 0x11,
    SMD_VDP_PLANE_SIZE_128X32 = 0x03
} smd_vdp_plane_size_t;

// CHECKME: QUIZÁ PODRÍAMOS TENER UN struct smd_context.vblank_flag o similar
extern volatile uint8_t smd_vdp_vblank_flag;

/**
 * @brief Initialises the VDP
 *
 * Before using the VDP, we must setup it first. This function controls the
 * initialisation, registers setup and VRAM/CRAM/VSRAM cleaning process.
 *
 * @note This function is called from the boot process so maybe you don't need
 * to call it anymore unless you want to reset the devices.
 */
void smd_vdp_init(void);

/**
 * @brief Turns on the display
 *
 */
void smd_vdp_display_enable(void);

/**
 * @brief Turns off the display
 *
 */
void smd_vdp_display_disable(void);

/**
 * @brief Waits until the next vertical blank starts
 *
 * @note Be aware that this will loop forever if interrupts are disabled
 */
void smd_vdp_vsync_wait(void);

/**
 * @brief Clears the internal VDP video ram
 *
 */
void smd_vdp_vram_clear(void);

/**
 * @brief Clears the internal VDP color ram
 *
 */
void smd_vdp_cram_clear(void);

/**
 * @brief Clears the internal VDP vertical scroll ram
 *
 */
void smd_vdp_vsram_clear(void);

/**
 * @brief Sets the background color
 *
 * Set the color index (0 - 63) wich will be used as background color
 */
void smd_vdp_background_color_set(const uint8_t index);

/**
 * @brief Sets the plane scroll mode for planes A and B
 *
 * @param hscroll_mode New horizontal scroll mode
 * @param vscroll_mode New vertical scroll mode
 */
void smd_vdp_scroll_mode_set(const smd_vdp_hscroll_mode_t hscroll_mode,
                         const smd_vdp_vscroll_mode_t vscroll_mode);




/**
 * @brief Set the plane size for planes A and B
 *
 * @param size New plane size
 */
void smd_vdp_plane_size_set(const smd_vdp_plane_size_t size);

/**
 * @brief Sets the automatic number of bytes to add after read/write operations
 *
 * The VDP has a register where we can set an autoincrement value to add after
 * ram accesses.
 *
 * @param increment Number of bytes to add
 */
void smd_vdp_autoinc_set(const uint8_t increment);


/*
    * TODO:
    smd_vdp_hilightshadow_set

*/




#endif /* SMD_VDP_H */
