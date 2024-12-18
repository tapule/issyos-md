/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            sys.c
 * \brief           System core routines to manage the Sega Megadrive/Genesis hardware
 */

#include "sys.h"
#include "mem_map.h"
#include "handlers.h"
#include "dma.h"
#include "pad.h"
#include "pal.h"
#include "psg.h"
#include "rand.h"
#include "sprite.h"
#include "vdp.h"
#include "xgm.h"
#include "ym2612.h"
#include "z80.h"

#define SMD_VERSION_PORT_MOD_FLAG   (0b10000000)
#define SMD_VERSION_PORT_VMOD_FLAG  (0b01000000)

#if SMD_SYS_HEADER_SRAM_ENABLED > 0
    #define _SYS_HEADER_SRAM_ENABLED "RA"
#else
    #define _SYS_HEADER_SRAM_ENABLED "  "
#endif


extern int main(void);

/**
 * \brief           Current status of interrups (true: enabled, false: disabled)
 */
static bool smd_sys_ints_status_flag;

static inline void smd_sys_boot(void);

/**
 * \brief           Sega Megadrive/Genesis rom header
 */
[[gnu::section(".text.smdheader")]]
const smd_sys_header_t smd_sys_header = {SMD_SYS_HEADER_SYS_NAME,
                                 SMD_SYS_HEADER_COPYRIGHT,
                                 SMD_SYS_HEADER_DOMESTIC_NAME,
                                 SMD_SYS_HEADER_OVERSEAS_NAME,
                                 SMD_SYS_HEADER_SERIAL,
                                 0,
                                 SMD_SYS_HEADER_DEVICES,
                                 0x00000000,
                                 SMD_SYS_HEADER_ROM_SIZE - 1,
                                 0x00FF0000,
                                 0x00FFFFFF,
                                 _SYS_HEADER_SRAM_ENABLED,
                                 0xF820,
                                 0x200001,
                                 0x20FFFF,
                                 "            ",
                                 SMD_SYS_HEADER_NOTES,
                                 SMD_SYS_HEADER_REGION,
                                 "             "};


/**
 * \brief           Sega Megadrive/Genesis m68k vector table
 *
 * All SMD m68k programs must have this vector table at their very beginning.
 * Each entry (32bit address) is used by the m68k in concrete cases.
 *
 * More info:
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 * https://www.namelessalgorithm.com/genesis/blog/genesis/
 */
[[gnu::section(".text.smdvectors")]]
static void (*const volatile smd_sys_vector_table[64])(void) = {
    0x00000000,                     /**< Initial stack ptr value (0 - 1 = ffffffff) */
    smd_sys_boot,                   /**< Initial program counter value (program entry point) */

    /* Hardware exception vectors */
    smd_exc_bus_error,              /**< Bus error */
    smd_exc_address_error,          /**< Address error */
    smd_exc_illegal_instruction,    /**< Illegal instruction */
    smd_exc_zero_divide,            /**< Division by zero */
    smd_exc_chk_instruction,        /**< CHK instruction (out of bounds) exception */
    smd_exc_trapv_instruction,      /**< TRAPV instruction exception */
    smd_exc_privilege_violation,    /**< Privilege violation */
    smd_exc_unhandled,              /**< TRACE exception */
    smd_exc_unhandled,              /**< Line-A emulator */
    smd_exc_unhandled,              /**< Line-F emulator */
    /* Unused exceptions (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    smd_exc_unhandled,              /**< Unused (reserved) */
    /* Hardware Interrupts */
    smd_int_unhandled,              /**< Spurious interrupt */
    smd_int_unhandled,              /**< Level 1 interrupt autovector */
    smd_int_external,               /**< Level 2 interrupt autovector (External interrupt) */
    smd_int_unhandled,              /**< Level 3 interrupt autovector */
    smd_int_hblank,                 /**< Level 4 interrupt autovector (H retrace interrupt) */
    smd_int_unhandled,              /**< Level 5 interrupt autovector */
    smd_int_vblank,                 /**< Level 6 interrupt autovector (V retrace interrupt) */
    smd_int_unhandled,              /**< Level 7 interrupt autovector */
    /* Trap exceptions vectors */
    smd_exc_unhandled,              /**< TRAP #00 exception */
    smd_exc_unhandled,              /**< TRAP #01 exception */
    smd_exc_unhandled,              /**< TRAP #02 exception */
    smd_exc_unhandled,              /**< TRAP #03 exception */
    smd_exc_unhandled,              /**< TRAP #04 exception */
    smd_exc_unhandled,              /**< TRAP #05 exception */
    smd_exc_unhandled,              /**< TRAP #06 exception */
    smd_exc_unhandled,              /**< TRAP #07 exception */
    smd_exc_unhandled,              /**< TRAP #08 exception */
    smd_exc_unhandled,              /**< TRAP #09 exception */
    smd_exc_unhandled,              /**< TRAP #10 exception */
    smd_exc_unhandled,              /**< TRAP #11 exception */
    smd_exc_unhandled,              /**< TRAP #12 exception */
    smd_exc_unhandled,              /**< TRAP #13 exception */
    smd_exc_unhandled,              /**< TRAP #14 exception */
    smd_exc_unhandled,              /**< TRAP #15 exception */
    /* Unused interrupt vectors (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled,              /**< Unused (reserved) */
    smd_int_unhandled               /**< Unused (reserved) */
                                    /*   ------------------------ */
                                    /*   256B  total */
};

