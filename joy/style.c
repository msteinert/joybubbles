/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/bubble.h"
#include "joy/style.h"

G_DEFINE_ABSTRACT_TYPE(JoyStyle, joy_style, G_TYPE_OBJECT)

static void
joy_style_init(JoyStyle *self)
{
}

static void
joy_style_class_init(JoyStyleClass *klass)
{
}

gboolean
joy_style_draw(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), FALSE);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), FALSE);
	g_return_val_if_fail(cr, FALSE);
	return JOY_STYLE_GET_CLASS(self)->draw(self, widget, cr);
}
