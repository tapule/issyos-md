/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            dma.c
 * \brief           VDP's Direct Memory Access operations
 */

#include "dma.h"
#include "mem_map.h"
#include "vdp.h"
#include "z80.h"

/**
 * \brief            Define a DMA queue command operation
 */
typedef struct smd_dma_queue_cmd_t {
    uint16_t autoinc;           /**< Autoincrement register in bytes */
    uint16_t length_h;          /**< Length register (high) in words */
    uint16_t length_l;          /**< Length register (low) in words */
    uint16_t addr_h;            /**< Source address register (high) in words */
    uint16_t addr_m;            /**< Source address register (middle) in words */
    uint16_t addr_l;            /**< Source address register (low) in words */
    uint16_t ctrl_addr_h;       /**< VDP command with the destination address */
    uint16_t ctrl_addr_l;       /**< VDP command (low). Start transfer */
} smd_dma_queue_cmd_t;

/**
 * \brief           DMA command queue and position index
 */
static smd_dma_queue_cmd_t smd_dma_queue[SMD_DMA_QUEUE_SIZE];
static uint16_t smd_dma_queue_index;

/**
 * \brief           Build a VDP ctrl port write address set command
 * \param[in]       xram_addr: VRAM/CRAM/VSRAM DMA address base command
 * \param[in]       dest: Destination ram address
 * \return          Ctrl port write address command
 */
static inline uint32_t
smd_dma_ctrl_addr_build(const smd_dma_transfer_type_t xram_addr, const uint32_t dest) {
    return (((uint32_t)(xram_addr)) | (((uint32_t)(dest) & 0x3FFF) << 16) | ((uint32_t)(dest) >> 14));
}

inline void
smd_dma_init(void) {
    smd_dma_queue_index = 0;
}

inline void
smd_dma_wait(void) {
    /* Checks the DMA in progress flag in status register */
    while (*SMD_VDP_CTRL_PORT_U32 & 0x10) {
        __asm__ volatile("\tnop\n");
    }
}

inline uint16_t
smd_dma_queue_size(void) {
    return smd_dma_queue_index;
}

inline void
smd_dma_queue_clear(void) {
    smd_dma_queue_index = 0;
}

void
smd_dma_queue_flush(void) {
    uint32_t *queue_p = (uint32_t *) smd_dma_queue;

    smd_z80_bus_request_fast();
    for (uint16_t i = 0; i < smd_dma_queue_index; ++i) {
        /*
         * Sets the autoincrement on word writes and the high part of the DMA
         * size in words
         */
        *SMD_VDP_CTRL_PORT_U32 = *queue_p;
        ++queue_p;
        /*
         * Sets the low part of the DMA size in words and the high part of
         * source address
         */
        *SMD_VDP_CTRL_PORT_U32 = *queue_p;
        ++queue_p;
        /* Sets the middle and low part of the DMA source address */
        *SMD_VDP_CTRL_PORT_U32 = *queue_p;
        ++queue_p;
        /* Issues the DMA from ram space and in words (see SEGA notes on DMA) */
        *SMD_VDP_CTRL_PORT_U16 = *queue_p >> 16;
        *SMD_VDP_CTRL_PORT_U16 = *queue_p;
        ++queue_p;
    }
    smd_z80_bus_release();
    smd_dma_queue_clear();
}

