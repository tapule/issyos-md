/*
 * SPDX-License-Identifier: MIT
 *
 * This file is part of The Curse of Issyos MegaDrive port.
 * Coded by: Juan Ángel Moreno Fernández (@_tapule) 2025
 * Github: https://github.com/tapule
 */

/**
 * \file            unpack.h
 * \brief           SLZ and ZX0 decompression functions
 */

#include "unpack.h"


/*-----------------------------------------------------------------------------
 _____ _     _____
|  ___| |   |___  |
| |___| |      / /
|___  | |     / /
 ___| | |___ / /__
|_____|_____|_____|

Copyright 2011 Javier Degirolmo (Sik)
sik dot the dot hedgehog at gmail dot com

SLZ is a LZ77-based compression format for Mega Drive homebrew. It serves as
a good general purpose compression scheme, mainly useful for data that needs
to be used during loading times.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void smd_unpack_slz(const uint8_t *in, uint8_t *out) {
   __asm__ __volatile__ (
      "move.b (%0)+,%%d0\n\t"       // Read uncompressed size
      "lsl.w #8,%%d0\n\t"
      "move.b (%0)+,%%d0\n\t"

      "moveq #1,%%d1\n"             // Cause code to fetch new token data
                                    // as soon as it starts
   "1: tst.w %%d0\n\t"              // Did we read all data?
      "beq 5f\n\t"                  // If so, we're done with it!

      "subq.w #1,%%d1\n\t"          // Check if we need more tokens
      "bne.s 2f\n\t"
      "move.b (%0)+,%%d2\n\t"
      "moveq #8,%%d1\n"

   "2: add.b %%d2,%%d2\n\t"         // Get next token type
      "bcc.s 4f\n\t"                // 0 = uncompressed, 1 = compressed

      "move.b (%0)+,%%d3\n\t"       // Compressed? Read string info
      "lsl.w #8,%%d3\n\t"           // %d3 = distance
      "move.b (%0)+,%%d3\n\t"       // %d4 = length
      "move.b %%d3,%%d4\n\t"
      "lsr.w #4,%%d3\n\t"
      "and.w #15,%%d4\n\t"

      "subq.w #3,%%d0\n\t"          // Length is offset by 3
      "sub.w %%d4,%%d0\n\t"         // Now that we know the string length,
                                    // discount it from the amount of data
                                    // to be read

      "addq.w #3,%%d3\n\t"          // Distance is offset by 3
      "neg.w %%d3\n\t"              // Make distance go backwards

      "add.w %%d4,%%d4\n\t"         // Copy bytes using Duff's device
      "add.w %%d4,%%d4\n\t"         // MUCH faster than a loop, due to lack
      "eor.w #60,%%d4\n\t"          // of iteration overhead
      "jmp 3f(%%pc,%%d4.w)\n"
   "3: move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"
      "move.b (%1,%%d3.w),(%1)+\n\t"

      "bra 1b\n"                    // Keep processing data

   "4: move.b (%0)+,(%1)+\n\t"      // Uncompressed? Read as is
      "subq.w #1,%%d0\n\t"          // It's always one byte long
      "bra 1b\n"                    // Keep processing data

   "5:"
      : "=a"(in),"=a"(out) : "0"(in),"1"(out) :
      "d0","d1","d2","d3","d4","memory","cc");
}


/*
   ZX0 decompressor for 68000
   Original version by Emmanuel Marty.
   Ported to C embedded asm by jamf 20250102
*/
void smd_unpack_zx0(const uint8_t *in, uint8_t *out) {
   __asm__ __volatile__ (
      "moveq #-128,%%d1\n\t"        // initialize empty bit queue
                                    // plus bit to roll into carry
      "moveq #-1,%%d2\n"            // initialize rep-offset to 1
// .literals:
   "1: bsr.s 8f\n\t"                // read number of literals to copy
      "subq.l #1,%%d0\n"            // dbf will loop until d0 is -1, not 0
// .copy_lits:
   "2: move.b (%0)+,(%1)+\n\t"      // copy literal byte
      "dbf %%d0,2b\n\t"               // loop for all literal bytes

      "add.b %%d1,%%d1\n\t"         // read 'match or rep-match' bit
      "bcs.s 7f\n"                  // if 1: read offset, if 0: rep-match

// .rep_match:
   "3: bsr.s 8f\n"                  // read match length (starts at 1)
// .do_copy:
   "4: subq.l #1,%%d0\n"            // dbf will loop until d0 is -1, not 0
// .do_copy_offs:
   "5: move.l %1,%%a2\n\t"          // calculate backreference address
      "add.l %%d2,%%a2\n"           // (dest + negative match offset)
// .copy_match:
   "6: move.b (%%a2)+,(%1)+\n\t"    // copy matched byte
      "dbf %%d0,6b\n\t"             // loop for all matched bytes

      "add.b %%d1,%%d1\n\t"         // read 'literal or match' bit
      "bcc.s 1b\n"                  // if 0: go copy literals

// .get_offset:
   "7: moveq #-2,%%d0\n\t"          // initialize value to $fe
      "bsr.s 9f\n\t"                // read high byte of match offset
      "addq.b #1,%%d0\n\t"          // obtain negative offset high byte
      "beq.s 13f\n\t"               // exit if EOD marker
      "move.w %%d0,%%d2\n\t"        // transfer negative high byte into d2
      "lsl.w #8,%%d2\n\t"           // shift it to make room for low byte

      "moveq #1,%%d0\n\t"           // initialize length value to 1
      "move.b (%0)+,%%d2\n\t"       // read low byte of offset + 1 bit of len
      "asr.l #1,%%d2\n\t"           // shift len bit into carry/offset in place
      "bcs.s 5b\n\t"                // if len bit is set, no need for more
      "bsr.s 11f\n\t"               // read rest of elias-encoded match length
      "bra.s 5b\n"                  // go copy match

// .get_elias:
   "8: moveq #1,%%d0\n"             // initialize value to 1
// .elias_loop:
   "9: add.b %%d1,%%d1\n\t"         // shift bit queue, high bit into carry
      "bne.s 10f\n\t"               // queue not empty, bits remain
      "move.b (%0)+,%%d1\n\t"       // read 8 new bits
      "addx.b %%d1,%%d1\n"          // shift bit queue, high bit into carry

// .got_bit:
   "10: bcs.s 12f\n"                // done if control bit is 1
// .elias_bt:
   "11: add.b %%d1,%%d1\n\t"        // read data bit
      "addx.l %%d0,%%d0\n\t"        // shift data bit into value in d0
      "bra.s 9b\n"                  // keep reading

// .got_elias:
   "12: rts\n"
// .done:
   "13:"
      : "=a"(in),"=a"(out) : "0"(in),"1"(out) :
      "a2","d0","d1","d2","memory","cc");
}
