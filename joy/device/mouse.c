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
#include "joy/cursor.h"
#include "joy/device/keyboard.h"
#include "joy/device/mouse.h"
#include "joy/screen.h"

G_DEFINE_ABSTRACT_TYPE(JoyDeviceMouse, joy_device_mouse, JOY_TYPE_DEVICE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_DEVICE_MOUSE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyDeviceMouse *)instance)->priv)

struct Private {
	JoyDevice *keyboard;
	long button;
};

static void
joy_device_mouse_init(JoyDeviceMouse *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->keyboard) {
		g_object_unref(priv->keyboard);
		priv->keyboard = NULL;
	}
	G_OBJECT_CLASS(joy_device_mouse_parent_class)->dispose(base);
}

static void
joy_device_mouse_class_init(JoyDeviceMouseClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	g_type_class_add_private(klass, sizeof(struct Private));
}

void
joy_device_mouse_button_down(JoyDevice *self, JoyMouseButton button)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	struct Private *priv = GET_PRIVATE(self);
	switch (button) {
	case JOY_MOUSE_BUTTON_LEFT:
	case JOY_MOUSE_BUTTON_MIDDLE:
	case JOY_MOUSE_BUTTON_RIGHT:
	case JOY_MOUSE_BUTTON_X1:
	case JOY_MOUSE_BUTTON_X2:
		priv->button |= button;
		break;
	default:
		break;
	}
}

void
joy_device_mouse_button_up(JoyDevice *self, JoyMouseButton button)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	struct Private *priv = GET_PRIVATE(self);
	switch (button) {
	case JOY_MOUSE_BUTTON_LEFT:
	case JOY_MOUSE_BUTTON_MIDDLE:
	case JOY_MOUSE_BUTTON_RIGHT:
	case JOY_MOUSE_BUTTON_X1:
	case JOY_MOUSE_BUTTON_X2:
		priv->button &= ~button;
		break;
	default:
		break;
	}
}

glong
joy_device_mouse_button_state(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE_MOUSE(self),
			JOY_MOUSE_BUTTON_NONE);
	return GET_PRIVATE(self)->button;
}

void
joy_device_mouse_set_keyboard(JoyDevice *self, JoyDevice *keyboard)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	g_return_if_fail(JOY_IS_DEVICE_KEYBOARD(keyboard));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->keyboard) {
		g_object_unref(priv->keyboard);
	}
	priv->keyboard = g_object_ref(keyboard);
}

JoyDevice *
joy_device_mouse_get_keyboard(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE_MOUSE(self), NULL);
	return GET_PRIVATE(self)->keyboard;
}

void
joy_device_mouse_set_cursor(JoyDevice *self, JoyCursor *cursor)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	g_return_if_fail(JOY_IS_CURSOR(cursor));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->set_cursor(self, cursor);
}

void
joy_device_mouse_warp(JoyDevice *self, gint x, gint y)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->warp(self, x, y);
}

void
joy_device_mouse_get_location(JoyDevice *self, gint *x, gint *y)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->get_location(self, x, y);
}

void
joy_device_mouse_show(JoyDevice *self)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->show(self);
}

void
joy_device_mouse_hide(JoyDevice *self)
{
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(self));
	JOY_DEVICE_MOUSE_GET_CLASS(self)->hide(self);
}

gboolean
joy_device_mouse_visible(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE_MOUSE(self), FALSE);
	return JOY_DEVICE_MOUSE_GET_CLASS(self)->visible(self);
}
