/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            sprite.h
 * \brief           ??
 *
 * ??
 *
 * More info:
 */

#ifndef SMD_SPRITE_H
#define SMD_SPRITE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SMD_SPR_SIZE_1X1 = 0x00,
    SMD_SPR_SIZE_1X2 = 0x01,
    SMD_SPR_SIZE_1X3 = 0x02,
    SMD_SPR_SIZE_1X4 = 0x03,
    SMD_SPR_SIZE_2X1 = 0x04,
    SMD_SPR_SIZE_2X2 = 0x05,
    SMD_SPR_SIZE_2X3 = 0x06,
    SMD_SPR_SIZE_2X4 = 0x07,
    SMD_SPR_SIZE_3X1 = 0x08,
    SMD_SPR_SIZE_3X2 = 0x09,
    SMD_SPR_SIZE_3X3 = 0x0A,
    SMD_SPR_SIZE_3X4 = 0x0B,
    SMD_SPR_SIZE_4X1 = 0x0C,
    SMD_SPR_SIZE_4X2 = 0x0D,
    SMD_SPR_SIZE_4X3 = 0x0E,
    SMD_SPR_SIZE_4X4 = 0x0F
} smd_spr_size_t;

void smd_spr_init(void);

uint16_t smd_spr_attributes_encode(const uint16_t priority, const uint16_t palette, const uint16_t v_flip,
                                   const uint16_t h_flip, const uint16_t tile_index);
// smd_spr_attributes_priority_set
// smd_spr_attributes_palette_set
// // void smd_spr_attributes_vflip_set(uint16_t* attributes);
void smd_spr_attributes_vflip(uint16_t *attributes);
uint16_t smd_spr_attributes_vflipb(uint16_t attributes);

// // void smd_spr_attributes_hflip_set(uint16_t* attributes);
void smd_spr_attributes_hflip(uint16_t *attributes);

// smd_spr_attributes_index_set

uint8_t smd_spr_size_encode(const uint8_t width, const uint8_t height);

uint8_t smd_spr_size_to_tiles(const smd_spr_size_t size);

// Esto se podría optimizar con un puntero a un struct para evitar
// pasar params por pila
void smd_spr_add(int16_t x, int16_t y, uint16_t attributes, uint8_t size);

void smd_spr_clear(void);

void smd_spr_update(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_SPRITE_H */
