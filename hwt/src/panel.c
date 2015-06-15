/*
 * Copyright (c) 2015 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#include <hwt/panel.h>
#include <hutils/memory.h>
#include <hwt/hwt.h>
#include <holder.h>
#include <hwt.h>
#include <panel.h>
#include <widget.h>

struct Panel {
	struct	HWTWidget widget;
	struct	HWTHolder holder;
};

static void	panel_destroy(struct HWT *, struct HWTWidget *);
static void	panel_draw(struct HWT *, struct HWTWidget *);
static void	panel_propagate_min(struct HWT *, struct HWTWidget *, struct
    HWTRect *);
static void	panel_propagate_size(struct HWT *, struct HWTWidget *, struct
    HWTRect const *);

static struct HWTWidgetType const *g_type;

struct HWTWidget *
hwt_panel_create()
{
	struct Panel *panel;

	CALLOC(panel, 1);
	widget_setup(&panel->widget, g_type);
	return &panel->widget;
}

void
hwt_panel_setup_(struct HWT *const a_hwt)
{
	struct HWTWidgetCallback callback;

	HWT_CALLBACK_SETUP(callback, panel);
	g_type = hwt_widget_register(a_hwt, "Panel", &callback);
}

struct HWTHolder *
hwt_panel_get_holder(struct HWTWidget *const a_parent)
{
	struct Panel *panel;

	HWT_CAST(g_type, panel, a_parent);
	return &panel->holder;
}

void
panel_destroy(struct HWT *const a_hwt, struct HWTWidget *const a_widget)
{
	struct Panel *panel;

	HWT_CAST(g_type, panel, a_widget);
	widget_free(a_hwt, &panel->holder.child);
}

void
panel_draw(struct HWT *const a_hwt, struct HWTWidget *const a_widget)
{
	(void)a_hwt;
	(void)a_widget;
}

void
panel_propagate_min(struct HWT *const a_hwt, struct HWTWidget *const a_widget,
    struct HWTRect *const a_min)
{
	struct Panel *panel;

	HWT_CAST(g_type, panel, a_widget);
	widget_propagate_min(a_hwt, panel->holder.child, a_min);
}

void
panel_propagate_size(struct HWT *const a_hwt, struct HWTWidget *const
    a_widget, struct HWTRect const *const a_size)
{
	struct Panel *panel;

	HWT_CAST(g_type, panel, a_widget);
	widget_propagate_size(a_hwt, panel->holder.child, a_size);
}