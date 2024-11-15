/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2022
 * Github: https://github.com/tapule/issyos-md
 *
 * File: main.c
 * Main entry point for the game
 */

#include <mddev.h>
#include "game.h"

int main(void)
{
    game_init();
    game_run();
}
