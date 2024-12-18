/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            vdp.c
 * \brief           Control routines for Sega Megadrive/Genesis VDP
 */

#include "vdp.h"
#include "mem_map.h"

/**
 * \brief           Stores if the console is working in PAL mode
 */
static uint8_t smd_vdp_smd_pal_mode_flag;

/* This flag is set when the vertical blank starts */
volatile uint8_t smd_vdp_vblank_flag;

void
smd_vdp_init(void) {
    /*
     * We need to start reading the control port because it cancels whatever it
     * was doing and put it into a well known state.
     * At the same time, we use this read to save the PAL mode.
     */
    /* TODO: SI MULTIPLICO POR 8 (DESPLAZAR 3 A LA IZQ) PODRÍA QUITAR LOS TERNARIOS */
    smd_vdp_smd_pal_mode_flag = *SMD_VDP_CTRL_PORT_U16 & 0x01;

    /* Initialise the VDP register */
    /* H interrupt off, HV counter on */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_1 | 0x04;
    /* Display off, V interrupt on, DMA on, V30 cells mode in pal, V28 ntsc  */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_2 | 0x34 | (smd_vdp_smd_pal_mode_flag ? 8 : 0);
    /* Plane A table address (divided by 0x2000 and lshifted 3 = rshift 10 ) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_PLANEA_ADDR | (SMD_VDP_PLANE_A_ADDR >> 10);
    /* Plane W table address (divided by 0x800 and lshifted 1 = rsifht 10) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_WINDOW_ADDR | (SMD_VDP_PLANE_W_ADDR >> 10);
    /* Plane B table address (divided by 0x2000 = rsifht 13) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_PLANEB_ADDR | (SMD_VDP_PLANE_B_ADDR >> 13);
    /* Sprite table address (divided by 0x200 = rsifht 9) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_SPRITE_ADDR | (SMD_VDP_SPRITE_TABLE_ADDR >> 9);
    /* Background color: palette 0, color 0 */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_BGCOLOR | 0x00;
    /* H interrupt frequency in raster lines (As we disabled it, set maximum) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_HBLANK_RATE | 0xFF;
    /* External interrupt off, V scroll, H scroll */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_3 | SMD_VDP_VSCROLL_MODE | SMD_VDP_HSCROLL_MODE;
    /* H40 cells mode, shadows and highlights off, interlace mode off */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_4 | 0x81;
    /* H Scroll table address (divided by 0x400 = rsifht 10) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_HSCROLL_ADDR | (SMD_VDP_HSCROLL_TABLE_ADDR >> 10);
    /* Auto increment in bytes for the VDP's address reg after read or write */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_AUTOINC | 0x02;
    /* Scroll size (planes A and B size) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_PLANE_SIZE | SMD_VDP_PLANE_SIZE;
    /* Window plane X position (no window) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_WINDOW_XPOS | 0x00;
    /* Window plane Y position (no window) */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_WINDOW_YPOS | 0x00;

    /* Clean the VDP's rams */
    smd_vdp_vram_clear();
    smd_vdp_cram_clear();
    smd_vdp_vsram_clear();
}

inline void
smd_vdp_display_enable(void) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_2 | 0x74 | (smd_vdp_smd_pal_mode_flag ? 8 : 0);
}

inline void
smd_vdp_display_disable(void) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_2 | 0x34 | (smd_vdp_smd_pal_mode_flag ? 8 : 0);
}

void
smd_vdp_vsync_wait(void) {
    /* Set the vblak flag to 0 and wait for the vblank interrupt to change it */
    smd_vdp_vblank_flag = 0;
    while (!smd_vdp_vblank_flag) {
        __asm__ volatile("\tnop\n");
    }
    smd_vdp_vblank_flag = 0;
}

void
smd_vdp_vram_clear(void) {
    *SMD_VDP_CTRL_PORT_U32 = SMD_VDP_VRAM_WRITE_CMD;

    for (uint16_t i = 0; i < (65536 / 4); ++i) {
        *SMD_VDP_DATA_PORT_U32 = 0x00;
    }
}

void
smd_vdp_cram_clear(void) {
    *SMD_VDP_CTRL_PORT_U32 = SMD_VDP_CRAM_WRITE_CMD;

    for (uint16_t i = 0; i < (128 / 4); ++i) {
        *SMD_VDP_DATA_PORT_U32 = 0;
    }
}

void
smd_vdp_vsram_clear(void) {
    *SMD_VDP_CTRL_PORT_U32 = SMD_VDP_VSRAM_WRITE_CMD;

    for (uint16_t i = 0; i < (80 / 4); ++i) {
        *SMD_VDP_DATA_PORT_U32 = 0;
    }
}

inline void
smd_vdp_background_color_set(const uint8_t index) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_BGCOLOR | index;
}

inline void
smd_vdp_scroll_mode_set(const smd_vdp_hscroll_mode_t hscroll_mode, const smd_vdp_vscroll_mode_t vscroll_mode) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_MODESET_3 | vscroll_mode | hscroll_mode;
}

inline void
smd_vdp_plane_size_set(const smd_vdp_plane_size_t size) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_PLANE_SIZE | size;
}

inline void
smd_vdp_autoinc_set(const uint8_t increment) {
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_AUTOINC | increment;
}
