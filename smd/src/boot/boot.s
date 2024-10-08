/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: boot.s
 * Sega Megadrive/Genesis bootstrap code
 *
 * At boot, the SMD has nothing initialised. We have to set up all pieces to
 * recover a clean state.
 * On top of that, we must do some extra tasks to use the C language.
 *
 * More info:
 * https://blog.bigevilcorporation.co.uk/2012/02/28/sega-megadrive-1-getting-started
 * https://www.namelessalgorithm.com/genesis/blog/genesis/
 */

.section .text.smdboot

.global smd_boot
smd_boot:
    /* Disable interrupts */
    move.w  #0x2700, sr

    /* Check if we are doing a cool or a hoot boot */
    /* If any controller CTRL port is setup, we are doing a hot boot */
    tst.l   0xa10008                /* Test CTRL 1 and 2 at the same time */
    bne.s   skip_ctrl3
    tst.w   0xa1000c                /* Test CTRL 3 */
skip_ctrl3:
    bne.s   hot_boot

    /* We are doing a cool boot, we must do all the initialisation stuff */
cool_boot:

    /* TMSS (Trademark Security System) handshake */
    move.b  0xa10001, d0            /* Check version, TMSS only on model 1+ */
    andi.b  #0x0F, d0               /* Version 0, TMSS not present, skip */
    beq skip_tmss
    move.l  #0x53454741, 0xa14000   /* Write 'SEGA' to TMSS register */
skip_tmss:

    /*
     * Use this section for one time hardware initialisation.
     * Be careful!! Whatever variable initialised here will be erased later in
     * the clear_loop. This section is skipped on reset too.
     */

    /* We are doing a hot boot (reset), some parts are already initialised */
hot_boot:

    /* Clear all work RAM (This includes bss and stack) */
    lea     0xFF0000, a0            /* RAM offset */
    moveq   #0, d0
    move.w  #0x3FFF, d1             /* Counter = 64k / 4 */
clear_loop:
    move.l  d0, (a0)+
    dbra    d1, clear_loop

    /* Copy initialised global and static data from ROM to work RAM */
    lea     _sdata, a0              /* Start of initialised data in ROM */
    lea     0xFF0000,a1             /* First RAM address */
    move.l  #_data_size, d0         /* Size of data to copy in bytes */
    addq.l  #1, d0                  /* Adjust to use words instead of bytes */
    lsr.l   #1, d0
    beq     skip_copy               /* No data to copy */
    subq.w  #1, d0                  /* Adjust to use 0 in dbra */
copy_loop:
    move.w  (a0)+, (a1)+            /* Copy a word of data to RAM */
    dbra    d0, copy_loop
skip_copy:

    /*
     * Use this section for software initialisation.
     * Put here whatever you need before main starts
     */

    /* initialise the Sega Megadrive/Genesis hardware */
    bsr smd_init

    /* Go play with it!! */
    jmp     main
    beq.s   smd_boot               /* main returned, reset */
