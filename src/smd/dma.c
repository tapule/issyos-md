/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: dma.c
 * VDP's Direct Memory Access operations
 */

#include "dma.h"
#include "config.h"
#include "z80.h"

/*
 * There are 3 ports to talk with the VDP. These ports can be accessed as 16 or
 * 32 bits.
 * To do DMA transfers, only the ctrl port is needed.
 */
static volatile uint16_t *const vdp_port_ctrl_w = (uint16_t*) 0xC00004;
static volatile uint32_t *const vdp_port_ctrl_l = (uint32_t*) 0xC00004;

/*
 * VDP's registers used to do DMA transfers. These registers are set by writing
 * a control word to the VDP control port where the high byte is as follow:
 *              | 1| 0| 0|R4|R3|R2|R1|R0|
 * R4-R0 is the register to write.
 */
#define VDP_REG_AUTOINC         0x8F00      /* Autoincrement data */
#define VDP_REG_DMALEN_L        0x9300      /* DMA length (low) */
#define VDP_REG_DMALEN_H        0x9400      /* DMA length (high) */
#define VDP_REG_DMASRC_L        0x9500      /* DMA source (low) */
#define VDP_REG_DMASRC_M        0x9600      /* DMA source (mid) */
#define VDP_REG_DMASRC_H        0x9700      /* DMA source (high) */

/*
 * Base commands for the control port to do DMA writes to different VDP's rams
 */
#define VDP_DMA_VRAM_WRITE_CMD      0x40000080
#define VDP_DMA_CRAM_WRITE_CMD      0xC0000080
#define VDP_DMA_VSRAM_WRITE_CMD     0x40000090

/* Defines a DMA queue command operation */
typedef struct dma_command
{
    uint16_t autoinc;       /* Autoincrement register in bytes */
    uint16_t length_h;      /* Length register (high) in words */
    uint16_t length_l;      /* Length register (low) in words */
    uint16_t addr_h;        /* Source address register (high) in words */
    uint16_t addr_m;        /* Source address register (middle) in words */
    uint16_t addr_l;        /* Source address register (low) in words */
    uint16_t ctrl_addr_h;   /* VDP command with the destination address */
    uint16_t ctrl_addr_l;   /* VDP command (low). Start transfer */
} dma_command_t;

/* DMA commands queue and position index */
static dma_command_t queue[DMA_QUEUE_SIZE];
static uint16_t queue_index;


/**
 * @brief Builds a VDP ctrl port write address set command
 * 
 * @param xram_addr VRAM/CRAM/VSRAM DMA address base command
 * @param dest Destination ram address
 * @return uint32_t Ctrl port write address command
 */
static inline uint32_t dma_ctrl_addr_build(uint32_t xram_addr, uint32_t dest)
{
    return (((uint32_t)(xram_addr)) | (((uint32_t)(dest) & 0x3FFF) << 16) | ((uint32_t)(dest) >> 14));
}

/**
 * @brief Executes a DMA transfer from RAM/ROM to VRAM/CRAM/VSRAM without
 *        checking 128kB boundaries
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM/CRAM/VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @param xram_addr VRAM/CRAM/VSRAM DMA address base command
 */