void
smd_dma_transfer(const smd_dma_transfer_t *restrict transfer) {
    uint32_t bytes_to_128k;
    uint32_t words_to_128k;
    uint16_t transfer_size;

    /*
     * We need to control transfers which cross 128kB boundaries due to a bug in
     * the VDP's DMA. If a transfer crosses a 128kB boundary, the data that
     * crosses the limit will be random, that is, garbage data.
     * If the data crosses the limit, we need to split the transfer in two
     * halves.There is no need to do more than two transfers as the VDPS's
     * maximum ram (vram, cram, vsram) size is 64kB.
     */
    /* How many bytes there are until the next 128k jump */
    bytes_to_128k = 0x20000 - ((uint32_t) transfer->src & 0x1FFFF);
    /* How many words there are until the next 128k jump */
    words_to_128k = bytes_to_128k >> 1;
    transfer_size = transfer->size;
    if (transfer_size > words_to_128k) {
        /* Does a fast transfer of second half */
        smd_dma_transfer_fast( &(smd_dma_transfer_t) {
            .type = transfer->type,
            .src = (void *) (((uint32_t) transfer->src) + bytes_to_128k),
            .dest = transfer->dest + bytes_to_128k,
            .size = transfer_size - words_to_128k,
            .inc = transfer->inc
        });
        transfer_size = words_to_128k;
    }
    /* Does a fast transfer here (first half if we splited) */
    smd_dma_transfer_fast( &(smd_dma_transfer_t) {
        .type = transfer->type,
        .src = transfer->src,
        .dest = transfer->dest,
        .size = transfer_size,
        .inc = transfer->inc
    });
}

void
smd_dma_transfer_fast(const smd_dma_transfer_t *restrict transfer) {
    /* Used to issue the dma from a ram space */
    volatile uint32_t cmd;
    uint16_t *cmd_p = (uint16_t *) &cmd;

    /* Prevent VDP corruption waiting for a running DMA copy/fill operation */
    smd_dma_wait();

    /* Sets the autoincrement on word writes */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_AUTOINC | transfer->inc;
    /* Sets the DMA size in words */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMALEN_L | (transfer->size & 0xFF);
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMALEN_H | ((transfer->size >> 8) & 0xFF);
    /*
     * Sets the DMA source address. An additional lshift is needed to convert
     * src from bytes to words
     */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMASRC_L | (((uint32_t) transfer->src >> 1) & 0xFF);
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMASRC_M | (((uint32_t) transfer->src >> 9) & 0xFF);
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMASRC_H | (((uint32_t) transfer->src >> 17) & 0x7F);
    /* Builds the ctrl port write address command in a ram variable */
    cmd = smd_dma_ctrl_addr_build(transfer->type, transfer->dest);
    /* Issues the DMA from a ram varible and in words (see SEGA notes on DMA) */
    *SMD_VDP_CTRL_PORT_U16 = *cmd_p;
    ++cmd_p;
    smd_z80_bus_request_fast();
    *SMD_VDP_CTRL_PORT_U16 = *cmd_p;
    smd_z80_bus_release();
}

/**
 * \brief           Enqueue a new DMA transfer from RAM/ROM to VRam/CRam/VSRam
 * \param[in]       transfer: Transfer operation configuration
 * \pre             transfer->inc must be at least 2
 * \note            Parameters or 128kB boundaries are not checked, so be aware
 *                  that it is a bit unsafe if you don't know what you are doing
 */
void
smd_dma_transfer_enqueue_fast(const smd_dma_transfer_t *restrict transfer) {
    smd_dma_queue_cmd_t *cmd;
    uint32_t *ctrl_addr_p;

    cmd = &smd_dma_queue[smd_dma_queue_index];
    ctrl_addr_p = (uint32_t *) &(cmd->ctrl_addr_h);

    /* Sets the autoincrement on word writes */
    cmd->autoinc = SMD_VDP_REG_AUTOINC | transfer->inc;
    /* Sets the DMA size in words */
    cmd->length_l = SMD_VDP_REG_DMALEN_L | (transfer->size & 0xFF);
    cmd->length_h = SMD_VDP_REG_DMALEN_H | ((transfer->size >> 8) & 0xFF);
    /*
     * Sets the DMA source address. An additional lshift is needed to convert
     * src from bytes to words
     */
    cmd->addr_l = SMD_VDP_REG_DMASRC_L | (((uint32_t) transfer->src >> 1) & 0xFF);
    cmd->addr_m = SMD_VDP_REG_DMASRC_M | (((uint32_t) transfer->src >> 9) & 0xFF);
    cmd->addr_h = SMD_VDP_REG_DMASRC_H | (((uint32_t) transfer->src >> 17) & 0x7F);
    /* Builds the ctrl port write address command in a ram variable */
    *ctrl_addr_p = smd_dma_ctrl_addr_build(transfer->type, transfer->dest);
    /* Advances the queue slot index */
    ++smd_dma_queue_index;
}

