/* Copyright 2011, 2012 Michael Steinert
 * Copyright 2011 EchoStar Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the Joybubbles project nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/platform/gfx3d/mouse.h"
#include "joy/platform/gfx3d/screen.h"

G_DEFINE_TYPE(JoyGfx3dMouse, joy_gfx3d_mouse, JOY_TYPE_DEVICE_MOUSE)

static void
joy_gfx3d_mouse_init(G_GNUC_UNUSED JoyGfx3dMouse *self)
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
