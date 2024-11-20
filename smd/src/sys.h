/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            sys.h
 * \brief           System core routines to boot and manage the Sega
 *                  Megadrive/Genesis hardware
 *
 * More info:
 * https://www.eeeguide.com/register-architecture-of-68000-microprocessor/
 */

/* TODO: Este módulo está en desarrollo. Faltan muchas funciones por implementar. */

#ifndef SMD_SYS_H
#define SMD_SYS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           SMD framework version
 */
#define SMD_VERSION 0.1

/**
 * \brief           Enable system interrupts
 *
 * The m68k provides 7 levels of interrupts were 7 has the highest priority and
 * 0 the lowest. For the Sega Megadrive/Genesis the most important interrupts
 * are:
 *  Level 6: Vertical retrace interrupt
 *  Level 4: Horizontal retrace interrupt
 *  Level 2: External interrupt
 * This function enables all of them
 */
void smd_ints_enable(void);

/**
 * \brief           Disable system interrupts
 */
void smd_ints_disable(void);

/**
 * \brief           Get interrupts status
 * \return          true if interrupts are enabled, false otherwise
 */
bool smd_ints_status(void);

/**
 * \brief           Check if the system is using PAL or NTSC video mode
 *
 * The Sega Megadrive/Genesis can run in PAL mode with a clock of 7.60 MHz or in
 * NTSC with a clock of 7.67 MHz.
 *
 * \return          true if the system is in PAL mode, otherwise it is in NTSC
 */
bool smd_is_pal(void);

/**
 * \brief           Check if the system is running in Japanese (Domestic) mode
 *
 * The Sega Megadrive/Genesis can run in Domestic mode (Japanese) or in Overseas
 * mode (US or European).
 *
 * \return          true if the system is running in Japanese mode, otherwise it
 *                  is in US/European mode
 */
bool smd_is_japanese(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_SYS_H */
