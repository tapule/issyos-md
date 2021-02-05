/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: video.h
 * Control routines for Sega Megadrive/Genesis VDP
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
 * 
 */

#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>
#include <stdbool.h>

/* Plane scroll modes (vscr | hscr1 | hscr0) */
typedef enum vid_hscroll_mode
{
    VID_HSCROLL_PLANE = 0x00,
    VID_HSCROLL_TILE  = 0x02,
    VID_HSCROLL_LINE  = 0x03
} vid_hscroll_mode_t;

typedef enum vid_vscroll_mode
{
    VID_VSCROLL_PLANE = 0x00,
    VID_VSCROLL_TILE  = 0x04    /* Vertically it scrolls by 2 tiles, not 1 */
} vid_vscroll_mode_t;

/* Plane sizes (vsz1 | vsz2 | 0 | 0 | hsz1 | hsz2) */
typedef enum vid_plane_size
{
    VID_PLANE_SIZE_32X32  = 0x00,
    VID_PLANE_SIZE_32X64  = 0x10,
    VID_PLANE_SIZE_32X128 = 0x30,
    VID_PLANE_SIZE_64X32  = 0x01,
    VID_PLANE_SIZE_64X64  = 0x11,    
    VID_PLANE_SIZE_128X32 = 0x03    
} vid_plane_size_t;


/**
 * @brief initialises the VDP
 * 
 * Before using the VDP, we must setup it first. This function controls the
 * initialisation, registers setup and VRAM/CRAM/VSRAM cleaning process. 
 * 
 * @note This function is called from the boot process so maybe you don't need
 * to call it anymore unless you want to reset the devices.
 */
void vid_init(void);

/**
 * @brief Turns on the display
 * 
 */
void vid_display_enable(void);

/**
 * @brief Turns off the display
 * 
 */
void vid_display_disable(void);

/**
 * @brief Waits until the next vertical blank starts
 * 
 * @note Be aware that this will loop forever if interrupts are disabled
 */
void vid_vsync_wait(void);

/**
 * @brief Sets the background color
 * 
 * Set the color index (0 - 63) wich will be used as background color
 */
void vid_background_color_set(uint8_t index);

/**
 * @brief Sets the plane scroll mode for planes A and B
 * 
 * @param hscr New horizontal scroll mode
 * @param vscr New vertical scroll mode
 */
void vid_scroll_mode_set(vid_hscroll_mode_t hscr, vid_vscroll_mode_t vscr);

/**
 * @brief Set the plane size for planes A and B
 * 
 * @param size New plane size
 */
void vid_plane_size_set(vid_plane_size_t size);

/*
    vid_background_color_set
        Establece el color de fondo
    vid_vram_clear
        Esto debe borrar toda la vram lo cual incluye: 0tile, planos, sprite
        table, hscroll, etc)
    vid_vsram_clear
        Borra la vsram
    vid_ cram_clear
        Borra la cram, aunque puede ser mejor establecer unas paletas por
        defecto??



*/

// QUIZÁ ESTAS DEBERÍAN IR EN MD (SYS)
// bool vid_is_pal(void);

// bool vid_is_ntsc(void);

#endif // VIDEO_H