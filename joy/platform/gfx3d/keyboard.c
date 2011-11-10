/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/platform/gfx3d/keyboard.h"
#include "joy/screen.h"

G_DEFINE_TYPE(JoyGfx3dKeyboard, joy_gfx3d_keyboard, JOY_TYPE_DEVICE_KEYBOARD)

static void
joy_gfx3d_keyboard_init(JoyGfx3dKeyboard *self)
{
}

static void
joy_gfx3d_keyboard_class_init(JoyGfx3dKeyboardClass *klass)
{
}

JoyDevice *
joy_gfx3d_keyboard_new(JoyScreen *screen)
{
	g_return_val_if_fail(JOY_IS_SCREEN(screen), NULL);
	return g_object_new(JOY_TYPE_GFX3D_KEYBOARD,
			"screen", screen,
			NULL);
}

