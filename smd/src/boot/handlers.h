/* SPDX-License-Identifier: MIT */
/**
 * MDDev development kit
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2021
 * Github: https://github.com/tapule/mddev
 *
 * File: handlers.s
 * Exception and interrupt handlers
 *
 * Response handlers for the most important exceptions and interrupts on the SMD
 */
#ifndef SMD_HANDLERS_H
#define SMD_HANDLERS_H

void smd_exc_bus_error(void);
void smd_exc_address_error(void);
void smd_exc_illegal_instruction(void);
void smd_exc_zero_divide(void);
void smd_exc_chk_instruction(void);
void smd_exc_trapv(void);
void smd_exc_privilege_violation(void);
void smd_exc_unhandled(void);

void smd_int_external(void);
void smd_int_hblank(void);
void smd_int_vblank(void);
void smd_int_unhandled(void);

#endif /* SMD_HANDLERS_H */