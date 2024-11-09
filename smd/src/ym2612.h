/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            ym2612.h
 * \brief           Control routines for Sega Megadrive/Genesis Yamaha YM2612 FM
 *                  sound chip
 *
 * The Yamaha YM2612 FM sound chip is a sound synthesizer integrated circuit
 * that can generate six simultaneous tones, each with their own configuration
 * of FM operators. Its capabilities include:
 *    6 channels of FM sound
 *    An 8-bit Digitized Audio channel (as replacement for one of FM channels)
 *    Stereo output capability
 *    One LFO (low frequency oscillator) to distort the FM sounds
 *    2 timers, for use by software
 *
 * More info:
 * https://www.smspower.org/maxim/Documents/YM2612
 * https://www.chibiakumas.com/68000/platform3.php
 * https://www.plutiedev.com/ym2612-operations
 * https://www.plutiedev.com/ym2612-registers
 */

#ifndef SMD_YM2612_H
#define SMD_YM2612_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialises the YM2612 sound hardware.
 *
 * Controls the initialisation process of the YM2612.
 *
 * CHECKME: Esta anotación tiene sentido en esta versión?
 * \note            This function is called from the boot process so maybe you
 * don't need to call it anymore.
 */
void smd_ym2612_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_YM2612_H */
