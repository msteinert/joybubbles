/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 keyboard implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_KEYBOARD_H
#define JOY_X11_KEYBOARD_H

#include "joy/device/keyboard.h"
#include "joy/platform/x11/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_X11_KEYBOARD \
	(joy_x11_keyboard_get_type())

#define JOY_X11_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_KEYBOARD, \
		JoyX11Keyboard))

#define JOY_IS_X11_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_KEYBOARD))

#define JOY_X11_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_KEYBOARD, \
		JoyX11KeyboardClass))

#define JOY_IS_X11_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_KEYBOARD))

#define JOY_X11_KEYBOARD_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_KEYBOARD, \
		JoyX11KeyboardClass))

typedef struct JoyX11KeyboardClass_ JoyX11KeyboardClass;

struct JoyX11Keyboard_ {
	/*< private >*/
	JoyDeviceKeyboard parent_instance;
};

struct JoyX11KeyboardClass_ {
	/*< private >*/
	JoyDeviceKeyboardClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_keyboard_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a keyboard device.
 *
 * \return A new keyboard device object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyDevice *
joy_x11_keyboard_new(void);

G_END_DECLS

#endif // JOY_X11_KEYBOARD_H

