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
 */

#include "string.h"

/**
 * \brief           LUT storing two decimal representation in range [0..99]
 */
static const char smd_str_lut_two_digit[100][2] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
    "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
    "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
    "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
    "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99"
};

/**
 * \brief           LUT storing the power of 10 values used to calculate the
 *                  length in digits of a 32bit value
 */
static const uint32_t smd_str_lut_ten_pow[10] = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000
};

/**
 * \brief           Count the number of decimal digits in an unsigned 32b value
 * \param[in]       val: Value to calculate its length in decimal digits
 * \return          Number of decimal digits
 * \note            Assume that the representation of 0 has length of 1 digit
 */
static uint16_t
smd_str_digits_count(const uint32_t val) {
    uint16_t len;

    /* All numbers have at least lenght of 1, included 0 */
    len = 1;
    /* Max uint32_t value is 4,294,967,295 so max number of digits is 10 */
    if (val >= smd_str_lut_ten_pow[9]) {
        return 10;
    }

    while (val >= smd_str_lut_ten_pow[len]) {
        ++len;
    }
    return len;
}

uint16_t
smd_str_len(const char *str) {
    const char *c = str;

    while (*c != '\0') {
        ++c;
    }
    return (c - str);
}

char *
smd_str_cat(char *restrict dest, const char *restrict src) {
    char *ret = dest;

    /* Move to the end of the first string */
    while (*dest != '\0') {
        ++dest;
    }

    /* Catenate second string */
    while (*src != '\0') {
        *dest = *src;
        ++dest;
        ++src;
    }
    return ret;
}

uint16_t
smd_str_from_uint(uint32_t val, char *restrict dest, uint16_t min_len) {
    uint16_t len;
    uint16_t i;
    uint16_t digits;

    len = smd_str_digits_count(val);
    i = 0;

    /* Check if we need to pad with 0 */
    if (len < min_len) {
        for (i = 0; i < min_len - len; ++i) {
            dest[i] = '0';
        }
    }
    /*
        Implements the "LR printer" algorithm from Tigran Hayrapetyan with the 2
        digits optimization as described here:
        https://towardsdatascience.com/34-faster-integer-to-string-conversion-algorithm-c72453d25352
    */
    while (len > 2) {
        /* Get the two digits on the left */
        digits = val / smd_str_lut_ten_pow[len - 2];
        /* Put them in place using the lut */
        dest[i] = smd_str_lut_two_digit[digits][0];
        dest[i + 1] = smd_str_lut_two_digit[digits][1];
        /* Remove the digits just processed */
        val = val - (digits * smd_str_lut_ten_pow[len - 2]);
        len = len - 2;
        i = i + 2;
    }
    /* Parse right most 2 digits */
    if (len == 2) {
        /* val has directly the two digits to process */
        dest[i] = smd_str_lut_two_digit[val][0];
        dest[i + 1] = smd_str_lut_two_digit[val][1];
        i = i + 2;
    } else {
        /* Only one digit left, no need to use the lut */
        dest[i] = '0' + val;
        ++i;
    }
    dest[i] = '\0';
    return i;
}

uint16_t
smd_str_from_int(int32_t val, char *restrict dest, uint16_t min_len) {
    if (val < 0) {
        dest[0] = '-';
        return smd_str_from_uint(-val, dest + 1, min_len) + 1;
    } else {
        return smd_str_from_uint(val, dest, min_len);
    }
}

uint16_t
smd_str_from_hex(int32_t val, char *restrict dest) {
    /* 32bits value = 8 hex chars */
    for (int8_t i = 7; i >= 0; --i) {
        int16_t digit;
        char ch;

        /* Get the digit on the right */
        digit = (val & 0x0F);
        ch = digit + '0';
        /* Check if it is in the range [A..F] */
        if (ch > '9') {
            ch += 7;    /* 'A' is 7 chars from '9' */
        }
        dest[i] = ch;
        /* Move to next hex digit */
        val = val >> 4;
    }
    dest[8] = '\0';
    return 8;
}
