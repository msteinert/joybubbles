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
