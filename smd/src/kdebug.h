/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
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

/**
 * \brief           Stringify utility macros
 */
#define smd_kdebug_stringify(x) #x
#define smd_kdebug_to_string(x) smd_kdebug_stringify(x)

/* Debugging disabled so do not evaluate kdebug functions. */
#ifdef NDEBUG

#define smd_kdebug_halt()                ((void) 0)
#define smd_kdebug_alert(x)              ((void) 0)
#define smd_kdebug_timer_start()         ((void) 0)
#define smd_kdebug_timer_stop()          ((void) 0)
#define smd_kdebug_timer_output()        ((void) 0)
#define smd_kdebug_warning_if(test, msg) ((void) 0)
#define smd_kdebug_error_if(test, msg)   ((void) 0)

#else

#define smd_kdebug_halt()         smd_kdebug_halt_imp()
#define smd_kdebug_alert(x)       smd_kdebug_alert_imp(x)
#define smd_kdebug_timer_start()  smd_kdebug_timer_start_imp()
#define smd_kdebug_timer_stop()   smd_kdebug_timer_stop_imp()
#define smd_kdebug_timer_output() smd_kdebug_timer_output_imp()
/* CHECKME: Should I move this macros to a unit like utils.h or similar? */
#define smd_kdebug_warning_if(test, msg)                                                                               \
    if (test) {                                                                                                        \
        smd_kdebug_alert(                                                                                              \
            "Warning at " smd_kdebug_to_string(__FILE__) " line " smd_kdebug_to_string(__LINE__) ": " msg);            \
    }
#define smd_kdebug_error_if(test, msg)                                                                                 \
    if (test) {                                                                                                        \
        smd_kdebug_alert("Error at " smd_kdebug_to_string(__FILE__) " line " smd_kdebug_to_string(__LINE__) ": " msg); \
        while (true) {}                                                                                                \
    }

/**
 * \brief           Pause rom emulation
 */
void smd_kdebug_halt_imp(void);

/**
 * \brief           Output a message string on the emulator's Message window
 * \param[in]       str: Text string to output
 */
void smd_kdebug_alert_imp(const char *restrict str);

/**
 * \brief           Start an internal emulator timer counter based on m68k cycles
 */
void smd_kdebug_timer_start_imp(void);

/**
 * \brief           Stop the internal emulator timer and output its value
 */
void smd_kdebug_timer_stop_imp(void);

/**
 * \brief           Output current internal emulator timer value
 */
void smd_kdebug_timer_output_imp(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SMD_KDEBUG_H */
