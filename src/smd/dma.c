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

/*
 * There are 3 ports to talk with the VDP. These ports can be accessed as 16 or
 * 32 bits.
 * To work with the VDP we need to write commands to the control port and if we
 * want to write or read vram, cram or vsram we must write the address to/from
 * the data port.
 */
static volatile uint16_t *const vdp_port_data_w = (uint16_t*) 0xC00000;
static volatile uint32_t *const vdp_port_data_l = (uint32_t*) 0xC00000;
static volatile uint16_t *const vdp_port_ctrl_w = (uint16_t*) 0xC00004;
static volatile uint32_t *const vdp_port_ctrl_l = (uint32_t*) 0xC00004;


typedef struct queue_command
{

} queue_command_t;


static queue_command_t queue[DMA_QUEUE_SIZE];
static uint16_t queue_index;

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

void dma_vram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}

void dma_cram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}

void dma_vsram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}

void dma_vram_fill(uint16_t to, uint16_t len, int16_t step, uint8_t val)
{

}

void dma_vram_copy(uint16_t from, uint16_t to, uint16_t len, int16_t step)
{

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
    
}

bool dma_queue_vram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}

bool dma_queue_cram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}

bool dma_queue_vsram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step)
{

}
