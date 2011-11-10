/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief GFX3D mouse implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_GFX3D_MOUSE_H
#define JOY_GFX3D_MOUSE_H

#include "joy/device/mouse.h"
#include "joy/platform/gfx3d/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_MOUSE \
	(joy_gfx3d_mouse_get_type())

#define JOY_GFX3D_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouse))

#define JOY_IS_GFX3D_MOUSE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_MOUSE))

#define JOY_GFX3D_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouseClass))

#define JOY_IS_GFX3D_MOUSE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_MOUSE))

#define JOY_GFX3D_MOUSE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_MOUSE, \
		JoyGfx3dMouseClass))

typedef struct JoyGfx3dMouseClass_ JoyGfx3dMouseClass;

struct JoyGfx3dMouse_ {
	/*< private >*/
	JoyDeviceMouse parent_instance;
};

struct JoyGfx3dMouseClass_ {
	/*< private >*/
	JoyDeviceMouseClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_mouse_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a mouse device.
 *
 * \param screen [in] The screen that this mouse is on.
 *
 * \return A new mouse device object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyDevice *
joy_gfx3d_mouse_new(JoyScreen *screen);

G_END_DECLS

#endif // JOY_GFX3D_MOUSE_H

