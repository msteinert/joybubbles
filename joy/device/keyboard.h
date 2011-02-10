/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Keyboard device abstraction
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_DEVICE_KEYBOARD_H
#define JOY_DEVICE_KEYBOARD_H

#include <joy/device/keysym.h>
#include <joy/device/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_DEVICE_KEYBOARD \
	(joy_device_keyboard_get_type())

#define JOY_DEVICE_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_DEVICE_KEYBOARD, \
		JoyDeviceKeyboard))

#define JOY_IS_DEVICE_KEYBOARD(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_DEVICE_KEYBOARD))

#define JOY_DEVICE_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_DEVICE_KEYBOARD, \
		JoyDeviceKeyboardClass))

#define JOY_IS_DEVICE_KEYBOARD_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_DEVICE_KEYBOARD))

#define JOY_DEVICE_KEYBOARD_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_DEVICE_KEYBOARD, \
		JoyDeviceKeyboardClass))

typedef struct JoyDeviceKeyboardClass_ JoyDeviceKeyboardClass;

struct JoyDeviceKeyboard_ {
	/*< private >*/
	JoyDevice parent_instance;
};

struct JoyDeviceKeyboardClass_ {
	/*< private >*/
	JoyDeviceClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_device_keyboard_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif // JOY_DEVICE_KEYBOARD_H
