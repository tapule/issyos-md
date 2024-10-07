/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule/mddev
 *
 * File: vectors.c
 * Sega Megadrive/Genesis m68k vector table
 *
 * All SMD m68k programs must have this vector table at their very beginning.
 * Each entry (32bit address) is used by the m68k in concrete cases.
 *
 * More info:
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 * https://www.namelessalgorithm.com/genesis/blog/genesis/
 */
#include "boot.h"
#include "handlers.h"

[[gnu::section(".text.smdvectors")]]
static void (*const volatile smd_vectors[64])(void) = {
    0x00000000,                 /* Initial stack ptr value (0 - 1 = ffffffff) */
    smd_boot,                   /* Program entry point */

    /* Hardware exception vectors */
    smd_exc_bus_error,              /* Bus error */
    smd_exc_address_error,          /* Address error */
    smd_exc_illegal_instruction,    /* Illegal instruction */
    smd_exc_zero_divide,            /* Division by zero */
    smd_exc_chk_instruction,        /* CHK CPUsmd_Exception */
    smd_exc_trapv,                  /* TRAPV CPUsmd_Exception */
    smd_exc_privilege_violation,    /* Privilege violation */
    smd_exc_unhandled,              /* TRACE exception */
    smd_exc_unhandled,              /* Line-A emulator */
    smd_exc_unhandled,              /* Line-F emulator */
    /* Unused exceptions (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    smd_exc_unhandled,              /* Unused (reserved) */
    /* Hardware Interrupts */
    smd_int_unhandled,              /* Spurious interrupt */
    smd_int_unhandled,              /* IRQ level 1 */
    smd_int_external,               /* IRQ level 2 (External interrupt) */
    smd_int_unhandled,              /* IRQ level 3 */
    smd_int_hblank,                 /* IRQ level 4 (H retrace interrupt) */
    smd_int_unhandled,              /* IRQ level 5 */
    smd_int_vblank,                 /* IRQ level 6 (V retrace interrupt) */
    smd_int_unhandled,              /* IRQ level 7 */
    /* Trap exceptions vectors */
    smd_exc_unhandled,              /* TRAP #00 exception */
    smd_exc_unhandled,              /* TRAP #01 exception */
    smd_exc_unhandled,              /* TRAP #02 exception */
    smd_exc_unhandled,              /* TRAP #03 exception */
    smd_exc_unhandled,              /* TRAP #04 exception */
    smd_exc_unhandled,              /* TRAP #05 exception */
    smd_exc_unhandled,              /* TRAP #06 exception */
    smd_exc_unhandled,              /* TRAP #07 exception */
    smd_exc_unhandled,              /* TRAP #08 exception */
    smd_exc_unhandled,              /* TRAP #09 exception */
    smd_exc_unhandled,              /* TRAP #10 exception */
    smd_exc_unhandled,              /* TRAP #11 exception */
    smd_exc_unhandled,              /* TRAP #12 exception */
    smd_exc_unhandled,              /* TRAP #13 exception */
    smd_exc_unhandled,              /* TRAP #14 exception */
    smd_exc_unhandled,              /* TRAP #15 exception */
    /* Unused interrupt vectors (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled,              /* Unused (reserved) */
    smd_int_unhandled               /* Unused (reserved) */
                                    /* ------------------------ */
                                    /* 256B  total */
};
