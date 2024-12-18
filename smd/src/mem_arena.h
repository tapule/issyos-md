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
 *
 * Basic memory allocator. It has an internal memory buffer of statically
 * allocated SMD_MEM_ARENA_SIZE bytes that cannot grow.
 * It provides two ways to allocate memory:
 *  - From the top of the buffer in a linear fashion where you can reset the
 *  buffer completely or to a previous position using marks.
 *  - From the bottom of the buffer intended for temporary operations with no
 *  guarantee that they will be preserved outside of your function. Here you can
 *  only allocate and reset, no marks are allowed.
 */

#ifndef SMD_MEM_ARENA_H
#define SMD_MEM_ARENA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Default arena size in bytes
 */
#ifndef SMD_MEM_ARENA_SIZE
    #define SMD_MEM_ARENA_SIZE (5 * 1024)
#endif

/**
 * \brief           Allocate count elements of a type in the memory arena
 * \param[in]       type: Type of elements to allocate
 * \param[in        count: Amount of elements to allocate
 * \return          Pointer to the reserved memory
 */
#define smd_mem_arena_alloc_type(type, count) \
    ((type *) smd_mem_arena_alloc((count) * sizeof(type)))

/**
 * \brief           Allocate count elements of a type in the memory arena and
 *                  initialize them to 0
 * \param[in]       type: Type of elements to allocate
 * \param[in        count: Amount of elements to allocate
 * \return          Pointer to the reserved memory
 */
#define smd_mem_arena_alloc_type_zero(type, count) \
    ((type *) smd_mem_arena_alloc_zero((count) * sizeof(type)))

/**
 * \brief           Arena buffer position mark used to reset to a concrete point
 */
typedef uint16_t smd_mem_arena_mark_t;

/**
 * \brief           Get the arena available free memory in bytes
 * \return          Current amount of free bytes in arena
 */
uint16_t smd_mem_arena_available(void);

/**
 * \brief           Reserve bytes from the memory arena
 * \param[in]       size: Amount of bytes to reserve
 * \return          Pointer to the reserved memory
 */
void *smd_mem_arena_alloc(uint16_t size);

/**
 * \brief           Reserve bytes from the memory arena and initilize them to 0
 * \param[in]       size: Amount of bytes to reserve
 * \return          Pointer to the reserved memory
 */
void *smd_mem_arena_alloc_zero(const uint16_t size);

/**
 * \brief           Reset the memory arena to its empty state
 */
void smd_mem_arena_reset(void);

/**
 * \brief           Reset the memory arena cursor back to a saved mark position
 * \param[in]       mark: Mark to reset the arena cursor to
 */
void smd_mem_arena_reset_to(const smd_mem_arena_mark_t mark);

/**
 * \brief           Get a mark to the current memory arena cursor position
 * \return          Arena cursor position (used to reset back to it)
 */
smd_mem_arena_mark_t smd_mem_arena_mark_get(void);

/**
 * \brief           Reserve bytes from the temporary memory arena
 * \param[in]       size: Amount of bytes to reserve
 * \return          Pointer to the reserved memory
 */
void *smd_mem_arena_temp_alloc(const uint16_t size);

/**
 * \brief           Reserve bytes from the temporary memory arena
 * \param[in]       size: Amount of bytes to reserve
 * \return          Pointer to the reserved memory
 */
void *smd_mem_arena_temp_alloc_zero(const uint16_t size);

/**
 * \brief           Reset the temporary memory in the arena to its empty state
 */
void smd_mem_arena_temp_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_MEM_ARENA_H */
