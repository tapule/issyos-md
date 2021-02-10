/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: megadrive.h
 * Sega Megadrive/Genesis main control unit
 *
 * System core routines to manage the Sega Megadrive/Genesis hardware
 *
 */

#ifndef MEGADRIVE_H
#define MEGADRIVE_H

#include "pad.h"
#include "pal.h"
#include "psg.h"
#include "rand.h"
#include "video.h"
#include "z80.h"

/**
 * @brief 
 * 
 */
void smd_init(void);

/**
 * @brief Habilita las interrupciones
 * 
 */
void smd_ints_enable(void);

/**
 * @brief deshabilita las interrupciones
 * 
 */
void smd_ints_disable(void);

/**
 * @brief Obtiene el estado de las interrupciones
 * 
 */
uint16_t smd_ints_status(void);



/*
    * TODO:

    smd_ints_mask_set
        Establecer la mascara de interrupciones
    smd_ints_mask_get
        Establecer la mascara de interrupciones


*/


#endif // MEGADRIVE_H
