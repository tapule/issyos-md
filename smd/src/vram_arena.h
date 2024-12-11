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
 *
 * Basic VRAM arena allocator over the first part of tiles in VRAM memory. It
 * manages a buffer of tiles in the range [0 ... (SMD_VRAM_ARENA_SIZE - 1)] that
 * cannot grow.
 * Tiles in the arena are reserved linerly and can be reset completely or to a
 * previous position using marks. Maks record a position in the arena to be
 * reset later.
 */

#ifndef SMD_VRAM_ARENA_H
#define SMD_VRAM_ARENA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Default VRAM arena size in tiles
 */
#ifndef SMD_VRAM_ARENA_SIZE
    #define SMD_VRAM_ARENA_SIZE 1536
#endif

/**
 * \brief           Arena buffer position mark used to reset to a concrete point
 */
typedef uint16_t smd_vram_arena_mark_t;

/**
 * \brief           Reserve tiles from the VRAM arena
 * \param[in]       size: Amount of tiles to reserve
 * \return          Start tile index in VRAM for the reserved memory
 */
uint16_t smd_vram_arena_alloc(const uint16_t size);

/**
 * \brief           Reset the VRAM arena to its empty state
 */
void smd_vram_arena_reset(void);

/**
 * \brief           Reset the VRAM arena cursor back to a saved mark position
 * \param[in]       mark: Mark to reset the arena cursor to
 */
void smd_vram_arena_reset_to(const smd_vram_arena_mark_t mark);

/**
 * \brief           Get a mark to the current VRAM arena cursor position
 * \return          Arena cursor position (used to reset back to it)
 */
smd_vram_arena_mark_t smd_vram_arena_mark_get(void);

/**
 * \brief           Get the available VRAM arena size in tiles
 * \return          Current amount of free tiles in the VRAM arena
 */
uint16_t smd_vram_arena_available(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_VRAM_ARENA_H */