void dma_transfer_fast(uint32_t src, uint16_t dest, uint16_t len, uint16_t inc,
                       uint32_t xram_addr)
{
    /* Used to issue the dma from a ram space */
    volatile uint32_t cmd;
    uint16_t *cmd_p = (uint16_t *) &cmd;

    /* Prevent VDP corruption waiting for a running DMA copy/fill operation */
    dma_wait();

    /* Sets the autoincrement on word writes */
    *vdp_port_ctrl_w = VDP_REG_AUTOINC | inc;
    /* Sets the DMA length in words */
    *vdp_port_ctrl_w = VDP_REG_DMALEN_L | (len & 0xFF);
    *vdp_port_ctrl_w = VDP_REG_DMALEN_H | ((len >> 8) & 0xFF);    
    /*
     * Sets the DMA source address. An additional lshift is needed to convert
     * src from bytes to words
     */
    *vdp_port_ctrl_w = VDP_REG_DMASRC_L | ((src >> 1) & 0xFF);
    *vdp_port_ctrl_w = VDP_REG_DMASRC_M | ((src >> 9) & 0xFF);
    *vdp_port_ctrl_w = VDP_REG_DMASRC_H | ((src >> 17) & 0x7F);
    /* Builds the ctrl port write address command in a ram variable */
    cmd = dma_ctrl_addr_build(xram_addr, dest);
    /* Issues the DMA from a ram varible and in words (see SEGA notes on DMA) */ 
    *vdp_port_ctrl_w = *cmd_p;
    ++cmd_p;
    z80_bus_request_fast();
    *vdp_port_ctrl_w = *cmd_p;
    z80_bus_release();
}

/**
 * @brief Executes a DMA transfer from RAM/ROM to VRAM/CRAM/VSRAM checking 128kB
 *        boundaries
 * 
 * When a transfer from RAM/ROM crosses a 128KB boundary, it must be split in
 * two halves due to a bug in the VDP's DMA
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM/CRAM/VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @param xram_addr VRAM/CRAM/VSRAM DMA address base command
 * @return true On success, false otherwise
 */
bool dma_transfer(uint32_t src, uint16_t dest, uint16_t len, uint16_t inc,
                  uint32_t xram_addr)
{
    uint32_t bytes_to_128k;
    uint32_t words_to_128k;

    if (inc < 2 || len == 0)
    {
        return false;
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
    bytes_to_128k = 0x20000 - (src & 0x1FFFF);
    /* How many words there are until the next 128k jump */
    words_to_128k = 0x20000 - (src & 0x1FFFF);
    if (len > words_to_128k)
    {
        /* Does a fast transfer of second half */
        dma_transfer_fast(src + bytes_to_128k, dest + bytes_to_128k,
                          len - words_to_128k, inc, xram_addr);
        len = words_to_128k;
    }
    /* Does a fast transfer here (first half if we splited) */
    dma_transfer_fast(src, dest, len, inc, xram_addr);
    return true;
}

/**
 * @brief Pushes a DMA transfer operation from RAM/ROM to VRAM/CRAM/VSRAM into
 *        the DMA's queue without checking 128kB boundaries
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM/CRAM/VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @param xram_addr VRAM/CRAM/VSRAM DMA address base command
 */
void dma_queue_push_fast(uint32_t src, uint16_t dest, uint16_t len, uint16_t inc,
                         uint32_t xram_addr)
{
    dma_command_t *cmd;
    uint32_t *ctrl_addr_p;

    cmd = &queue[queue_index];
    ctrl_addr_p = (uint32_t *) &(cmd->ctrl_addr_h);

    /* Sets the autoincrement on word writes */
    cmd->autoinc = VDP_REG_AUTOINC | inc;
    /* Sets the DMA length in words */
    cmd->length_l = VDP_REG_DMALEN_L | (len & 0xFF);
    cmd->length_h = VDP_REG_DMALEN_H | ((len >> 8) & 0xFF);    
    /*
     * Sets the DMA source address. An additional lshift is needed to convert
     * src from bytes to words
     */
    cmd->addr_l = VDP_REG_DMASRC_L | ((src >> 1) & 0xFF);
    cmd->addr_m = VDP_REG_DMASRC_M | ((src >> 9) & 0xFF);
    cmd->addr_h = VDP_REG_DMASRC_H | ((src >> 17) & 0x7F);
    /* Builds the ctrl port write address command in a ram variable */
    *ctrl_addr_p = dma_ctrl_addr_build(xram_addr, dest);
    /* Advances the queue slot index */
    ++queue_index;
}

/**
 * @brief Pushes a DMA transfer operation from RAM/ROM to VRAM/CRAM/VSRAM into
 *        the DMA's queue checking 128kB boundaries
 * 
 * When a transfer operation from RAM/ROM crosses a 128KB boundary, it is
 * splitted in two halves due to a bug in the VDP's DMA. Two DMA commands are
 * pushed to the queue.
 * 
 * @param src Source address on RAM/ROM space
 * @param dest Destination address on VRAM/CRAM/VSRAM
 * @param len Transfer length in words
 * @param inc Write position increment after each write (normally 2)
 * @param xram_addr VRAM/CRAM/VSRAM DMA address base command
 * @return true On success, false otherwise
 */
bool dma_queue_push(uint32_t src, uint16_t dest, uint16_t len, uint16_t inc,
                    uint32_t xram_addr)
{
    uint32_t bytes_to_128k;
    uint32_t words_to_128k;

    if (inc < 2 || len == 0 || (queue_index >= DMA_QUEUE_SIZE))
    {
        return false;
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
    bytes_to_128k = 0x20000 - (src & 0x1FFFF);
    /* How many words there are until the next 128k jump */
    words_to_128k = 0x20000 - (src & 0x1FFFF);
    if (len > words_to_128k)
    {
        /* There is at least space for one commad, but we need two */
        if ((queue_index + 1) >= DMA_QUEUE_SIZE)
        {
            return false;
        }
        /* Pushes a transfer command of second half */
        dma_queue_push_fast(src + bytes_to_128k, dest + bytes_to_128k,
                            len - words_to_128k, inc, xram_addr);
        len = words_to_128k;
    }
    /* Pushes transfer command here (first half if we splited) */
    dma_queue_push_fast(src, dest, len, inc, xram_addr);
    return true;
}

inline void dma_init(void)
{
    queue_index = 0;
}

inline void dma_wait(void)
{
    /* Checks the DMA in progress flag in status register */
    while (*vdp_port_ctrl_l & 0x10)
    {
        __asm__ volatile ("\tnop\n");
    }
}

inline bool dma_vram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_transfer((uint32_t) src, dest, len, inc, VDP_DMA_VRAM_WRITE_CMD);
}

inline bool dma_cram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_transfer((uint32_t) src, dest, len, inc, VDP_DMA_CRAM_WRITE_CMD);
}

