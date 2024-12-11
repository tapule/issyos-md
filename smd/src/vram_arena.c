/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            vram_arena.h
 * \brief           Basic VRAM memory allocator
 */

#include "vram_arena.h"
#include "kdebug.h"

/**
 * \brief           Arena current position (cursor)
 */
static uint16_t smd_vram_arena_pos = 0;

inline uint16_t
smd_vram_arena_alloc(const uint16_t size) {
    uint16_t current_pos;

    smd_kdebug_error_if(smd_vram_arena_pos + size > SMD_VRAM_ARENA_SIZE, "No VRAM available at smd_vram_arena_alloc");

    current_pos = smd_vram_arena_pos;
    smd_vram_arena_pos += size;
    return current_pos;
}

inline void
smd_vram_arena_reset(void) {
    smd_vram_arena_pos = 0;
}

inline void
smd_vram_arena_reset_to(const smd_vram_arena_mark_t mark) {
    smd_kdebug_error_if((uint16_t) mark > SMD_VRAM_ARENA_SIZE, "Invalid VRAM mark at smd_vram_arena_reset_to");

    smd_vram_arena_pos = (uint16_t) mark;
}

inline smd_vram_arena_mark_t
smd_vram_arena_mark_get(void) {
    return smd_vram_arena_pos;
}

inline uint16_t
smd_vram_arena_available(void) {
    return SMD_VRAM_ARENA_SIZE - smd_vram_arena_pos;
}
