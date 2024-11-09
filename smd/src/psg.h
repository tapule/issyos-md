/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            psg.h
 * \brief           Control routines for Sega Megadrive/Genesis PSG
 *
 * The PSG (Programmable Sound Generator) is a sound hardware for simple sounds
 * output. It contains four channels, three of them are tone generators (square
 * wave generators) and the last one is a noise genrator. Each channel has its
 * own volume control.
 *
 * More info:
 * https://www.plutiedev.com/psg
 * https://blog.bigevilcorporation.co.uk/2012/09/03/sega-megadrive-10-sound-part-i-the-psg-chip/
 */

#ifndef SMD_PSG_H
#define SMD_PSG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialize the PSG sound hardware.
 *
 * Controls the initialisation process of the PSG. It reset the channels and
 * put them in silence.
 *
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore.
 */
void smd_psg_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_PSG_H */
