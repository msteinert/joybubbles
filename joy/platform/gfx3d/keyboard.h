/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief GFX3D keyboard implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_GFX3D_KEYBOARD_H
#define JOY_GFX3D_KEYBOARD_H

#include "joy/device/keyboard.h"
#include "joy/platform/gfx3d/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_KEYBOARD \
	(joy_gfx3d_keyboard_get_type())

#define JOY_GFX3D_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_KEYBOARD, \
		JoyGfx3dKeyboard))

#define JOY_IS_GFX3D_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_KEYBOARD))

#define JOY_GFX3D_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_KEYBOARD, \
		JoyGfx3dKeyboardClass))

#define JOY_IS_GFX3D_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_KEYBOARD))

#define JOY_GFX3D_KEYBOARD_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_KEYBOARD, \
		JoyGfx3dKeyboardClass))

typedef struct JoyGfx3dKeyboardClass_ JoyGfx3dKeyboardClass;

struct JoyGfx3dKeyboard_ {
	/*< private >*/
	JoyDeviceKeyboard parent_instance;
};

struct JoyGfx3dKeyboardClass_ {
	/*< private >*/
	JoyDeviceKeyboardClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_keyboard_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a keyboard device.
 *
 * \param screen [in] The screen this keyboard is on.
 *
 * \return A new keyboard device object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyDevice *
joy_gfx3d_keyboard_new(JoyScreen *screen);

G_END_DECLS

#endif // JOY_GFX3D_KEYBOARD_H
