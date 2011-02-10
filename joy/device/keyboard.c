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

G_DEFINE_ABSTRACT_TYPE(JoyDeviceKeyboard, joy_device_keyboard, \
		JOY_TYPE_DEVICE)

static void
joy_device_keyboard_init(JoyDeviceKeyboard *self)
{
}

static void
joy_device_keyboard_class_init(JoyDeviceKeyboardClass *klass)
{
}
