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

#include <widget.h>
#include <hutils/memory.h>
#include <holder.h>
#include <hwt.h>

void
widget_free(struct HWT *const a_hwt, struct HWTWidget **const a_widget)
{
	struct HWTWidget *widget;

	widget = *a_widget;
	if (NULL == widget) {
		return;
	}
	hwt_get_callback(widget)->destroy(a_hwt, widget);
	if (NULL != widget->holder) {
		widget->holder->child = NULL;
	}
	/* Careful, *a_widget can be NULL here. */
	FREE(widget);
	*a_widget = NULL;
}

void
widget_setup(struct HWTWidget *const a_widget, struct HWTWidgetType const
    *const a_type)
{
	assert(NULL != a_type);
	a_widget->type = a_type;
	a_widget->holder = NULL;
}

void
widget_propagate_min(struct HWT *const a_hwt, struct HWTWidget *const
    a_widget, struct HWTRect *const a_min)
{
	if (NULL != a_widget) {
		hwt_get_callback(a_widget)->propagate_min(a_hwt, a_widget,
		    a_min);
	}
}

void
widget_propagate_size(struct HWT *const a_hwt, struct HWTWidget *const
    a_widget, struct HWTRect const *const a_size)
{
	if (NULL != a_widget) {
		hwt_get_callback(a_widget)->propagate_size(a_hwt, a_widget,
		    a_size);
	}
}
