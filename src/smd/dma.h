/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: dma.h
 * VDP's Direct Memory Access operations
 *
 * Althought you can write the Sega Megadrive/Genesis VDP's video memory using
 * the m68k directly, the VDP's DMA capabilities let you transfer large blocks
 * of memory (graphics) directly without the CPU's help.
 * DMA operations are faster during the vertical blanking or when the display is
 * disabled.
 *
 * More info:
 * https://www.plutiedev.com/dma-transfer
 * https://github.com/Stephane-D/SGDK/blob/master/inc/dma.h
 */

#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialises the DMA system
 * 
 * @note This function is called from the boot process so maybe you don't need
 * to call it anymore unless you want to reset the DMA system.
 */
void dma_init(void);

/**
 * @brief Wait for a running DMA copy/fill operation to finish
 * 
 */
void dma_wait(void);

/**
 * @brief Executes a DMA transfer from RAM/ROM to VRAM
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false otherwise
 */
bool dma_vram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

/**
 * @brief Executes a DMA transfer from RAM/ROM to CRAM
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on CRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false otherwise
 */
bool dma_cram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

/**
 * @brief Executes a DMA transfer from RAM/ROM to VSRAM
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false otherwise
 */
bool dma_vsram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

/**
 * @brief Returns the current DMA's queue command size
 * 
 * @return uint16_t Total DMA commands in the queue
 */
uint16_t dma_queue_size(void);

/**
 * @brief Resets the DMA's queue command
 * 
 */
void dma_queue_clear(void);

/**
 * @brief Executes all the pending DMA's commands in the queue and resets it
 * 
 */
void dma_queue_flush(void);

/**
 * @brief Adds a new DMA transfer from RAM/ROM to VRAM in the queue
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false if the queue is full
 */
bool dma_queue_vram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

/**
 * @brief Adds a new DMA transfer from RAM/ROM to CRAM in the queue
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on CRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false if the queue is full
 */
bool dma_queue_cram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

/**
 * @brief Adds a new DMA transfer from RAM/ROM to VSRAM in the queue
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @return True on success, false if the queue is full
 */
bool dma_queue_vsram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc);

#endif // DMA_H
