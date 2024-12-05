/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
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
typedef enum smd_pad_id_t /* : uint8_t*/ {
    SMD_PAD_1 = 0,
    SMD_PAD_2 = 1
} smd_pad_id_t;

/**
 * \brief            Gamepad types
 */
typedef enum smd_pad_type_t /* : uint8_t */ {
    SMD_PAD_TYPE_UNKNOWN    = 0x00, /**< Unknown gamepad, not initialized */
    SMD_PAD_TYPE_3BTN       = 0x01, /**< 3-buttons gamepad type */
    SMD_PAD_TYPE_6BTN       = 0x02, /**< 6-buttons gamepad type */
    SMD_PAD_TYPE_UNPLUGGED  = 0x04  /**< Gamepad not connected  */
} smd_pad_type_t;

/**
 * \brief            Gamepad buttons identifiers
 */
enum {
    SMD_PAD_BTN_UP      = (1 << 0),
    SMD_PAD_BTN_DOWN    = (1 << 1),
    SMD_PAD_BTN_LEFT    = (1 << 2),
    SMD_PAD_BTN_RIGHT   = (1 << 3),
    SMD_PAD_BTN_B       = (1 << 4),
    SMD_PAD_BTN_C       = (1 << 5),
    SMD_PAD_BTN_A       = (1 << 6),
    SMD_PAD_BTN_START   = (1 << 7),
    SMD_PAD_BTN_Z       = (1 << 8),
    SMD_PAD_BTN_Y       = (1 << 9),
    SMD_PAD_BTN_X       = (1 << 10),
    SMD_PAD_BTN_MODE    = (1 << 11)
};

/**
 * \brief           Initialize the gamepad ports
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore unless you want to reset the
 *                  devices.
 */
void smd_pad_init(void);

/**
 * \brief           Update the gamepads state
 * \note            This function must be called on each frame to control user's
 *                  actions
 */
void smd_pad_update(void);

/**
 * \brief           Get the type for the specified gamepad id
 * \param[in]       pad_id: Gamepad id to query
 * \return          Type of the queried gamepad id
 */
smd_pad_type_t smd_pad_type(const smd_pad_id_t pad_id);

/**
 * \brief           Check if the specified gamepad id is plugged
 * \param[in]       pad_id: Gamepad id to query
 * \return          true if the queried gamepad id is plugged, false otherwise
 */
bool smd_pad_is_plugged(const smd_pad_id_t pad_id);

/**
 * \brief           Check if a set of buttons on a gamepad are currently pressed
 * \param[in]       pad_id: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true if the set of buttons are currently pressed, false otherwise
 */
bool smd_pad_btn_state(const smd_pad_id_t pad_id, const uint16_t buttons);

/**
 * \brief           Check if a set of buttons on a gamepad were just pressed in
 *                  the current frame.
 * \param[in]       pad_id: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true if the set of buttons were currently pressed, false otherwise
 */
bool smd_pad_btn_pressed(const smd_pad_id_t pad_id, const uint16_t buttons);

/**
 * \brief           Check if a set of buttons on a gamepad were just released in
 *                  the current frame.
 * \param[in]       pad_id: Gamepad id to query
 * \param[in]       buttons: Set of buttons to query
 * \return          true if the set of buttons were currently released, false otherwise
 */
bool smd_pad_btn_released(const smd_pad_id_t pad_id, const uint16_t buttons);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PAD_H */
