/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            handlers.h
 * \brief           Response handlers for exceptions and interrupts on the SMD
 */
#ifndef SMD_HANDLERS_H
#define SMD_HANDLERS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Handler for bus error exception
 */
void smd_exc_bus_error(void);

/**
 * \brief           Handler for address error exception
 */
void smd_exc_address_error(void);

/**
 * \brief           Handler for illegal instruction exception
 */
void smd_exc_illegal_instruction(void);

/**
 * \brief           Handler for divide by zero exception
 */
void smd_exc_zero_divide(void);

/**
 * \brief           Handler for chk m68k instruction exception
 */
void smd_exc_chk_instruction(void);

/**
 * \brief           Handler for trapv m68k instruction exception
 */
void smd_exc_trapv_instruction(void);

/**
 * \brief           Handler for privileged instruction used exception
 */
void smd_exc_privilege_violation(void);

/**
 * \brief           Generic handler for unhandled exceptions
 */
void smd_exc_unhandled(void);


void smd_int_external(void);
void smd_int_hblank(void);
void smd_int_vblank(void);
void smd_int_unhandled(void);

#ifdef __cplusplus
}
#endif

#endif /* SMD_HANDLERS_H */
