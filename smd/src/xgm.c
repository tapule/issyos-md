/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            xgm.c
 * \brief           Sound and music system
 */

#include <stdint.h>
#include "null_data.h"
#include "sys.h"
#include "xgm.h"
#include "xgm_drv.h"
#include "z80.h"

/**
 * \brief           XGM samples table
 *
 * XGM Sound driver has a sample table in the z80 RAM used to store sample
 * addresses and sizes. The first entry of this table (id 0) is used for the
 * silent sample (smd_null_data can be used as silent sample).
 */
#define SMD_XGM_SAMPLE_ID_TABLE     ((volatile uint8_t *) 0xA01C00)

/**
 * \brief           XGM status variable address in z80 RAM
 *
 * XGM status variable:
 *      |b7|b6|b5|b4|b3|b2|b1|b0|
 *  b0: PCM0 playing
 *  b1: PCM1 playing
 *  b2: PCM2 playing
 *  b3: PCM3 playing
 *  b4: ??
 *  b5: ??
 *  b6: XGM playing
 *  b7: Driver ready
 */
#define SMD_XGM_STATUS_ADDR         ((volatile uint8_t *) 0xA00102)
#define SMD_XGM_STATUS_PLAYING      (1)
#define SMD_XGM_STATUS_READY        (128)

/**
 * \brief           XGM commands variable address in z80 RAM
 *
 * XGM commands variable:
 *      |b7|b6|b5|b4|b3|b2|b1|b0|
 *  b0: Play PCM0 command
 *  b1: Play PCM1 command
 *  b2: Play PCM2 command
 *  b3: Play PCM3 command
 *  b4: Pause/Stop XGM command
 *  b5: Resume Play XGM command
 *  b6: Play XGM command
 *  b7: Unused??
 */
#define SMD_XGM_COMMAND_ADDR        ((volatile uint8_t *) 0xA00100)
#define SMD_XGM_COMMAND_CLEAR       (0x0F)
#define SMD_XGM_COMMAND_PAUSE       (0x10)
#define SMD_XGM_COMMAND_RESUME      (0x20)
#define SMD_XGM_COMMAND_PLAY        (0x40)

/**
 * \brief           XGM parameters data address in z80 RAM
 */
#define SMD_XGM_PARAMS_ADDR         ((volatile uint8_t *) 0xA00104)

/**
 * \brief           "Dummy" music sequence to reset/turn-off the YM2612 sounds
 *
 * This sequence contains commands to reset/turn-off the all YM2612 sounds. It
 * must be used to silent and put the XGM driver in a healthy state.
 *
 * \note            Converted from stop_xgm binary resource in SGDK
 */
#define SMD_XGM_RESET_SEQUENCE_SIZE (172)
const uint8_t xgm_reset_sequence[SMD_XGM_RESET_SEQUENCE_SIZE] = {
    0xAC, 0x5E, 0x40, 0x7F, 0x44, 0x7F, 0x48, 0x7F, 0x4C, 0x7F, 0x41, 0x7F, 0x45, 0x7F, 0x49, 0x7F, 0x4D, 0x7F,
    0x42, 0x7F, 0x46, 0x7F, 0x4A, 0x7F, 0x4E, 0x7F, 0x80, 0xFF, 0x84, 0xFF, 0x88, 0xFF, 0x8C, 0xFF, 0x4E, 0x81,
    0xFF, 0x85, 0xFF, 0x89, 0xFF, 0x8D, 0xFF, 0x82, 0xFF, 0x86, 0xFF, 0x8A, 0xFF, 0x8E, 0xFF, 0x7E, 0x40, 0x7F,
    0x44, 0x7F, 0x48, 0x7F, 0x4C, 0x7F, 0x41, 0x7F, 0x45, 0x7F, 0x49, 0x7F, 0x4D, 0x7F, 0x42, 0x7F, 0x46, 0x7F,
    0x4A, 0x7F, 0x4E, 0x7F, 0x80, 0xFF, 0x84, 0xFF, 0x88, 0xFF, 0x8C, 0xFF, 0x6E, 0x81, 0xFF, 0x85, 0xFF, 0x89,
    0xFF, 0x8D, 0xFF, 0x82, 0xFF, 0x86, 0xFF, 0x8A, 0xFF, 0x8E, 0xFF, 0x8A, 0x00, 0x01, 0x02, 0x04, 0x05, 0x06,
    0xDE, 0x44, 0xFF, 0x45, 0xFF, 0x46, 0xFF, 0x47, 0xFF, 0x48, 0xFF, 0x49, 0xFF, 0x4A, 0xFF, 0x4B, 0xFF, 0x4C,
    0xFF, 0x4D, 0xFF, 0x4E, 0xFF, 0x4F, 0xFF, 0x50, 0xFF, 0x51, 0xFF, 0x52, 0xFF, 0x53, 0xFF, 0xCE, 0x54, 0xFF,
    0x55, 0xFF, 0x56, 0xFF, 0x57, 0xFF, 0x58, 0xFF, 0x59, 0xFF, 0x5A, 0xFF, 0x5B, 0xFF, 0x36, 0x9F, 0xBF, 0xDF,
    0xFF, 0x2C, 0x8F, 0x00, 0xA5, 0x01, 0xCC, 0x1F, 0xE0, 0xFE};

