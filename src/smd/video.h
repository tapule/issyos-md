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

// QUIZÁ ESTAS DEBERÍAN IR EN MD (SYS)
// bool vid_is_pal(void);

// bool vid_is_ntsc(void);

#endif // VIDEO_H
