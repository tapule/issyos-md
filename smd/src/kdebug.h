/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (\_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            kdebug.h
 * \brief           Gens KMod Debugging routines
 *
 * Gens KMod is a modified version developed by Kaneda of Gens emulator by
 * Stephane Dallongeville. Among other improvements KMod adds some debugging
 * features. These features are also supported by Blastem emulator.
 *
 * More info:
 * https://documentation.help/Gens-KMod/documentation.pdf
 */

#ifndef SMD_KDEBUG_H
#define SMD_KDEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Debugging disabled so do not evaluate kdebug functions. */
#ifdef NDEBUG

#define smd_kdebug_halt() ((void)0)
#define smd_kdebug_alert(x) ((void)0)
#define smd_kdebug_timer_start() ((void)0)
#define smd_kdebug_timer_stop() ((void)0)
#define smd_kdebug_timer_output() ((void)0)

#else

#define smd_kdebug_halt() __smd_kdebug_halt()
#define smd_kdebug_alert(x) __smd_kdebug_alert(x)
#define smd_kdebug_timer_start() __smd_kdebug_timer_start()
#define smd_kdebug_timer_stop() __smd_kdebug_timer_stop()
#define smd_kdebug_timer_output() __smd_kdebug_timer_output()

/**
 * \brief           Pause rom emulation
 */
void __smd_kdebug_halt(void);

/**
 * \brief           Output a message string on the emulator's Message window
 * \param[in]       str: Text string to output
 */
void __smd_kdebug_alert(const char *restrict str);

/**
 * \brief           Start an internal emulator timer counter based on m68k cycles
 */
void __smd_kdebug_timer_start(void);

/**
 * \brief           Stop the internal emulator timer and output its value
 */
void __smd_kdebug_timer_stop(void);

/**
 * \brief           Output current internal emulator timer value
 */
void __smd_kdebug_timer_output(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SMD_KDEBUG_H */