/* XGM variables for sound synchronisation */
/**
 * \brief           Default reference music tempo in ticks per second: 60
 */
static uint16_t xgm_tempo;

/**
 * \brief           System music tempo in ticks per second: 60 in NTSC, 50 in PAL
 */
static uint16_t xgm_tempo_def;

/**
 * \brief           Counter for music synchronization
 */
static int16_t xgm_tempo_cnt;

/**
 * \brief           Next channel that should be used for sfx
 */
static uint16_t smd_xgm_sfx_next_channel;

/**
 * \brief           Is sfx muted?
 */
static bool smd_xgm_sfx_muted;

void
smd_xgm_init(void) {
    uint16_t xgm_ready;

    smd_z80_bus_request();

    /* Loads the XGM driver into the z80 memmory space */
    smd_z80_data_load(xgm_drv, 0, XGM_DRV_SIZE);

    /*
     * Set the silent sample address in the sample table. We use the smd_null_data
     * array here as it is 256 bytes aligned as required for the XGM samples
     */
    smd_xgm_sfx_set(0, smd_null_data, NULL_DATA_SIZE);

    smd_z80_reset();
    xgm_ready = *SMD_XGM_STATUS_ADDR & SMD_XGM_STATUS_READY;
    smd_z80_bus_release();

    /*
     * The XGM does some kind of initialization so we need to wait for it to
     * be ready.
     */
    do {
        while (!smd_z80_is_bus_free()) {}
        /* Request the bus here to read the xgm status */
        smd_z80_bus_request();
        xgm_ready = *SMD_XGM_STATUS_ADDR & SMD_XGM_STATUS_READY;
        smd_z80_bus_release();
    } while (!xgm_ready);

    /* Skip channel 0 for sfx as it is normally used for music */
    smd_xgm_sfx_next_channel = 1;
    smd_xgm_sfx_muted = false;
    /* Initialize XGM synchronisation variables */
    xgm_tempo = 60;
    xgm_tempo_def = smd_is_pal() ? 50 : 60;
    xgm_tempo_cnt = 0;
}

inline void
smd_xgm_update(void) {
    int16_t cnt = xgm_tempo_cnt;
    uint16_t step = xgm_tempo_def;
    uint16_t num = 0;

    /*
     * Calculates number of pending frames depending on whether system is in PAL
     * or NTSC mode. In NTSC each frame notifies one tick to the z80 but in PAL
     * we must notify 60 ticks per second so every 5 frames notifies one more
     * tick (6 ticks every 5 fames = 60 ticks every 50 frames or one second)
     */
    while (cnt <= 0) {
        ++num;
        cnt += step;
    }
    xgm_tempo_cnt = cnt - xgm_tempo;

    while (true) {
        smd_z80_bus_request();
        /*
         * XGM MODIFYING_F (0x0E) variable controls whether the z80 is accessing
         * the PENDING_FRM (0x0F) variable or not. We check here if this is
         * happening and wait for the z80 to finish doing it.
         */
        if (!SMD_XGM_PARAMS_ADDR[0x0E]) {
            break;
        }
        smd_z80_bus_release();

        /* Wait a bit (about 80 cycles) */
        __asm__ volatile("\t\tmovm.l %d0-%d3,-(%sp)\n");
        __asm__ volatile("\t\tmovm.l (%sp)+,%d0-%d3\n");
    }
    /*
     * XGM PENDING_FRM (0x0F) variable contains number of XGM frames to process.
     * Increment it here.
     */
    SMD_XGM_PARAMS_ADDR[0x0F] += num;
}

