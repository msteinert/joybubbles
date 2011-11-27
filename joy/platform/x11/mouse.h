/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 mouse implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_MOUSE_H
#define JOY_X11_MOUSE_H

#include "joy/device/mouse.h"
#include "joy/platform/x11/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_X11_MOUSE \
	(joy_x11_mouse_get_type())

#define JOY_X11_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_MOUSE, \
		JoyX11Mouse))

#define JOY_IS_X11_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_MOUSE))

#define JOY_X11_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_MOUSE, \
		JoyX11MouseClass))

#define JOY_IS_X11_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_MOUSE))

#define JOY_X11_MOUSE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_MOUSE, \
		JoyX11MouseClass))

typedef struct JoyX11MouseClass_ JoyX11MouseClass;

struct JoyX11Mouse_ {
	/*< private >*/
	JoyDeviceMouse parent_instance;
};

struct JoyX11MouseClass_ {
	/*< private >*/
	JoyDeviceMouseClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_mouse_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a mouse device.
 *
 * \return A new mouse device object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyDevice *
joy_x11_mouse_new(void);

G_END_DECLS

#endif // JOY_X11_MOUSE_H
