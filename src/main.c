/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            main.c
 * \brief           Main entry point for the game
 */

#include "game.h"

int main(void)
{
    game_init();
    game_run();
}