void
smd_xgm_sfx_set(const uint8_t id, const uint8_t *restrict sample, const uint32_t size) {
    /* Get the sample id index in the sample table (4 bytes per sample) */
    volatile uint8_t *smp_table_ptr = (SMD_XGM_SAMPLE_ID_TABLE + (uint32_t) (id << 2));

    /* Set the sample address in the sample table index */
    smp_table_ptr[0] = ((uint32_t) sample) >> 8;
    smp_table_ptr[1] = ((uint32_t) sample) >> 16;
    /* Set the sample size in the sample table */
    smp_table_ptr[2] = size >> 8;
    smp_table_ptr[3] = size >> 16;
}

void
smd_xgm_sfx_play(const uint8_t id, const uint8_t priority, const uint16_t channel) {
    volatile uint8_t *pcm_params;

    if (!smd_xgm_sfx_muted) {
        smd_ints_disable();
        smd_z80_bus_request();

        pcm_params = SMD_XGM_PARAMS_ADDR + 0x04 + (channel * 2);

        /*
         * Set PCM priority and sample id to play:
         * SMD_XGM_PARAMS_ADDR + 0x04 = PCM0 priority
         * SMD_XGM_PARAMS_ADDR + 0x05 = PCM0 sample id
         * SMD_XGM_PARAMS_ADDR + 0x06 = PCM1 priority
         * SMD_XGM_PARAMS_ADDR + 0x07 = PCM1 sample id
         * SMD_XGM_PARAMS_ADDR + 0x08 = PCM2 priority
         * SMD_XGM_PARAMS_ADDR + 0x09 = PCM2 sample id
         * SMD_XGM_PARAMS_ADDR + 0x0A = PCM3 priority
         * SMD_XGM_PARAMS_ADDR + 0x0B = PCM3 sample id
         */
        pcm_params[0] = priority & 0x0F;
        pcm_params[1] = id;

        /*
         * Set XGM driver play PCM channel command
         * SMD_XGM_COMMAND_PLAY_PCM0 = 0x01
         * SMD_XGM_COMMAND_PLAY_PCM1 = 0x02
         * SMD_XGM_COMMAND_PLAY_PCM2 = 0x04
         * SMD_XGM_COMMAND_PLAY_PCM3 = 0x08
         */
        *SMD_XGM_COMMAND_ADDR |= (1 << channel);

        smd_z80_bus_release();
        smd_ints_enable();

        /* Adjust play auto next channel skipping channel 0 */
        smd_xgm_sfx_next_channel = channel + 1;
        if (smd_xgm_sfx_next_channel > 3) {
            smd_xgm_sfx_next_channel = 1;
        }
    }
}

inline void
smd_xgm_sfx_play_auto(const uint8_t id, const uint8_t priority) {
    smd_xgm_sfx_play(id, priority, smd_xgm_sfx_next_channel);
}

inline void
smd_xgm_sfx_stop(const uint16_t channel) {
    /* Stop a channel is the same as playing silent sample on it with maximum priority */
    smd_xgm_sfx_play(0, 15, channel);
}

inline void
smd_xgm_sfx_mute(void) {
    if (!smd_xgm_sfx_muted) {
        /* Stop all sample channels first */
        for (int16_t i = 0; i < 4; ++i) {
            smd_xgm_sfx_stop(i);
        }
        smd_xgm_sfx_muted = true;
    }
}

inline void
smd_xgm_sfx_unmute(void) {
    smd_xgm_sfx_muted = false;
}

inline bool
smd_xgm_sfx_is_muted(void) {
    return smd_xgm_sfx_muted;
}

