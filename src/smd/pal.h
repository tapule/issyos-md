/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: pal.h
 * Palette routines
 *
 * The Sega Megadrive/Genesis VDP can handle up to four paletes of 16 colors
 * each. The first color of each palette is used as transparency, unless used as
 * background color.
 * Tiles and sprites use this palettes to render its pixels.
 * Palettes are stored in the VDP's internal CRAM where 32 bytes are used for
 * each (2 bytes per color). Colors are stored in BGR format were each component
 * is a hexadecimal digit:
 *       0x0BGR  ->  0000 BBB0 GGG0 RRR0
 * Only even numbers can be used (i.e. 02468ACE).
 * There is no need to write an entire palette, you can write individual colors
 * too.
 *
 * More info:
 * https://www.plutiedev.com/tiles-and-palettes
 * 
 */

#ifndef PAL_H
#define PAL_H

#include <stdint.h>
#include <stdbool.h>

/*
    pal_load
        Carga una paleta en un slot, 16 colores
        -> usa dma (con o sin cola??)

    pal_color_set
        establece uno de los 64 colores??
        establece un color de una paleta??
        -> es directa, no dma

    pal_colors_set
        Establece varios colores de la cram de golpe
        -> usa dma (con o sin cola?)
        -> debería ser interna??

  
    
    pal_fade_in
    pal_fade_out

    pal_fade_update
        esta hace realmente el fade. Se debe llamar en el update del principal
        Quizá sería mejor un pal_fade_step y un pal_update que se tenga que
        llamar desde el principal no?
    pal_is_fading


La idea es mantener un buffer de paletas y que el pal_update las suba al vdp si
es necesario. Por ejemplo, cuando se esta haciendo fade, puede que cada frame
sea necesario actualizar la cram... o algo así
Sería como si las operaciones de las paletas se hicieran internamente en el buff
y una vez completado el trabajo se subiera el buffer a la cram. pal_update
debería controlar cuando hay que ejecutar un paso de fade o cuando hay que
cambiar un color.
Por ejemplo, para hacer un fade_in se podría hacer
    pal_load pal1, &palette2    // Carga pal1 del buffer interno
    pal_load pal2, &palette2    // Carga pal2 del buffer interno
    pal_load pal3, &palette3    ...
    pal_load pal4, &palette4    ...
    pal_fade_in speed           // Inicia un fade de negro a las paletas del buff
    pal_update                  // Prepara las paletas, el buff y upload a cram


Puede que se quiera hacer un fade a una paleta determinada o un color, por
ejemplo a blanco para hacer un efecto de flash, o a rojo, etc. Esto implica que
hay que hacer una interpolación (más o menos) de un color a otro y se necesitan
dos sets de paletas: origen, destino



*/

#endif // PAL_H
