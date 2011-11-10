/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/device/keyboard.h"
#include "joy/device/mouse.h"

G_DEFINE_ABSTRACT_TYPE(JoyDeviceKeyboard, joy_device_keyboard, \
		JOY_TYPE_DEVICE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_DEVICE_KEYBOARD, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyDeviceKeyboard *)instance)->priv)

struct Private {
	JoyDevice *mouse;
	long modifier;
};


static void
joy_device_keyboard_init(JoyDeviceKeyboard *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->mouse) {
		g_object_unref(priv->mouse);
		priv->mouse = NULL;
	}
	G_OBJECT_CLASS(joy_device_keyboard_parent_class)->dispose(base);
}

static void
joy_device_keyboard_class_init(JoyDeviceKeyboardClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	g_type_class_add_private(klass, sizeof(struct Private));
}

void
joy_device_keyboard_set_mouse(JoyDevice *self, JoyDevice *mouse)
{
	g_return_if_fail(JOY_IS_DEVICE_KEYBOARD(self));
	g_return_if_fail(JOY_IS_DEVICE_MOUSE(mouse));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->mouse) {
		g_object_unref(priv->mouse);
	}
	priv->mouse = g_object_ref(mouse);
}

JoyDevice *
joy_device_keyboard_get_mouse(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE_KEYBOARD(self), NULL);
	return GET_PRIVATE(self)->mouse;
}

void
joy_device_keyboard_modifier_down(JoyDevice *self, JoyKeySym sym)
{
	g_return_if_fail(JOY_IS_DEVICE_KEYBOARD(self));
	struct Private *priv = GET_PRIVATE(self);
	switch (sym) {
	case JOY_KEY_Shift_L:
		priv->modifier |= JOY_MOD_SHIFT_LEFT;
		break;
	case JOY_KEY_Shift_R:
		priv->modifier |= JOY_MOD_SHIFT_RIGHT;
		break;
	case JOY_KEY_Control_L:
		priv->modifier |= JOY_MOD_CONTROL_LEFT;
		break;
	case JOY_KEY_Control_R:
		priv->modifier |= JOY_MOD_CONTROL_RIGHT;
		break;
	case JOY_KEY_Caps_Lock:
		priv->modifier |= JOY_MOD_CAPS_LOCK;
		break;
	case JOY_KEY_Shift_Lock:
		priv->modifier |= JOY_MOD_SHIFT_LOCK;
		break;
	case JOY_KEY_Meta_L:
		priv->modifier |= JOY_MOD_META_LEFT;
		break;
	case JOY_KEY_Meta_R:
		priv->modifier |= JOY_MOD_META_RIGHT;
		break;
	case JOY_KEY_Alt_L:
		priv->modifier |= JOY_MOD_ALT_LEFT;
		break;
	case JOY_KEY_Alt_R:
		priv->modifier |= JOY_MOD_ALT_RIGHT;
		break;
	case JOY_KEY_Super_L:
		priv->modifier |= JOY_MOD_SUPER_LEFT;
		break;
	case JOY_KEY_Super_R:
		priv->modifier |= JOY_MOD_SUPER_RIGHT;
		break;
	case JOY_KEY_Hyper_L:
		priv->modifier |= JOY_MOD_HYPER_LEFT;
		break;
	case JOY_KEY_Hyper_R:
		priv->modifier |= JOY_MOD_HYPER_RIGHT;
		break;
	default:
		break;
	}
}

void
joy_device_keyboard_modifier_up(JoyDevice *self, JoyKeySym sym)
{
	g_return_if_fail(JOY_IS_DEVICE_KEYBOARD(self));
	struct Private *priv = GET_PRIVATE(self);
	switch (sym) {
	case JOY_KEY_Shift_L:
		priv->modifier &= ~JOY_MOD_SHIFT_LEFT;
		break;
	case JOY_KEY_Shift_R:
		priv->modifier &= ~JOY_MOD_SHIFT_RIGHT;
		break;
	case JOY_KEY_Control_L:
		priv->modifier &= ~JOY_MOD_CONTROL_LEFT;
		break;
	case JOY_KEY_Control_R:
		priv->modifier &= ~JOY_MOD_CONTROL_RIGHT;
		break;
	case JOY_KEY_Caps_Lock:
		priv->modifier &= ~JOY_MOD_CAPS_LOCK;
		break;
	case JOY_KEY_Shift_Lock:
		priv->modifier &= ~JOY_MOD_SHIFT_LOCK;
		break;
	case JOY_KEY_Meta_L:
		priv->modifier &= ~JOY_MOD_META_LEFT;
		break;
	case JOY_KEY_Meta_R:
		priv->modifier &= ~JOY_MOD_META_RIGHT;
		break;
	case JOY_KEY_Alt_L:
		priv->modifier &= ~JOY_MOD_ALT_LEFT;
		break;
	case JOY_KEY_Alt_R:
		priv->modifier &= ~JOY_MOD_ALT_RIGHT;
		break;
	case JOY_KEY_Super_L:
		priv->modifier &= ~JOY_MOD_SUPER_LEFT;
		break;
	case JOY_KEY_Super_R:
		priv->modifier &= ~JOY_MOD_SUPER_RIGHT;
		break;
	case JOY_KEY_Hyper_L:
		priv->modifier &= ~JOY_MOD_HYPER_LEFT;
		break;
	case JOY_KEY_Hyper_R:
		priv->modifier &= ~JOY_MOD_HYPER_RIGHT;
		break;
	default:
		break;
	}
}

glong
joy_device_keyboard_modifier_state(JoyDevice *self)
{
	g_return_val_if_fail(JOY_IS_DEVICE_KEYBOARD(self), JOY_MOD_NONE);
	return GET_PRIVATE(self)->modifier;
}

