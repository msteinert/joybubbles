/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Input device abstraction
 * \author Mike Steinert <michael.steinert@echostar.com>
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

