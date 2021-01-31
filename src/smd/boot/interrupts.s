/* SPDX-License-Identifier: MIT */
/**
 * The Curse of Issyos MegaDrive port
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021 
 * Github: https://github.com/tapule/issyos-md
 *
 * File: interrupts.s
 * Interrupt handlers
 * 
 * Response handlers for the most important interrupts on SMD
 */

 .section .text

.global _int_external
 _int_external:
    rte

.global _int_hblank
 _int_hblank:
    rte

.global _int_vblank
 _int_vblank:
    rte

.global _int_unhandled
 _int_unhandled:
    rte
