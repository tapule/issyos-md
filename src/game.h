/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2022 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: game.h
 * Main game object
 *
 */

#ifndef GAME_H
#define GAME_H

#include <mddev.h>

/* Game initialization */
void game_init(void);

/* Starts the game */
void game_run(void);

#endif /* GAME_H */