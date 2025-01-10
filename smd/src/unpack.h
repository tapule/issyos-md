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
 *
 * SLZ is a LZ77-based compression format for Mega Drive homebrew by Sik.
 * ZX0 is a custom LZ77/LZSS based compression format, that provides a tradeoff
 * between high compression ratio, and extremely simple fast decompression by
 * Einar Saukas. The decompression rutine is a port from the ZX0 decompressor
 * for 68000 by Emmanuel Marty.
 *
 * More info:
 * https://github.com/sikthehedgehog/mdtools/tree/master/slz
 * https://github.com/einar-saukas/ZX0
 * https://github.com/emmanuel-marty/salvador
 */

#ifndef SMD_UNPACK_H
#define SMD_UNPACK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief           Decompress a SLZ stream
 * \param[in]       in: SLZ compressed data buffer
 * \param[in]       out: Destination buffer for decompressed data
 * \pre             out buffer must have enought space to hold the uncompressed
 *                  data
 */
void smd_unpack_slz(const uint8_t *in, uint8_t *out);

/**
 * \brief           Decompress a ZX0 stream
 * \param[in]       in: ZX0 compressed data buffer
 * \param[in]       out: Destination buffer for decompressed data
 * \pre             out buffer must have enought space to hold the uncompressed
 *                  data
 */
void smd_unpack_zx0(const uint8_t *in, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif /* SMD_UNPACK_H */
