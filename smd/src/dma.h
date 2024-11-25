/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
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
#include "vdp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Default DMA internal queue size in commands or operations
 */
#ifndef SMD_DMA_QUEUE_SIZE
    #define SMD_DMA_QUEUE_SIZE (64)
#endif

/**
 * \brief           DMA transfer types based on VDP's ram destination
 */
typedef enum smd_dma_transfer_type_t {
    SMD_DMA_VRAM_TRANSFER  = SMD_VDP_DMA_VRAM_WRITE_CMD,    /**< Ram/Rom to VRam transfer */
    SMD_DMA_CRAM_TRANSFER  = SMD_VDP_DMA_CRAM_WRITE_CMD,    /**< Ram/Rom to CRam transfer */
    SMD_DMA_VSRAM_TRANSFER = SMD_VDP_DMA_VSRAM_WRITE_CMD    /**< Ram/Rom to VSRam transfer */
} smd_dma_transfer_type_t;

/**
 * \brief           DMA transfer operation
 */
typedef struct smd_dma_transfer_t {
    void *src;                      /**< Source address on Ram/Rom space */
    uint16_t dest;                  /**< Destination address on VRam/CRam/VSRam */
    uint16_t size;                  /**< Transfer size in words */
    uint16_t inc;                   /**< Write position increment after each write (normally 2) */
    smd_dma_transfer_type_t type;   /**< DMA transfer type */
} smd_dma_transfer_t;

/**
 * \brief           Convenient alias for DMA transfer functions
 */
typedef void (*smd_dma_transfer_ft)(const smd_dma_transfer_t *restrict transfer);

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
 * \brief           Execute a DMA transfer from RAM/ROM to VRam/CRam/VSRam
 * \param[in]       transfer: Transfer operation configuration
 * \pre             transfer->inc must be at least 2
 */
void smd_dma_transfer(const smd_dma_transfer_t *restrict transfer);

/**
 * \brief           Execute a fast DMA transfer from RAM/ROM to VRam/CRam/VSRam
 * \param[in]       transfer: Transfer operation configuration
 * \pre             transfer->inc must be at least 2
 * \note            Parameters or 128kB boundaries are not checked, so be aware
 *                  that it is a bit unsafe if you don't know what you are doing
 */
void smd_dma_transfer_fast(const smd_dma_transfer_t *restrict transfer);

/**
 * \brief           Enqueue a new DMA transfer from RAM/ROM to VRam/CRam/VSRam
 * \param[in]       transfer: Transfer operation configuration
 * \pre             transfer->inc must be at least 2
 */
void smd_dma_transfer_enqueue(const smd_dma_transfer_t *restrict transfer);

/**
 * \brief           Executes a DMA VRAM fill operation
 * \param[in]       dest: Destination address on VRAM
 * \param[in]       size: Transfer size in bytes, minimum 2
 * \param[in]       value: Value used to fill the vram
 * \param[in]       inc: Write position increment after each write (normally 1)
 * \return          true on success, false otherwise
 * \pre             size must be at least 2 because of a fill operation bug that
 *                  writes a minimum of two bytes
 * \note            The DMA VRAM fill operation does not stop the m68k, so it is
 *                  a good idea to use it with smd_dma_wait() function to wait for
 *                  it to finish the fill operation.
 */
void smd_dma_vram_fill(const uint16_t dest, uint16_t size, const uint8_t value, const uint16_t inc);

#ifdef __cplusplus
}
#endif

#endif /* SMD_DMA_H */