void
smd_dma_transfer_enqueue(const smd_dma_transfer_t *restrict transfer) {
    uint32_t bytes_to_128k;
    uint32_t words_to_128k;
    uint16_t transfer_size;

    if (smd_dma_queue_index >= SMD_DMA_QUEUE_SIZE) {
        return;
    }

    /*
     * We need to control transfers which cross 128kB boundaries due to a bug in
     * the VDP's DMA. If a transfer crosses a 128kB boundary, the data that
     * crosses the limit will be random, that is, garbage data.
     * If the data crosses de limit, we need to split the transfer in two
     * halves.There is no need to do more than two transfers as the VDPS's
     * maximum ram (vram, cram, vsram) size is 64kB.
     */
    /* How many bytes there are until the next 128k jump */
    bytes_to_128k = 0x20000 - ((uint32_t) transfer->src & 0x1FFFF);
    /* How many words there are until the next 128k jump */
    words_to_128k = bytes_to_128k >> 1;
    transfer_size = transfer->size;
    if (transfer_size > words_to_128k) {
        /* There is at least space for one commad, but we need two */
        if ((smd_dma_queue_index + 1) >= SMD_DMA_QUEUE_SIZE) {
            return;
        }
        /* Pushes a transfer command of second half */
        smd_dma_transfer_enqueue_fast( &(smd_dma_transfer_t) {
            .type = transfer->type,
            .src = (void *) (((uint32_t) transfer->src) + bytes_to_128k),
            .dest = transfer->dest + bytes_to_128k,
            .size = transfer_size - words_to_128k,
            .inc = transfer->inc
        });
        transfer_size = words_to_128k;
    }
    /* Pushes transfer command here (first half if we splited) */
    smd_dma_transfer_enqueue_fast( &(smd_dma_transfer_t) {
        .type = transfer->type,
        .src = transfer->src,
        .dest = transfer->dest,
        .size = transfer_size,
        .inc = transfer->inc
    });
}

void
smd_dma_vram_fill(const uint16_t dest, uint16_t size, const uint8_t value, const uint16_t inc) {
    /*
     * In a DMA fill operation, the first write writes an entire word instead of
     * a byte. Then, in each write a byte is written. Therefore, a size of 1
     * will write 2 bytes, a size of 2 will write 3 bytes, etc.
     * We need to do an initial decrement to size.
     *      value = 0xFF
     *      size = 2 -> size = 1 -> FF FF
     *      size = 3 -> size = 2 -> FF FF FF
     *      size = 4 -> size = 3 -> FF FF FF FF
     */
    --size;

    /* Prevent VDP corruption waiting for a running DMA copy/fill operation */
    smd_dma_wait();

    /* Sets the autoincrement after each write */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_AUTOINC | inc;
    /* Sets the DMA size in bytes */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMALEN_L | (size & 0xFF);
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMALEN_H | ((size >> 8) & 0xFF);
    /* Sets the DMA operation to VRAM fill operation */
    *SMD_VDP_CTRL_PORT_U16 = SMD_VDP_REG_DMASRC_H | 0x80;
    /* Builds the ctrl port write address command */
    *SMD_VDP_CTRL_PORT_U32 = smd_dma_ctrl_addr_build(SMD_VDP_DMA_VRAM_WRITE_CMD, dest);
    /* Set fill value. The high byte must be equal for the first write */
    *SMD_VDP_DATA_PORT_U16 = (value << 8) | value;
}
