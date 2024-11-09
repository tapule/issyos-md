/*
 * SPDX-License-Identifier: [TIPO_LICENCIA]
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            null_data.c
 * \brief           256 zero bytes of data usable for any purpose
 */

#include "null_data.h"

alignas(256) const uint8_t smd_null_data[NULL_DATA_SIZE] = {0};
