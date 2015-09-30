/*
 * Copyright (c) 2014, 2015
 * Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
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

#ifndef HUTILS_MEMORY_H
#define HUTILS_MEMORY_H

#include <stdlib.h>
#include <hutils/macros.h>

#define CALLOC(ptr, num) do {\
	ptr = calloc(num, sizeof *ptr);\
	if (NULL == ptr) {\
		err(EXIT_FAILURE, "calloc");\
	}\
} HUTILS_COND(while, 0)
#define MALLOC(ptr, size) do {\
	ptr = malloc(size);\
	if (NULL == ptr) {\
		err(EXIT_FAILURE, "malloc");\
	}\
} HUTILS_COND(while, 0)
#define FREE(ptr) do {\
	if (NULL != ptr) {\
		free(ptr);\
		ptr = NULL;\
	}\
} HUTILS_COND(while, 0)

#endif
