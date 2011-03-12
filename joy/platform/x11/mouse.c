/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/platform/x11/mouse.h"

G_DEFINE_TYPE(JoyX11Mouse, joy_x11_mouse, JOY_TYPE_DEVICE_MOUSE)

static void
joy_x11_mouse_init(JoyX11Mouse *self)
{
}

static void
set_cursor(JoyDevice *self, JoyCursor *cursor)
{
}

static void
warp(JoyDevice *self, gint x, gint y)
{
}

static void
get_location(JoyDevice *self, gint *x, gint *y)
{
	if (x) {
		*x = 0;
	}
	if (y) {
		*y = 0;
	}
}

static void
show(JoyDevice *self)
{
}

static void
hide(JoyDevice *self)
{
}

static gboolean
visible(JoyDevice *self)
{
	return TRUE;
}

static void
joy_x11_mouse_class_init(JoyX11MouseClass *klass)
{
	JoyDeviceMouseClass *mouse_class = JOY_DEVICE_MOUSE_CLASS(klass);
	mouse_class->set_cursor = set_cursor;
	mouse_class->warp = warp;
	mouse_class->get_location = get_location;
	mouse_class->show = show;
	mouse_class->hide = hide;
	mouse_class->visible = visible;
}

JoyDevice *
joy_x11_mouse_new(void)
{
	return g_object_new(JOY_TYPE_X11_MOUSE, NULL);
}
