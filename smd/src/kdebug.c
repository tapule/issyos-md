/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            kdebug.c
 * \brief           Gens KMod Debugging routines
 */

#include "kdebug.h"
#include "vdp.h"

#define SMD_KMOD_CONTROL_PAUSE_CMD (0x00)
#define SMD_KMOD_TIMER_START_CMD   (0x80)
#define SMD_KMOD_TIMER_STOP_CMD    (0x40)
#define SMD_KMOD_TIMER_OUTPUT_CMD  (0x00)

inline void
__smd_kdebug_halt(void) {
    /* Pause game command */
    *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_CONTROL | SMD_KMOD_CONTROL_PAUSE_CMD;
}

void
__smd_kdebug_alert(const char *restrict str) {
    /* We need to write string byte by byte */
    while (*str) {
        *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_MESSAGE | *str;
        ++str;
    }
    /* Session ends with a 0 */
    *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_MESSAGE | 0x00;
}

inline void
__smd_kdebug_timer_start(void) {
    /* Start emulator timer command */
    *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_TIMER | SMD_KMOD_TIMER_START_CMD;
}

inline void
__smd_kdebug_timer_stop(void) {
    /* Stop and output emulator timer command */
    *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_TIMER | SMD_KMOD_TIMER_STOP_CMD;
}

inline void
__smd_kdebug_timer_output(void) {
    /* Output emulator timer command */
    *SMD_VDP_PORT_CTRL_W = SMD_VDP_REG_KMOD_TIMER | SMD_KMOD_TIMER_OUTPUT_CMD;
}
