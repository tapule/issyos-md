/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            dma.h
 * \brief           VDP's Direct Memory Access operations
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

#ifndef SMD_DMA_H
#define SMD_DMA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dma_transfer_t {
    void *src;
    uint16_t dest;
    uint16_t size;
    uint16_t inc;
} dma_transfer_t;


/**
 * \brief           Default DMA internal queue size in commands or operations
 */
#ifndef SMD_DMA_QUEUE_SIZE
    #define SMD_DMA_QUEUE_SIZE (64)
#endif

/**
 * \brief           Initialize the DMA system
 * \note            This function is called from the boot process so maybe you
 *                  don't need to call it anymore unless you want to reset the
 *                  DMA system.
 */
void smd_dma_init(void);

/**
 * \brief           Wait for a running DMA copy/fill operation to finish
 */
void smd_dma_wait(void);

/**
 * \brief           Execute a DMA transfer from RAM/ROM to VRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VRAM
 * \param[in]       size Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 */
bool smd_dma_vram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size, const uint16_t inc);

/**
 * \brief           Execute a DMA transfer from RAM/ROM to CRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on CRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 */
bool smd_dma_cram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size, const uint16_t inc);

/**
 * \brief           Execute a DMA transfer from RAM/ROM to VSRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VSRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 */
bool smd_dma_vsram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size, const uint16_t inc);

/**
 * \brief           Execute a fast DMA transfer from RAM/ROM to VRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 * \note            Parameters or 128kB boundaries are not checked, so be aware
 *                  that it is a bit unsafe if you don't know what you are doing
 */
bool smd_dma_vram_transfer_fast(const void *restrict src, const uint16_t dest, const uint16_t size, const uint16_t inc);

/**
 * \brief           Execute a fast DMA transfer from RAM/ROM to CRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on CRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 * \note            Parameters or 128kB boundaries are not checked, so be aware
 *                  that it is a bit unsafe if you don't know what you are doing
 */
bool smd_dma_cram_transfer_fast(const void *restrict src, const uint16_t dest, const uint16_t size, const uint16_t inc);

/**
 * \brief           Execute a fast DMA transfer from RAM/ROM to VSRAM
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VSRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false otherwise
 * \note            Parameters or 128kB boundaries are not checked, so be aware
 *                  that it is a bit unsafe if you don't know what you are doing
 */
bool smd_dma_vsram_transfer_fast(const void *restrict src, const uint16_t dest, const uint16_t size,
                                 const uint16_t inc);

/**
 * \brief           Executes a DMA VRAM fill operation
 * \param[in]       dest: Destination address on VRAM
 * \param[in]       size: Transfer size in bytes, minimum 2
 * \param[in]       value: Value used to fill the vram
 * \param[in]       inc: Write position increment after each write (normally 1)
 * \return          true on success, false otherwise
 * \note            The DMA VRAM fill operation does not stop the m68k, so it is
 *                  a good idea to use it with smd_dma_wait() function to wait for
 *                  it to finish the fill operation.
 */
bool smd_dma_vram_fill(const uint16_t dest, uint16_t size, const uint8_t value, const uint16_t inc);

/**
 * \brief           Return the current DMA's queue size in commands
 * \return          Total DMA commands in the queue
 */
uint16_t smd_dma_queue_size(void);

/**
 * \brief           Reset the DMA's queue
 */
void smd_dma_queue_clear(void);

/**
 * \brief           Execute all the pending DMA's commands in the queue.
 * \note            The queue ends empty
 */
void smd_dma_queue_flush(void);

/**
 * \brief           Add a new DMA transfer from RAM/ROM to VRAM to the queue
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false if the queue is full
 */
bool smd_dma_queue_vram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size,
                                 const uint16_t inc);

/**
 * \brief           Add a new DMA transfer from RAM/ROM to CRAM in the queue
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on CRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false if the queue is full
 */
bool smd_dma_queue_cram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size,
                                 const uint16_t inc);

/**
 * \brief           Add a new DMA transfer from RAM/ROM to VSRAM in the queue
 * \param[in]       src: Source address on RAM/ROM space
 * \param[in]       dest: Destination address on VSRAM
 * \param[in]       size: Transfer size in words
 * \param[in]       inc: Write position increment after each write (normally 2)
 * \return          true on success, false if the queue is full
 */
bool smd_dma_queue_vsram_transfer(const void *restrict src, const uint16_t dest, const uint16_t size,
                                  const uint16_t inc);

#ifdef __cplusplus
}
#endif

#endif /* SMD_DMA_H */
