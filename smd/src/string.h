/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2024
 * Github: https://github.com/tapule
 */

/**
 * \file            string.h
 * \brief           String manipulation functions
 *
 * More info:
 * We use the "LR printer" algorithm from Tigran Hayrapetyan with the 2 digits
 * optimization to convert form uint32 to string. It is described here:
 * https://towardsdatascience.com/34-faster-integer-to-string-conversion-algorithm-c72453d25352
 */

#ifndef SMD_STRING_H
#define SMD_STRING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Calculate the length of a string
 * \param[in]       str: Null terminated string to get its length
 * \return          Length of the string not counting the final '\0'
 */
uint16_t smd_str_len(const char *str);

/**
 * \brief           Catenate two strings
 * \param[in,out]   dest: Destination string where catenate src at the end
 * \param[in]       src: String to catenate at the end of dest
 * \return          Pointer to the resulting catenated string
 * \pre             dest string must have enought space to hold the resulting
 *                  catenated string
 */
char *smd_str_cat(char *restrict dest, const char *restrict src);

/**
 * \brief           Convert an unsigned 32b value to its decimal representantion
 * \param[in]       val: Value to convert to decimal representation
 * \param[in,out]   dest: Destination buffer to print the result on
 * \param[in]       min_len: Minimum length that must have the resulting string.
 *                  The string will be padded with 0 if needed.
 * \return          Lenght of the resulting string
 * \pre             dest string must have enought space to hold the resulting
 *                  converted string
 */
uint16_t smd_str_from_uint(uint32_t val, char *restrict dest, uint16_t min_len);

/**
 * \brief           Convert a signed 32b value to its decimal representantion
 * \param[in]       val: Value to convert to decimal representation
 * \param[in,out]   dest: Destination buffer to print the result on
 * \param[in]       min_len: Minimum length that must have the resulting string.
 *                  The string will be padded with 0 if needed.
 * \return          Lenght of the resulting string
 * \pre             dest string must have enought space to hold the resulting
 *                  converted string
 */
uint16_t smd_str_from_int(int32_t val, char *restrict dest, uint16_t min_len);

/**
 * \brief           Convert a signed 32b value to its hexadecimal representantion
 * \param[in]       val: Value to convert to hexadecimal representation
 * \param[in,out]   dest: Destination buffer to print the result on
 * \return          Lenght of the resulting string
 * \pre             dest string must have enought space to hold the resulting
 *                  converted string
 */
uint16_t smd_str_from_hex(int32_t val, char *restrict dest);

/*
      Some more functions to explore
      str_from_fix
      -----------
      str_cmp
      str_cpy
      str_ncpy
*/

#ifdef __cplusplus
}
#endif

#endif /* SMD_STRING_H */
