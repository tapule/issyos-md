/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: video.c
 * Control routines for Sega Megadrive/Genesis VDP
 */

#include "video.h"

/*
 * There are 3 ports to talk with the VDP. These ports can be accessed as 16 or
 * 32 bits.
 */
static volatile uint16_t *const vdp_port_data = (uint16_t*) 0xC00000;
static volatile uint16_t *const vdp_port_ctrl_w = (uint16_t*) 0xC00004;
static volatile uint32_t *const vdp_port_ctrl_l = (uint32_t*) 0xC00004;
static volatile uint16_t *const vdp_port_hv_counter = (uint16_t*) 0xC00008;

/*
 * The VDP has 24 registers (some of them not used) which control how video
 * hardware behaves. These registers are set by writing a control word to the
 * VDP control port where the high byte is as follow:
 *              | 1| 0| 0|R4|R3|R2|R1|R0|
 * R4-R0 is the register to write, so 0x81 (10000001) will write to the register
 * 0x01 wich is the Mode register 2.
 */
#define VDP_REG_MODESET_1       0x8000      /* Mode register #1 */
#define VDP_REG_MODESET_2       0x8100      /* Mode register #2 */
#define VDP_REG_PLANEA_ADDR     0x8200      /* Plane A data table address */
#define VDP_REG_WINDOW_ADDR     0x8300      /* Window data table address */
#define VDP_REG_PLANEB_ADDR     0x8400      /* Plane B data table address */
#define VDP_REG_SPRITE_ADDR     0x8500      /* Sprite data table address */
#define VDP_REG_BGCOLOUR        0x8700      /* Background color */
#define VDP_REG_HBLANK_RATE     0x8A00      /* HBlank interrupt rate */
#define VDP_REG_MODESET_3       0x8B00      /* Mode register #3 */
#define VDP_REG_MODESET_4       0x8C00      /* Mode register #4 */
#define VDP_REG_HSCROLL_ADDR    0x8D00      /* HScroll data table address */
#define VDP_REG_AUTOINC         0x8F00      /* Autoincrement data */
#define VDP_REG_PLANE_SIZE      0x9000      /* Plane A and B size */
#define VDP_REG_WINDOW_XPOS     0x9100      /* Window X position */
#define VDP_REG_WINDOW_YPOS     0x9200      /* Window Y position */
#define VDP_REG_DMALEN_L        0x9300      /* DMA length (low) */
#define VDP_REG_DMALEN_H        0x9400      /* DMA length (high) */
#define VDP_REG_DMASRC_L        0x9500      /* DMA source (low) */
#define VDP_REG_DMASRC_M        0x9600      /* DMA source (mid) */
#define VDP_REG_DMASRC_H        0x9700      /* DMA source (high) */

/* Stores if the console is working in PAL mode */
static uint8_t vid_pal_mode;

/* This flag is set when the vertical blank starts */
volatile uint8_t vid_vblank_flag = 1;

void vid_init(void)
{
    /*
     * We need to start reading the control port because it cancels whatever it
     * was doing and put it into a well known state.
     * At the same time, we use this read to save the PAL mode.
     */
    vid_pal_mode = *vdp_port_ctrl_w & 0x01;

    /* Initialise the VDP register */
    /* H interrupt off, HV counter on */
    *vdp_port_ctrl_w = VDP_REG_MODESET_1 | 0x04;
    /* Display off, V interrupt on, DMA on, V30 cells mode in pal, V28 ntsc  */
    *vdp_port_ctrl_w = VDP_REG_MODESET_2 | 0x34 | (vid_pal_mode ? 8 : 0);
    /* Plane A table address at 0xC000 (divided by 0x2000 and lshifted 3) */
    *vdp_port_ctrl_w = VDP_REG_PLANEA_ADDR | 0x30;
    /* Plane W table address at 0xD000 (divided by 0x800 and lshifted 1) */
    *vdp_port_ctrl_w = VDP_REG_WINDOW_ADDR | 0x34;
    /* Plane B table address at 0xE000 (divided by 0x2000) */
    *vdp_port_ctrl_w = VDP_REG_PLANEA_ADDR | 0x07;
    /* Sprite table address at 0xFC00 (divided by 0x200) */
    *vdp_port_ctrl_w = VDP_REG_SPRITE_ADDR | 0x7E;
    /* Background color: palette 0, colour 0 */
    *vdp_port_ctrl_w = VDP_REG_BGCOLOUR | 0x00;
    /* H interrupt frequency in raster lines (As we disabled it, set maximum) */
    *vdp_port_ctrl_w = VDP_REG_HBLANK_RATE | 0xFF;
    /* External interrupt off, V scroll full, H scroll full */
    *vdp_port_ctrl_w = VDP_REG_MODESET_3 | 0x00;
    /* H40 cells mode, shadows and highlights off, interlace mode off */
    *vdp_port_ctrl_w = VDP_REG_MODESET_4 | 0x81;
    /* H Scroll table address at 0xF800 (divided by 0x400) */
    *vdp_port_ctrl_w = VDP_REG_HSCROLL_ADDR | 0x3E;
    /* Auto increment in bytes for the VDP's address reg after read or write */
    *vdp_port_ctrl_w = VDP_REG_AUTOINC | 0x02;
    /* Scroll size 64x32 cells (planes A and B size) */
    *vdp_port_ctrl_w = VDP_REG_PLANE_SIZE | 0x01;
    /* Window plane X position (no window) */
    *vdp_port_ctrl_w = VDP_REG_WINDOW_XPOS | 0x00;
    /* Window plane Y position (no window) */
    *vdp_port_ctrl_w = VDP_REG_WINDOW_YPOS | 0x00;
}

void vid_display_enable(void)
{
    *vdp_port_ctrl_w = VDP_REG_MODESET_2 | 0x74 | (vid_pal_mode ? 8 : 0);
}

void vid_display_disable(void)
{
    *vdp_port_ctrl_w = VDP_REG_MODESET_2 | 0x34 | (vid_pal_mode ? 8 : 0);
}

void vid_vsync_wait(void)
{
    /* Set the vblak flag to 0 and wait for the vblank interrupt to change it */
    vid_vblank_flag = 0;
    while (!vid_vblank_flag)
    {
        __asm__ volatile ("\tnop\n");
    }
    vid_vblank_flag = 0;
}