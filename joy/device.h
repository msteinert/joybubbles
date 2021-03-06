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
 * \brief Input device abstraction
 */

#ifndef JOY_DEVICE_H
#define JOY_DEVICE_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_DEVICE \
	(joy_device_get_type())

#define JOY_DEVICE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_DEVICE, JoyDevice))

#define JOY_IS_DEVICE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_DEVICE))

#define JOY_DEVICE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_DEVICE, JoyDeviceClass))

#define JOY_IS_DEVICE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_DEVICE))

#define JOY_DEVICE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_DEVICE, \
		JoyDeviceClass))

typedef struct JoyDeviceClass_ JoyDeviceClass;

struct JoyDevice_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

struct JoyDeviceClass_ {
	/*< private >*/
	GObjectClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_device_get_type(void) G_GNUC_CONST;

/**
 * \brief Get the name of a device.
 *
 * \self [in] A device object.
 *
 * \return The name of \e self.
 */
const gchar *
joy_device_get_name(JoyDevice *self);

/**
 * \brief Get the screen this device is on.
 *
 * \self [in] A device object.
 *
 * \return The screen \e self is on.
 */
JoyScreen *
joy_device_get_screen(JoyDevice *self);

G_END_DECLS

#endif // JOY_DEVICE_H
