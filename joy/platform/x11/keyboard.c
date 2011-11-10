/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/platform/x11/keyboard.h"

G_DEFINE_TYPE(JoyX11Keyboard, joy_x11_keyboard, JOY_TYPE_DEVICE_KEYBOARD)

static void
joy_x11_keyboard_init(JoyX11Keyboard *self)
{
}

static void
joy_x11_keyboard_class_init(JoyX11KeyboardClass *klass)
{
}

JoyDevice *
joy_x11_keyboard_new(void)
{
	return g_object_new(JOY_TYPE_X11_KEYBOARD,
			NULL);
}

