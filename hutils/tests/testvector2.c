/*
 * Copyright (c) 2015-2016 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
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

#include <hutils/vector2.h>
#include <math.h>
#include <htest/htest.h>

HTEST(DotAxes)
{
	struct Vector2f const c_x = {1.0f, 0.0f};
	struct Vector2f const c_y = {0.0f, 1.0f};

	HTRY_FLT(1.0f, ==, vector2f_dot(&c_x, &c_x));
	HTRY_FLT(0.0f, ==, vector2f_dot(&c_x, &c_y));
	HTRY_FLT(1.0f, ==, vector2f_dot(&c_y, &c_y));
}

HTEST(Magnitude)
{
	struct Vector2f const c_v = {3.0f, 4.0f};

	HTRY_FLT(5.0f, ==, vector2f_get_magnitude(&c_v));
}

HTEST(Negate)
{
	struct Vector2f const c_v = {1.0f, 2.0f};
	struct Vector2f v;

	HTRY_PTR(&v, ==, vector2f_negate(&v, &c_v));
	HTRY_FLT(-1.0f, ==, v.x);
	HTRY_FLT(-2.0f, ==, v.y);
}

HTEST(Scale)
{
	struct Vector2f const c_v = {1.0f, 2.0f};
	struct Vector2f v;

	HTRY_PTR(&v, ==, vector2f_scale(&v, &c_v, 2.0f));
	HTRY_FLT(2.0f, ==, v.x);
	HTRY_FLT(4.0f, ==, v.y);
}

HTEST(Sub)
{
	struct Vector2f const c_v1 = {5.0f, 9.0f};
	struct Vector2f const c_v2 = {1.0f, 2.0f};
	struct Vector2f v;

	HTRY_PTR(&v, ==, vector2f_sub(&v, &c_v1, &c_v2));
	HTRY_FLT(4.0f, ==, v.x);
	HTRY_FLT(7.0f, ==, v.y);
}

HTEST_SUITE(Vector2)
{
	HTEST_ADD(DotAxes);
	HTEST_ADD(Magnitude);
	HTEST_ADD(Negate);
	HTEST_ADD(Scale);
	HTEST_ADD(Sub);
}
