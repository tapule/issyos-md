/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            mem_arena.h
 * \brief           Basic memory arena allocator
 */

#include "mem_arena.h"
#include "mem_utils.h"
#include "kdebug.h"

/**
 * \brief           Memory arena buffer
 */
alignas(2) static uint8_t smd_mem_arena[SMD_MEM_ARENA_SIZE] = {0};

/**
 * \brief           Arena current position (cursor)
 */
static uint16_t smd_mem_arena_pos = 0;

/**
 * \brief           Arena temporary memory position (cursor)
 */
static uint16_t smd_mem_arena_temp_pos = SMD_MEM_ARENA_SIZE;

inline uint16_t
smd_mem_arena_available(void) {
    return smd_mem_arena_temp_pos - smd_mem_arena_pos;
}

void *
smd_mem_arena_alloc(uint16_t size) {
    void *p = nullptr;

    /* Align size to 2 bytes */
    size = (size + 1) & (-2);

    smd_kdebug_error_if(smd_mem_arena_pos + size > smd_mem_arena_temp_pos, "No memory available at smd_mem_arena_alloc");

    p = &smd_mem_arena[smd_mem_arena_pos];
    smd_mem_arena_pos += size;
    return p;
}

inline void *
smd_mem_arena_alloc_zero(const uint16_t size) {
    void *p = nullptr;

    p = smd_mem_arena_alloc(size);
    smd_mem_set(p, 0, size);
    return p;
}

inline void
smd_mem_arena_reset(void) {
    smd_mem_arena_pos = 0;
}

inline void
smd_mem_arena_reset_to(const smd_mem_arena_mark_t mark) {
    smd_kdebug_error_if((uint16_t) mark > smd_mem_arena_temp_pos, "Invalid memory mark at smd_mem_arena_reset_to");

    smd_mem_arena_pos = (uint16_t) mark;
}

inline smd_mem_arena_mark_t
smd_mem_arena_mark_get(void) {
    return smd_mem_arena_pos;
}

void *
smd_mem_arena_temp_alloc(uint16_t size) {
    void *p = nullptr;

    /* Align size to 2 bytes */
    size = (size + 1) & (-2);

    smd_kdebug_error_if(smd_mem_arena_temp_pos - size < smd_mem_arena_pos, "No temporary memory available at smd_mem_arena_temp_alloc");

    smd_mem_arena_temp_pos -= size;
    p = &smd_mem_arena[smd_mem_arena_temp_pos];
    return p;
}

inline void *
smd_mem_arena_temp_alloc_zero(const uint16_t size) {
    void *p = nullptr;

    p = smd_mem_arena_temp_alloc(size);
    smd_mem_set(p, 0, size);
    return p;
}

inline void
smd_mem_arena_temp_reset(void) {
    smd_mem_arena_temp_pos = SMD_MEM_ARENA_SIZE;
}