/**
 * \brief           Sega Megadrive/Genesis bootstrap code
 *
 * At boot, the SMD has nothing initialised. We have to set up all pieces to
 * recover a clean state. Additionally, we must do some extra tasks to use the C
 * language.
 *
 * More info:
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 * https://www.namelessalgorithm.com/genesis/blog/genesis/
 */
[[gnu::interrupt, gnu::section(".text.smdboot"), noreturn]]
static inline void
smd_sys_boot(void) {
    extern uint32_t _text_size;
    extern uint32_t _data_size;
    uint16_t *ram_addr = (uint16_t *) SMD_RAM_ADDRESS;

    /* Disable interrupts and set Supervisor bit */
    __asm__ volatile("\tmov.w	#0x2700, %sr\n");

    /*
     * Check if we are doing a cool or a hot boot
     * If any controller CTRL port is setup, we are doing a hot boot
     */
    if (*SMD_PAD_1_CTRL_PORT == 0 && *SMD_PAD_2_CTRL_PORT == 0 && SMD_PAD_EXP_CTRL_PORT == 0) {
        /* We are doing a cool boot, we must do all the initialisation stuff */

        /* TMSS (Trademark Security System) handshake */
        uint8_t md_ver = *SMD_VERSION_PORT & 0x0F;

        /* Check version, TMSS only on model 1+ */
        if (md_ver != 0) {
            /* Write 'SEGA' to TMSS register */
            *SMD_TMSS_PORT = 0x53454741;
        }

        /*
        * Use this section for one time hardware initialisation.
        * Be careful!! Whatever variable initialised here will be erased later
        * in the RAM clear step. This section is skipped on reset too.
        */
    }

    /*
     * Clear all work RAM (This includes bss and stack)
     * It must be done here instead of calling a function because the function
     * stack frame data would be cleared.
     */
    for (uint16_t i = 0; i < (32 * 1024); ++i) {
        *ram_addr = 0;
        ++ram_addr;
    }

    /* Copy initialised global and static data from ROM to work RAM */
    {
        /* Start of initialised data in ROM */
        uint16_t *data_addr = (uint16_t *) &_text_size;
        /* Size of data to copy in bytes */
        uint16_t data_size = (uint16_t) ((uint32_t) (&_data_size));

        ram_addr = (uint16_t *) SMD_RAM_ADDRESS;
        /* Adjust to use words instead of bytes */
        data_size = (data_size + 1) / 2;
        while (data_size) {
            *ram_addr = *data_addr;
            ++ram_addr;
            ++data_addr;
            --data_size;
        }
    }

    /*
     * Sega Megadrive/Genesis sub-systems initialization
     * Put here whatever you need to initialize before main runs
     */
    {
        /* Initialize the z80 secondary CPU */
        smd_z80_init();
        /* Initialize gamepad ports */
        smd_pad_init();
        /* Initialize the PSG (Programmable Sound Generator) */
        smd_psg_init();
        /* Initialize the Yamaha YM2612 FM sound chip */
        smd_ym2612_init();
        /* Initialize the XGM based sound engine */
        smd_xgm_init();
        /* Initialize the graphics  */
        smd_vdp_init();
        /* Initialize the pseudo-random number generator */
        smd_rnd_init();
        /* Initialize the DMA system  */
        smd_dma_init();
        /* Initialize the palette system  */
        smd_pal_init();
        /* Initialize the sprite system  */
        smd_spr_init();
    }

    /* Go play with it!! */
    main();

    /* noreturn stuff */
    while (true) {}
}

inline void
smd_sys_ints_enable(void) {
    /*
     * The three least significant bits of the high byte of sr (Status register)
     * are used as interrupt mask:
     *  |T|0|S|0|0|I2|I1|I0|
     *  - T: Trace mode. When set, the processor generates the Trace exception
     *      after each instruction.
     *  - S: Supervisor / user state (Must be set when programming for the MD)
     *  - I2-I0: Interrupt priority mask. Allow interrupts levels higher that
     *      the value set in them. A value of 0b010 will allow only interrupts
     *      higher that level 2 (3 to 7) for example.
     */
    __asm__ volatile("\tandi.w	#0xF8FF, %sr\n");
    smd_sys_ints_status_flag = true;
}

inline void
smd_sys_ints_disable(void) {
    __asm__ volatile("\tori.w	#0x700, %sr\n");
    smd_sys_ints_status_flag = false;
}

inline bool
smd_sys_ints_status(void) {
    return smd_sys_ints_status_flag;
}

inline bool
smd_sys_is_pal(void) {
    return *SMD_VERSION_PORT & SMD_VERSION_PORT_VMOD_FLAG;
}

inline bool
smd_sys_is_japanese(void) {
    return !(*SMD_VERSION_PORT & SMD_VERSION_PORT_MOD_FLAG);
}
