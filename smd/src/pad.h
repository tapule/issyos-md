/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            pad.h
 * \brief           Control routines for Sega Megadrive/Genesis gamepads
 *
 * The Sega Megadrive/Genesis has three I/O ports: CTRL1, CTRL2 and EXP. The
 * first two are dedicated to controllers while EXP is for modem support.
 * The generic controllers are the 3-button gamepads which include D-Pad, A, B,
 * C and Start buttons. But there are also newer 6-button gamepads which add
 * four more buttons X, Y, Z and Mode.
 *
 * More info:
 * https://www.plutiedev.com/io-ports
 * https://www.plutiedev.com/controllers
 * https://www.chibiakumas.com/68000/platform2.php#LessonP11
 */

#ifndef SMD_PAD_H
#define SMD_PAD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Gamepad identifiers
 */
#define SMD_PAD_1            (0x0000)
#define SMD_PAD_2            (0x0001)
#define SMD_PAD_COUNT        (0x0002)

/**
 * \brief            Gamepad types
 */
/* CHECKME: The gamepad type should be a enum type */
#define SMD_PAD_TYPE_3BTN    (0x00) /**< 3-buttons gamepad type */
#define SMD_PAD_TYPE_6BTN    (0x01) /**< 6-buttons gamepad type */
#define SMD_PAD_TYPE_UNKNOWN (0x0F) /**< Unknown or not connected gamepad */

/**
 * \brief            Gamepad buttons identifiers
 */
#define SMD_PAD_BTN_UP       (0x0001)
#define SMD_PAD_BTN_DOWN     (0x0002)
#define SMD_PAD_BTN_LEFT     (0x0004)
#define SMD_PAD_BTN_RIGHT    (0x0008)
#define SMD_PAD_BTN_B        (0x0010)
#define SMD_PAD_BTN_C        (0x0020)
#define SMD_PAD_BTN_A        (0x0040)
#define SMD_PAD_BTN_START    (0x0080)
#define SMD_PAD_BTN_Z        (0x0100)
#define SMD_PAD_BTN_Y        (0x0200)
#define SMD_PAD_BTN_X        (0x0400)
#define SMD_PAD_BTN_MODE     (0x0800)

/**
 * \brief           Initialize the gamepad ports
 *
 * Before using the gamepads, we must setup them first. This function controls
 * the detection and initialisation process of gamepads.
 *
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore unless you want to reset the
 *                  devices.
 */
void smd_pad_init(void);

/**
 * \brief           Update the gamepads state
 *
 * Each frame we must update the internal state of gamepads' buttons to control
 * the user's actions
 *
 * \note            This function must be called on each frame
 */
void smd_pad_update(void);

/**
 * \brief           Get the type for the specified gamepad id
 * \param[in]       pad: Gamepad id to query
 * \return          Type of queried gamepad id
 */
/* CHECKME: The gamepad type should be a enum type */
uint8_t smd_pad_type(const uint16_t pad);

/**
 * \brief           Check if a set of buttons on a gamepad are currently pressed
 * \param[in]       pad: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true if the set of buttons are currently pressed, false otherwise
 */
bool smd_pad_btn_state(const uint16_t pad, const uint16_t buttons);

/**
 * \brief           Check if a set of buttons on a gamepad were just pressed in
 *                  the current frame.
 * \param[in]       pad: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true If the set of buttons were currently pressed, false otherwise
 */
bool smd_pad_btn_pressed(const uint16_t pad, const uint16_t buttons);

/**
 * \brief           Check if a set of buttons on a gamepad were just released in
 *                  the current frame.
 * \param[in]       pad: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true If the set of buttons were currently released, false otherwise
 */
bool smd_pad_btn_released(const uint16_t pad, const uint16_t buttons);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PAD_H */
