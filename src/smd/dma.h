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
 */

#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stdbool.h>

void dma_init(void);

void dma_wait(void);

void dma_vram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

void dma_cram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

void dma_vsram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

void dma_vram_fill(uint16_t to, uint16_t len, int16_t step, uint8_t val);

void dma_vram_copy(uint16_t from, uint16_t to, uint16_t len, int16_t step);


uint16_t dma_queue_size(void);

void dma_queue_clear(void);

void dma_queue_flush(void);

// True if queue is full
bool dma_queue_vram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

bool dma_queue_cram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

bool dma_queue_vsram_transfer(uint32_t from, uint16_t to, uint16_t len, int16_t step);

/*
    * TODO:
 
    dma_queue_command
        interna. Encola un comando. Genérica para operaciones internas

    dma_command
        interna. realiza una operacion dma directamente sin cola

*/

#endif // DMA_H
