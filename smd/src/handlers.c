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

#include "handlers.h"
#include "xgm.h"
#include "vdp.h"

[[gnu::interrupt]]
void smd_exc_bus_error(void)
{}

[[gnu::interrupt]]
void smd_exc_address_error(void)
{}

[[gnu::interrupt]]
void smd_exc_illegal_instruction(void)
{}

[[gnu::interrupt]]
void smd_exc_zero_divide(void)
{}

[[gnu::interrupt]]
void smd_exc_chk_instruction(void)
{}

[[gnu::interrupt]]
void smd_exc_trapv_instruction (void)
{}

[[gnu::interrupt]]
void smd_exc_privilege_violation(void)
{}

[[gnu::interrupt]]
void smd_exc_unhandled(void)
{}

[[gnu::interrupt]]
void smd_int_external(void)
{}

[[gnu::interrupt]]
void smd_int_hblank(void)
{}

[[gnu::interrupt]]
void smd_int_vblank(void)
{
    smd_xgm_update();
    smd_vdp_vblank_flag = 1;
    ++smd_int_counter;
}

[[gnu::interrupt]]
void smd_int_unhandled(void)
{}
