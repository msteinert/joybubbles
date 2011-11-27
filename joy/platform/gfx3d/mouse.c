/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/platform/gfx3d/mouse.h"
#include "joy/platform/gfx3d/screen.h"

G_DEFINE_TYPE(JoyGfx3dMouse, joy_gfx3d_mouse, JOY_TYPE_DEVICE_MOUSE)

static void
joy_gfx3d_mouse_init(JoyGfx3dMouse *self)
{
}

static void
set_cursor(JoyDevice *self, JoyCursor *cursor)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_set_cursor(screen, cursor);
	}
}

static void
warp(JoyDevice *self, gint x, gint y)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_warp_cursor(screen, x, y);
	}
}

static void
get_location(JoyDevice *self, gint *x, gint *y)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_get_cursor_position(screen, x, y);
	} else {
		if (x) {
			*x = 0;
		}
		if (y) {
			*y = 0;
		}
	}
}

static void
show(JoyDevice *self)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_show_cursor(screen);
	}
}

static void
hide(JoyDevice *self)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_hide_cursor(screen);
	}
}

static gboolean
visible(JoyDevice *self)
{
	JoyScreen *screen = joy_device_get_screen(self);
	if (G_LIKELY(screen)) {
		return joy_gfx3d_screen_cursor_visible(screen);
	}
	return FALSE;
}

static void
joy_gfx3d_mouse_class_init(JoyGfx3dMouseClass *klass)
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
joy_gfx3d_mouse_new(JoyScreen *screen)
{
	g_return_val_if_fail(JOY_IS_SCREEN(screen), NULL);
	return g_object_new(JOY_TYPE_GFX3D_MOUSE,
			"screen", screen,
			NULL);
}
