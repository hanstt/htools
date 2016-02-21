/*
 * Copyright (c) 2016 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HUTILS_STDINT_H
#define HUTILS_STDINT_H

#include <hconf/include/hutils/stdint.h>

#if defined(HCONF_STDINT_STDINT_H)
/* LIBS=dont */
# include <stdint.h>
#elif defined(HCONF_STDINT_SYS_TYPES_H)
/* LIBS=dont */
# include <sys/types.h>
typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;
typedef u_int32_t uint32_t;
typedef u_int64_t uint64_t;
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
#else
# error Not hconf:ed.
#endif

#define HCONF_TEST \
	char pi8[1 == sizeof(int8_t) ? 1 : -1];\
	char pu8[1 == sizeof(uint8_t) ? 1 : -1];\
	char pi16[2 == sizeof(int16_t) ? 1 : -1];\
	char pu16[2 == sizeof(uint16_t) ? 1 : -1];\
	char pi32[4 == sizeof(int32_t) ? 1 : -1];\
	char pu32[4 == sizeof(uint32_t) ? 1 : -1];\
	char pi64[8 == sizeof(int64_t) ? 1 : -1];\
	char pu64[8 == sizeof(uint64_t) ? 1 : -1];\
	char pip[sizeof(void *) == sizeof(intptr_t) ? 1 : -1];\
	char pup[sizeof(void *) == sizeof(uintptr_t) ? 1 : -1];\
	(void)pi8;\
	(void)pu8;\
	(void)pi16;\
	(void)pu16;\
	(void)pi32;\
	(void)pu32;\
	(void)pi64;\
	(void)pu64;\
	(void)pip;\
	(void)pup

#endif
