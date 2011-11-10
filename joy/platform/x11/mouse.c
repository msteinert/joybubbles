/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/macros.h"
#include "joy/platform/x11/mouse.h"
#include "joy/platform/x11/screen.h"

G_DEFINE_TYPE(JoyX11Mouse, joy_x11_mouse, JOY_TYPE_DEVICE_MOUSE)

static void
joy_x11_mouse_init(JoyX11Mouse *self)
{
}

static void
set_cursor(JoyDevice *self, JoyCursor *cursor)
{
	JOY_UNIMPLEMENTED;
}

static void
warp(JoyDevice *self, gint x, gint y)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		return;
	}
	Display *display = joy_x11_screen_get_display(screen);
	if (G_UNLIKELY(!display)) {
		return;
	}
	XWarpPointer(display, None,
			XRootWindow(display, joy_screen_get_id(screen)),
			0, 0, 0, 0, x, y);
}

static void
get_location(JoyDevice *self, gint *x, gint *y)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		goto error;
	}
	Display *display = joy_x11_screen_get_display(screen);
	if (G_UNLIKELY(!display)) {
		goto error;
	}
	guint mask;
	Window root, child;
	gint root_x, root_y, win_x, win_y;
	Bool status = XQueryPointer(display,
			XRootWindow(display, joy_screen_get_id(screen)),
			&root, &child, &root_x, &root_y, &win_x, &win_y,
			&mask);
	if (x) {
		*x = status ? root_x : 0;
	}
	if (y) {
		*y = status ? root_y : 0;
	}
	return;
error:
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
	JOY_UNIMPLEMENTED;
}

static void
hide(JoyDevice *self)
{
	JOY_UNIMPLEMENTED;
}

static gboolean
visible(JoyDevice *self)
{
	JOY_UNIMPLEMENTED;
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
	return g_object_new(JOY_TYPE_X11_MOUSE,
			NULL);
}

