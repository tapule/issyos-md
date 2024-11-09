/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            pal.c
 * \brief           Palette routines
 */

#include "pal.h"
#include "dma.h"
#include "vdp.h"

/**
 * \brief           Internal palette color buffers and its pointers
 */
static uint16_t smd_pal_buffers[2][64];
static uint16_t *smd_pal_primary;
static uint16_t *smd_pal_alternate;

/**
 * \brief           Should us update CRAM with the primary buffer?
 */
static bool smd_pal_update_needed;

/**
 * \brief           Is there a fade operation running?
 */
static bool smd_pal_fading;

/**
 * \brief           Fade operation speed in frames
 */
static uint8_t smd_pal_fade_speed;

/**
 * \brief            Fade operation frame counter
 */
static uint8_t smd_pal_fade_counter;

void
smd_pal_init(void) {
    smd_pal_primary = &smd_pal_buffers[0][0];
    smd_pal_alternate = &smd_pal_buffers[1][0];
    smd_pal_update_needed = false;
    smd_pal_fading = false;
    smd_pal_fade_speed = 0;
    smd_pal_fade_counter = 0;
}

void
smd_pal_primary_set(const uint16_t index, uint16_t count, const uint16_t *restrict colors) {
    /* We update the primary buffer, so we need to update CRAM */
    smd_pal_update_needed = true;

    while (count) {
        /* Adjust the offset to sum using 0..n style */
        --count;
        smd_pal_primary[index + count] = colors[count];
    }
}

void
smd_pal_alternate_set(const uint16_t index, uint16_t count, const uint16_t *restrict colors) {
    while (count) {
        /* Adjust the offset to sum using 0..n style */
        --count;
        smd_pal_alternate[index + count] = colors[count];
    }
}

inline void
smd_pal_cram_set(const uint16_t index, const uint16_t count, const uint16_t *restrict colors) {
    /* We can't use a fast transfer here as we don't know the source address */
    /* CHECKME: Should I synchonize the internal primary buffer here?? */
    smd_dma_cram_transfer(colors, index << 1, count, 2);
}

inline void
smd_pal_swap(void) {
    uint16_t *tmp;

    tmp = smd_pal_primary;
    smd_pal_primary = smd_pal_alternate;
    smd_pal_alternate = tmp;

    smd_pal_update_needed = true;
}

inline void
smd_pal_fade(const uint16_t speed) {
    /* Fade operation setup */
    smd_pal_fade_speed = speed;
    smd_pal_fade_counter = 0;
    smd_pal_fading = true;
}

bool
smd_pal_fade_step(void) {
    uint16_t i = 64;
    uint16_t primary_component;
    uint16_t alternate_component;

    if (!smd_pal_fading) {
        return false;
    }

    ++smd_pal_fade_counter;
    if (smd_pal_fade_counter == smd_pal_fade_speed) {
        smd_pal_fade_counter = 0;
        smd_pal_update_needed = false;
        while (i) {
            /* Adjust the index to use 0..63 instead of 1..64 */
            --i;
            /* Updates red component in the primary color buffer */
            primary_component = smd_pal_primary[i] & 0x00E;
            alternate_component = smd_pal_alternate[i] & 0x00E;
            if (primary_component != alternate_component) {
                smd_pal_update_needed = true;
                smd_pal_primary[i] += primary_component < alternate_component ? 0x002 : -0x002;
            }
            /* Updates green component in the primary color buffer */
            primary_component = smd_pal_primary[i] & 0x0E0;
            alternate_component = smd_pal_alternate[i] & 0x0E0;
            if (primary_component != alternate_component) {
                smd_pal_update_needed = true;
                smd_pal_primary[i] += primary_component < alternate_component ? 0x020 : -0x020;
            }
            /* Updates blue component  in the primary color buffer */
            primary_component = smd_pal_primary[i] & 0xE00;
            alternate_component = smd_pal_alternate[i] & 0xE00;
            if (primary_component != alternate_component) {
                smd_pal_update_needed = true;
                smd_pal_primary[i] += primary_component < alternate_component ? 0x200 : -0x200;
            }
        }
        /* No color change in this step, so the fade operation ended */
        if (!smd_pal_update_needed) {
            smd_pal_fading = false;
            return false;
        }
    }
    /* The fade operation still running */
    return true;
}

inline void
smd_pal_fade_stop(void) {
    smd_pal_fading = false;
}

void
smd_pal_fade_wait(void) {
    while (smd_pal_fade_step()) {
        smd_vdp_vsync_wait();
        smd_pal_update();
    }
}

inline bool
smd_pal_is_fading(void) {
    return smd_pal_fading;
}

void
smd_pal_update(void) {
    if (smd_pal_update_needed) {
        smd_pal_update_needed = false;
        /* Transfer the whole primary color buffer to CRAM */
        smd_dma_cram_transfer_fast(smd_pal_primary, 0, 64, 2);
    }
}
