/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/cursor.h"
#include "joy/device/mouse.h"
#include "joy/screen.h"

G_DEFINE_ABSTRACT_TYPE(JoyDeviceMouse, joy_device_mouse, JOY_TYPE_DEVICE)

static void
joy_device_mouse_init(JoyDeviceMouse *self)
{
}

static void
joy_device_mouse_class_init(JoyDeviceMouseClass *klass)
{
}

void
joy_device_mouse_set_cursor(JoyDevice *self, JoyCursor *cursor)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	g_return_if_fail(JOY_IS_CURSOR(cursor));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->set_cursor(self, cursor);
}

void
joy_device_mouse_warp(JoyDevice *self, JoyScreen *screen, gint x, gint y)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	g_return_if_fail(JOY_IS_SCREEN(screen));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->warp(self, screen, x, y);
}

void
joy_device_mouse_get_location(JoyDevice *self, JoyScreen **screen,
		gint *x, gint *y)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->get_location(self, screen, x, y);
}
