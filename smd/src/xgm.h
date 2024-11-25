/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            xgm.h
 * \brief           Sound and music system
 *
 * This system provides an interface to play sfx and music in the Sega
 * Megadrive/Genesis. It uses the XGM (eXtended Genesis Music) sound driver by
 * Stephane Dallongeville developed for SGDK.
 * The XGM driver takes VGM (or XGM) file as input to play music. It supports 4
 * PCM channels at a fixed 14 Khz and allows to play sfx through PCM with 16
 * level of priority. The driver is designed to avoid DMA contention when
 * possible (depending CPU load).
 *
 * More info:
 * https://github.com/Stephane-D/SGDK
 */

#ifndef SMD_XGM_H
#define SMD_XGM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Initialise the sound system using the XGM driver
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore.
 */
void smd_xgm_init(void);

/**
 * \brief           Handles the sound timing notifying the z80 in each frame.
 * \note            This function is called automatically in the vint so you
 *                  don't need to call it.
 */
void smd_xgm_update(void);

/**
 * \brief           Adds a PCM sample to the XGM sample table
 *
 * The XGM sample table can hold up to 255 samples. Sample id 0 is not allowed
 * and normally samples id < 64 are used by music, but you can use them if you
 * know what are you doing. The norm for SFX is using samples id [64..255].
 * By design the XGM driver needs its samples aligned to 256 bytes boundary with
 * a size multiple of 256.
 *
 * \param[in]       id: XGM sample table id to define
 * \param[in]       sample: Sample address (must be aligned to 256 bytes boundary)
 * \param[in]       size: Sample size in bytes (must be multiple of 256 bytes)
 * \note            For optimization purposes, this function does not request
 *                  the z80 bus so be aware that you must request it in your code:
 *                     smd_z80_bus_request();
 *                       smd_xgm_sfx_set(64, my_sfx_1, MY_SFX_1_SIZE);
 *                       smd_xgm_sfx_set(65, my_sfx_2, MY_SFX_2_SIZE);
 *                     smd_z80_bus_release();
 */
void smd_xgm_sfx_set(const uint8_t id, const uint8_t *restrict sample, const uint32_t size);

/**
 * \brief           Start playing a PCM sample on a specific channel
 *
 * Plays a sample id previously defined with smd_xgm_sfx_set in the selected
 * channel. There are 4 channels available [0...3], but be aware that the first
 * one is usually used by music. The norm for SFX is using channels [1...3].
 * Priority is used to decide whether a sample which is playing in the channel
 * should be replaced with this one. If priority is higher than priority in the
 * current sample, it will be replaced with the new one.
 *
 * \param[in]       id: Sample id in the XGM sample table to play
 * \param[in]       priority: Playing priority ranging from 0 (lowest) to 15 highest
 * \param[in]       channel: Desired channel to play the sample
 */
void smd_xgm_sfx_play(const uint8_t id, const uint8_t priority, const uint16_t channel);

/**
 * \brief           Start playing a PCM sample autoselecting the channel
 *
 * Acts like smd_xgm_sfx_play but selects the channel to use automatically.
 * Priority is used to decide whether a sample which is playing in the channel
 * should be replaced with this one. If priority is higher than priority in the
 * current sample, it will be replaced with the new one.
 *
 * \param[in]       id: Sample id in the XGM sample table to play
 * \param[in]       priority: Playing priority ranging from 0 (lowest) to 15
 */
void smd_xgm_sfx_play_auto(const uint8_t id, const uint8_t priority);

/**
 * \brief           Stop playing the PCM sample on a specific channel
 * \param[in]       channel: Desired channel to stop
 */
void smd_xgm_sfx_stop(const uint16_t channel);

/**
 * \brief           Mute PCM sounds
 */
void smd_xgm_sfx_mute(void);

/**
 * \brief           Unmute PCM sounds
 */
void smd_xgm_sfx_unmute(void);

/**
 * \brief           Checks whether PCM sounds are muted
 * \return          true if PCM sounds are muted, false otherwise
 */
bool smd_xgm_sfx_is_muted(void);

/**
 * \brief           Start playing a XGM music track
 *
 * Load and start playing the specified XGM track. XGM files must be in its
 * compiled form XGC. The utility xgmtool can be used to convert directly from
 * VGM files to XGC.
 *
 * \param[in]       song: XGM track address
 */
void smd_xgm_music_play(const uint8_t *restrict song);

/**
 * \brief           Pause music
 */
void smd_xgm_music_pause(void);

/**
 * \brief           Resume music
 */
void smd_xgm_music_resume(void);

/**
 * \brief           Stop music
 */
void smd_xgm_music_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_XGM_H */
