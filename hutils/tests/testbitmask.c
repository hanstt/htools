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
#include <htest/htest.h>

HTEST(DefaultValue)
{
	struct Bitmask *bitmask;

	bitmask = bitmask_create(1);
	HTRY_PTR(NULL, !=, bitmask);

	HTRY_I(0, ==, bitmask_get(bitmask, 0));
	bitmask_set(bitmask, 0);
	HTRY_I(1, ==, bitmask_get(bitmask, 0));
	bitmask_clear(bitmask, 0);
	HTRY_I(0, ==, bitmask_get(bitmask, 0));

	bitmask_free(&bitmask);
	HTRY_PTR(NULL, ==, bitmask);
}

HTEST(ZeroSizeFails)
{
	HTRY_SIGNAL(bitmask_create(0));
}

HTEST(OutOfBounds)
{
	struct Bitmask *bitmask;

	bitmask = bitmask_create(1);
	HTRY_SIGNAL(bitmask_clear(bitmask, -1));
	HTRY_SIGNAL(bitmask_clear(bitmask, 1));
	HTRY_SIGNAL(bitmask_get(bitmask, -1));
	HTRY_SIGNAL(bitmask_get(bitmask, 1));
	HTRY_SIGNAL(bitmask_set(bitmask, -1));
	HTRY_SIGNAL(bitmask_set(bitmask, 1));
	bitmask_clear(bitmask, 0);
	bitmask_get(bitmask, 0);
	bitmask_set(bitmask, 0);
}

HTEST(OneAll)
{
	struct Bitmask *bitmask;
	int i;

	bitmask = bitmask_create(100);

	for (i = 0; 100 > i; ++i) {
		HTRY_I(0, ==, bitmask_get(bitmask, i));
	}
	bitmask_one(bitmask);
	for (i = 0; 100 > i; ++i) {
		HTRY_I(0, !=, bitmask_get(bitmask, i));
	}

	bitmask_free(&bitmask);
}

HTEST(ZeroAll)
{
	struct Bitmask *bitmask;
	int i;

	bitmask = bitmask_create(100);

	for (i = 0; 100 > i; ++i) {
		bitmask_set(bitmask, i);
	}
	for (i = 0; 100 > i; ++i) {
		HTRY_I(0, !=, bitmask_get(bitmask, i));
	}
	bitmask_zero(bitmask);
	for (i = 0; 100 > i; ++i) {
		HTRY_I(0, ==, bitmask_get(bitmask, i));
	}

	bitmask_free(&bitmask);
}

HTEST_SUITE(Bitmask)
{
	HTEST_ADD(DefaultValue);
	HTEST_ADD(ZeroSizeFails);
	HTEST_ADD(OutOfBounds);
	HTEST_ADD(OneAll);
	HTEST_ADD(ZeroAll);
}