void
smd_xgm_music_play(const uint8_t *restrict song) {
    uint8_t ids[0x100 - 4];
    uint32_t addr;

    /* Prepare sample id table (0..63) */
    for (uint16_t i = 0; i < 63; ++i) {
        /* Sample address in sample bank data */
        addr = ((uint32_t) song[(i * 4) + 0]) << 8;
        addr |= ((uint32_t) song[(i * 4) + 1]) << 16;

        /* Silent sample ? use null data as sample address */
        if (addr == 0xFFFF00) {
            addr = (uint32_t) smd_null_data;
        }
        /* Adjust sample address (make it absolute) */
        else {
            addr += ((uint32_t) song) + 0x100;
        }

        /* Write adjusted addr */
        ids[(i * 4) + 0] = addr >> 8;
        ids[(i * 4) + 1] = addr >> 16;
        /* and recopy len */
        ids[(i * 4) + 2] = song[(i * 4) + 2];
        ids[(i * 4) + 3] = song[(i * 4) + 3];
    }

    smd_ints_disable();
    smd_z80_bus_request();

    /* Upload sample id table (first entry is silent sample, we don't transfer it) */
    smd_z80_data_load(ids, 0x1C00 + 4, 0x100 - 4);

    /* Get song address and bypass sample id table */
    addr = ((uint32_t) song) + 0x100;
    /* bypass sample data (use the sample data size) */
    addr += ((uint32_t) song[0xFC]) << 8;
    addr += ((uint32_t) song[0xFD]) << 16;
    /* and bypass the music data size field */
    addr += 4;

    /* Set XGM music data address */
    SMD_XGM_PARAMS_ADDR[0] = addr >> 0;
    SMD_XGM_PARAMS_ADDR[1] = addr >> 8;
    SMD_XGM_PARAMS_ADDR[2] = addr >> 16;
    SMD_XGM_PARAMS_ADDR[3] = addr >> 24;

    /* Clear previous commands */
    *SMD_XGM_COMMAND_ADDR &= SMD_XGM_COMMAND_CLEAR;
    /* Set XGM driver play XGM command */
    *SMD_XGM_COMMAND_ADDR |= SMD_XGM_COMMAND_PLAY;
    /* Clear pending frame */
    SMD_XGM_PARAMS_ADDR[0x0F] = 0;

    smd_z80_bus_release();
    smd_ints_enable();
}

void
smd_xgm_music_pause(void) {
    smd_ints_disable();
    smd_z80_bus_request();

    /* Clear previous commands */
    *SMD_XGM_COMMAND_ADDR &= SMD_XGM_COMMAND_CLEAR;
    /* Set XGM driver pause XGM command */
    *SMD_XGM_COMMAND_ADDR |= SMD_XGM_COMMAND_PAUSE;
    /* Clear pending frame */
    SMD_XGM_PARAMS_ADDR[0x0F] = 0;

    smd_z80_bus_release();
    smd_ints_enable();
}

void
smd_xgm_music_resume(void) {
    smd_ints_disable();
    smd_z80_bus_request();

    /* Check if we are already playing a song */
    if ((*SMD_XGM_COMMAND_ADDR & SMD_XGM_COMMAND_PLAY) == 0) {
        /* Clear previous commands */
        *SMD_XGM_COMMAND_ADDR &= SMD_XGM_COMMAND_CLEAR;
        /* Set XGM driver resume XGM command */
        *SMD_XGM_COMMAND_ADDR |= SMD_XGM_COMMAND_RESUME;
        /* Clear pending frame */
        SMD_XGM_PARAMS_ADDR[0x0F] = 0;
    }
    smd_z80_bus_release();
    smd_ints_enable();
}

void
smd_xgm_music_stop(void) {
    uint32_t addr;

    /*
     * To stop a song and put the XGM driver in a healthy state, it needs a
     * special sequence to be played
     */
    addr = (uint32_t) xgm_reset_sequence;

    smd_ints_disable();
    smd_z80_bus_request();

    /* Set stop sequence as XGM music data address */
    SMD_XGM_PARAMS_ADDR[0] = addr >> 0;
    SMD_XGM_PARAMS_ADDR[1] = addr >> 8;
    SMD_XGM_PARAMS_ADDR[2] = addr >> 16;
    SMD_XGM_PARAMS_ADDR[3] = addr >> 24;

    /* Clear previous commands */
    *SMD_XGM_COMMAND_ADDR &= SMD_XGM_COMMAND_CLEAR;
    /* Set XGM driver play XGM command */
    *SMD_XGM_COMMAND_ADDR |= SMD_XGM_COMMAND_PLAY;
    /* Clear pending frame */
    SMD_XGM_PARAMS_ADDR[0x0F] = 0;

    smd_z80_bus_release();
    smd_ints_enable();
}
