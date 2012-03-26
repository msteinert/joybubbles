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

/**
 * \file
 * \brief Keyboard device abstraction
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

/**
 * \brief Modifier key IDs
 */
typedef enum {
	JOY_MOD_NONE = 0, /**< Sentinel value */
	JOY_MOD_SHIFT_LEFT = 1 << 0, /**< Left shift */
	JOY_MOD_SHIFT_RIGHT = 1 << 1, /**< Right shift */
	JOY_MOD_CONTROL_LEFT = 1 << 2, /**< Left control */
	JOY_MOD_CONTROL_RIGHT = 1 << 3, /**< Right control */
	JOY_MOD_CAPS_LOCK = 1 << 4, /**< Caps Lock */
	JOY_MOD_SHIFT_LOCK = 1 << 5, /**< Shift lock */
	JOY_MOD_META_LEFT = 1 << 6, /**< Left meta */
	JOY_MOD_META_RIGHT = 1 << 7, /**< Right meta */
	JOY_MOD_ALT_LEFT = 1 << 8, /**< Left Alt */
	JOY_MOD_ALT_RIGHT = 1 << 9, /**< Right Alt */
	JOY_MOD_SUPER_LEFT = 1 << 10, /**< Left super */
	JOY_MOD_SUPER_RIGHT = 1 << 11, /**< Right super */
	JOY_MOD_HYPER_LEFT = 1 << 12, /**< Left hyper */
	JOY_MOD_HYPER_RIGHT = 1 << 13, /**< Right hyper */
	JOY_MODE_MAX = G_MAXINT /**< Sentinel value */
} JoyMod;

typedef struct JoyDeviceKeyboardClass_ JoyDeviceKeyboardClass;

struct JoyDeviceKeyboard_ {
	/*< private >*/
	JoyDevice parent_instance;
	gpointer priv;
};

struct JoyDeviceKeyboardClass_ {
	/*< private >*/
	JoyDeviceClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_device_keyboard_get_type(void) G_GNUC_CONST;

/**
 * \brief Set the state of a modifier key for a keyboard.
 *
 * \param self [in] A keyboard device object.
 * \param sym [in] A modifier key sym.
 */
void
joy_device_keyboard_modifier_down(JoyDevice *self, JoyKeySym sym);

/**
 * \brief Unset the state of a modifier key for a keyboard.
 *
 * \param self [in] A keyboard device object.
 * \param sym [in] A modifier key sym.
 */
void
joy_device_keyboard_modifier_up(JoyDevice *self, JoyKeySym sym);

/**
 * \brief Retrieve the state of the modifier keys for a keyboard.
 *
 * \param self [in] A keyboard device object.
 *
 * \param The modifier key state for \e self.
 */
glong
joy_device_keyboard_modifier_state(JoyDevice *self);

/**
 * \brief Set the mouse device associated with a keyboard.
 *
 * \param self [in] A keyboard device object.
 * \param mouse [in] A mouse device object.
 */
void
joy_device_keyboard_set_mouse(JoyDevice *self, JoyDevice *mouse);

/**
 * \brief Get the mouse device associated with a keyboard.
 *
 * \param self [in] A keyboard device object.
 *
 * \return The mouse device associated with \e self.
 */
JoyDevice *
joy_device_keyboard_get_mouse(JoyDevice *self);

G_END_DECLS

#endif // JOY_DEVICE_KEYBOARD_H