inline bool dma_vsram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_transfer((uint32_t) src, dest, len, inc, VDP_DMA_VSRAM_WRITE_CMD);
}

inline uint16_t dma_queue_size(void)
{
    return queue_index;
}

inline void dma_queue_clear(void)
{
    queue_index = 0;
}

void dma_queue_flush(void)
{
    uint32_t *queue_p = (uint32_t *) queue;
    uint16_t i;

    /* Prevent VDP corruption waiting for a running DMA copy/fill operation */
    dma_wait();

    z80_bus_request_fast();
    for (i = 0; i < queue_index; ++i)
    {
        /*
         * Sets the autoincrement on word writes and the high part of the DMA
         * length in words
         */
        *vdp_port_ctrl_l = *queue_p++;
        /*
         * Sets the low part of the DMA length in words and the high part of
         * source address
         */
        *vdp_port_ctrl_l = *queue_p++;
        /* Sets the middle and low part of the DMA source address */
        *vdp_port_ctrl_l = *queue_p++;
        /* Issues the DMA from a ram space and in words (see SEGA notes on DMA) */ 
        *vdp_port_ctrl_w = *queue_p >> 16;
        *vdp_port_ctrl_w = *queue_p++;
    }
    z80_bus_release();
    dma_queue_clear();
}

bool dma_queue_vram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_queue_push((uint32_t) src, dest, len, inc, VDP_DMA_VRAM_WRITE_CMD);
}

bool dma_queue_cram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_queue_push((uint32_t) src, dest, len, inc, VDP_DMA_CRAM_WRITE_CMD);
}

bool dma_queue_vsram_transfer(void *src, uint16_t dest, uint16_t len, uint16_t inc)
{
    return dma_queue_push((uint32_t) src, dest, len, inc, VDP_DMA_VSRAM_WRITE_CMD);
}
