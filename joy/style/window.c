/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/style/window.h"
#include "joy/window.h"

G_DEFINE_ABSTRACT_TYPE(JoyStyleWindow, joy_style_window, JOY_TYPE_STYLE)

static void
joy_style_window_init(JoyStyleWindow *self)
{
}

static gboolean
draw(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_WINDOW(widget), FALSE);
	JoyStyleWindowClass *klass = JOY_STYLE_WINDOW_GET_CLASS(self);
	if (klass->draw_background) {
		klass->draw_background(self, widget, cr);
	}
	return TRUE;
}

static void
joy_style_window_class_init(JoyStyleWindowClass *klass)
{
	JoyStyleClass *style_class = JOY_STYLE_CLASS(klass);
	style_class->draw = draw;
}
