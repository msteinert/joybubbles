/* Copyright 2011, 2012 Michael Steinert
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
#include "joy/macros.h"
#include "joy/platform/x11/mouse.h"
#include "joy/platform/x11/screen.h"

G_DEFINE_TYPE(JoyX11Mouse, joy_x11_mouse, JOY_TYPE_DEVICE_MOUSE)

static void
joy_x11_mouse_init(G_GNUC_UNUSED JoyX11Mouse *self)
{
}

static void
set_cursor(G_GNUC_UNUSED JoyDevice *self, G_GNUC_UNUSED JoyCursor *cursor)
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
show(G_GNUC_UNUSED JoyDevice *self)
{
	JOY_UNIMPLEMENTED;
}

static void
hide(G_GNUC_UNUSED JoyDevice *self)
{
	JOY_UNIMPLEMENTED;
}

static gboolean
visible(G_GNUC_UNUSED JoyDevice *self)
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
