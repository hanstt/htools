/*
 * Copyright (c) 2015 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
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

#include <hutils/bitmask.h>
#include <stdint.h>
#include <hutils/memory.h>

#define NUM_UINT8(bit_num)  ((bit_num + 7) / 8)
#define NUM_UINT32(bit_num) ((bit_num + 31) / 32)

struct Bitmask {
	int	bit_num;
	uint32_t	*data;
};

struct Bitmask *
bitmask_copy(struct Bitmask const *const a_src)
{
	struct Bitmask *dst;

	dst = bitmask_create(a_src->bit_num);
	memmove(dst->data, a_src->data, NUM_UINT8(a_src->bit_num));
	return dst;
}

void
bitmask_copy_mask(struct Bitmask *const a_dst, struct Bitmask const *const
    a_src)
{
	if (a_dst->bit_num != a_src->bit_num) {
		abort();
	}
	memmove(a_dst->data, a_src->data, NUM_UINT8(a_src->bit_num));
}

struct Bitmask *
bitmask_create(int const a_bit_num)
{
	struct Bitmask *bm;

	if (0 >= a_bit_num) {
		abort();
	}
	CALLOC(bm, 1);
	bm->bit_num = a_bit_num;
	CALLOC(bm->data, NUM_UINT32(a_bit_num));
	return bm;
}

void
bitmask_free(struct Bitmask **const a_bm)
{
	struct Bitmask *bm;

	bm = *a_bm;
	if (NULL == bm) {
		return;
	}
	FREE(bm->data);
	FREE(*a_bm);
}

int
bitmask_get(struct Bitmask *const a_bm, int const a_index)
{
	if (0 > a_index || a_bm->bit_num <= a_index) {
		abort();
	}
	return (1 << (31 & a_index)) & a_bm->data[a_index / 32];
}

void
bitmask_one(struct Bitmask *const a_bm)
{
	memset(a_bm->data, 0xff, NUM_UINT8(a_bm->bit_num));
}

void
bitmask_set(struct Bitmask *const a_bm, int const a_index)
{
	if (0 > a_index || a_bm->bit_num <= a_index) {
		abort();
	}
	a_bm->data[a_index / 32] |= (1 << (31 & a_index));
}

void
bitmask_unset(struct Bitmask *const a_bm, int const a_index)
{
	if (0 > a_index || a_bm->bit_num <= a_index) {
		abort();
	}
	a_bm->data[a_index / 32] &= ~(1 << (31 & a_index));
}

void
bitmask_zero(struct Bitmask *const a_bm)
{
	memset(a_bm->data, 0, NUM_UINT8(a_bm->bit_num));
}
