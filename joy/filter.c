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
#include "joy/filter.h"

G_DEFINE_ABSTRACT_TYPE(JoyFilter, joy_filter, G_TYPE_OBJECT)

static void
joy_filter_init(JoyFilter *self)
{
}

static void
joy_filter_class_init(JoyFilterClass *klass)
{
}

void
joy_filter_apply(JoyFilter *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_if_fail(JOY_IS_FILTER(self));
	g_return_if_fail(JOY_IS_BUBBLE(widget));
	g_return_if_fail(cr);
	cairo_save(cr);
	JOY_FILTER_GET_CLASS(self)->apply(self, widget, cr);
	cairo_restore(cr);
}
