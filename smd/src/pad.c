/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            pad.c
 * \brief           Control routines for Sega Megadrive/Genesis gamepads
 */

#include "pad.h"
#include "ports.h"
#include "z80.h"

/**
 * \brief           Gamepads types
 */
static uint8_t smd_pad_types[SMD_PAD_COUNT];

/**
 * \brief           Current and previous frame gamepads states
 *
 * A 0 bit means not pressed, a 1 bit means pressed:
 *      | 0| 0| 0| 0| Md| X| Y| Z|St| A| C| B| R| L| D| U|
 */
static uint16_t smd_pad_state[SMD_PAD_COUNT];
static uint16_t smd_pad_state_old[SMD_PAD_COUNT];

void
smd_pad_init(void) {
    /* Reset types and states */
    for (uint16_t i = 0; i < SMD_PAD_COUNT; ++i) {
        smd_pad_types[i] = SMD_PAD_TYPE_UNKNOWN;
        smd_pad_state[i] = 0;
        smd_pad_state_old[i] = 0;
    }

    /*
     * To setup the controllers, we have to write 0x40 to each port. This will
     * use the TH pin of each control port as output an the remaining pins as
     * input.
     *                  7  6  5  4  3  2  1  0
     * Data/Ctrl pins: |?|TH|TL|TR| R| L| D| U|
     * Data: 0 means a button has been pressed, 1 if it has been released
     * CTrl: 0 means a pin on data port is an output, 1 if it is an input
     * The pin 7 is not connected so we can ignore it.
     *
     * It is also important to halt the z80 while we are touching these ports to
     * prevent it to access the m68k's bus due to a hardware bug.
     *
     */
    smd_z80_bus_request_fast();
    *SMD_PAD_1_DATA_PORT = 0x40;
    *SMD_PAD_1_CTRL_PORT = 0x40;
    *SMD_PAD_2_DATA_PORT = 0x40;
    *SMD_PAD_2_CTRL_PORT = 0x40;
    *SMD_PAD_EXP_DATA_PORT = 0x40;
    *SMD_PAD_EXP_CTRL_PORT = 0x40;
    smd_z80_bus_release();
}

void
smd_pad_update(void) {

    /* CHECKME: This function only updates PAD1 and PAD2, because MD has only 2 pads */
    smd_pad_state_old[SMD_PAD_1] = smd_pad_state[SMD_PAD_1];
    smd_pad_state_old[SMD_PAD_2] = smd_pad_state[SMD_PAD_2];

    smd_z80_bus_request_fast();
    /* 1st step read:
     * | ?| ?| C| B| R| L| D| U|
     */
    *SMD_PAD_1_DATA_PORT = 0x40;
    *SMD_PAD_2_DATA_PORT = 0x40;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    smd_pad_state[SMD_PAD_1] = *SMD_PAD_1_DATA_PORT & 0x3F;
    smd_pad_state[SMD_PAD_2] = *SMD_PAD_2_DATA_PORT & 0x3F;
    /* 2nd step read:
     * | ?| ?|St| A| 0| 0| D| U|
     */
    *SMD_PAD_1_DATA_PORT = 0x00;
    *SMD_PAD_2_DATA_PORT = 0x00;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    /* Rearrange into |St| A| C| B| R| L| D| U| */
    smd_pad_state[SMD_PAD_1] |= ((*SMD_PAD_1_DATA_PORT & 0x30) << 2);
    smd_pad_state[SMD_PAD_2] |= ((*SMD_PAD_2_DATA_PORT & 0x30) << 2);
    /* Steps 3rd, 4th and 5th:
     * Ignore results
     */
    *SMD_PAD_1_DATA_PORT = 0x40;
    *SMD_PAD_2_DATA_PORT = 0x40;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    *SMD_PAD_1_DATA_PORT = 0x00;
    *SMD_PAD_2_DATA_PORT = 0x00;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    *SMD_PAD_1_DATA_PORT = 0x40;
    *SMD_PAD_2_DATA_PORT = 0x40;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    /* 6th step read:
     * | ?| ?|St| A| 0| 0| 0| 0|
     * If bits 3-0 are 0 it's a 6-button gamepad, otherwhise it's a 3-button.
     */
    *SMD_PAD_1_DATA_PORT = 0x00;
    *SMD_PAD_2_DATA_PORT = 0x00;
    __asm__ volatile("\tnop\n");
    __asm__ volatile("\tnop\n");
    if ((*SMD_PAD_1_DATA_PORT & 0x0F) != 0x00) {
        smd_pad_types[SMD_PAD_1] = SMD_PAD_TYPE_3BTN;
    } else {
        smd_pad_types[SMD_PAD_1] = SMD_PAD_TYPE_6BTN;
        /* 7th step read:
         * | ?| ?| C| B| Md| X| Y| Z|
        */
        *SMD_PAD_1_DATA_PORT = 0x40;
        __asm__ volatile("\tnop\n");
        __asm__ volatile("\tnop\n");
        smd_pad_state[SMD_PAD_1] |= ((*SMD_PAD_1_DATA_PORT & 0x0F) << 8);
    }
    if ((*SMD_PAD_2_DATA_PORT & 0x0F) != 0x00) {
        smd_pad_types[SMD_PAD_2] = SMD_PAD_TYPE_3BTN;
    } else {
        smd_pad_types[SMD_PAD_2] = SMD_PAD_TYPE_6BTN;
        /* 7th step read:
         * | ?| ?| C| B| Md| X| Y| Z|
        */
        *SMD_PAD_2_DATA_PORT = 0x40;
        __asm__ volatile("\tnop\n");
        __asm__ volatile("\tnop\n");
        smd_pad_state[SMD_PAD_2] |= ((*SMD_PAD_2_DATA_PORT & 0x0F) << 8);
    }
    smd_z80_bus_release();

    /* Invert the state bits to use 0 as not pressed, 1 as pressed */
    smd_pad_state[SMD_PAD_1] ^= 0x0FFF;
    smd_pad_state[SMD_PAD_2] ^= 0x0FFF;
}

inline uint8_t
smd_pad_type(const uint16_t pad) {
    return smd_pad_types[pad];
}

inline bool
smd_pad_btn_state(const uint16_t pad, const uint16_t buttons) {
    /* TODO: Change these if's for some assert system or use an enum instead for pad */
    if (pad > SMD_PAD_COUNT) {
        return false;
    }
    return (smd_pad_state[pad] & buttons);
}

inline bool
smd_pad_btn_pressed(const uint16_t pad, const uint16_t buttons) {
    if (pad > SMD_PAD_COUNT) {
        return false;
    }
    /* Pressed this frame and not pressed last frame */
    return ((smd_pad_state[pad] & buttons) && !(smd_pad_state_old[pad] & buttons));
}

inline bool
smd_pad_btn_released(const uint16_t pad, const uint16_t buttons) {
    if (pad > SMD_PAD_COUNT) {
        return false;
    }
    /* Not pressed this frame and pressed last frame */
    return (!(smd_pad_state[pad] & buttons) && (smd_pad_state_old[pad] & buttons));
}
