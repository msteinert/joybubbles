/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief GFX3D application implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_GFX3D_APPLICATION_H
#define JOY_GFX3D_APPLICATION_H

#include "joy/application.h"
#include "joy/platform/gfx3d/types.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_APPLICATION \
	(joy_gfx3d_application_get_type())

#define JOY_GFX3D_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_APPLICATION, \
		JoyGfx3dApplication))

#define JOY_IS_GFX3D_APPLICATION(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_APPLICATION))

#define JOY_GFX3D_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_APPLICATION, \
		JoyGfx3dApplicationClass))

#define JOY_IS_GFX3D_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_APPLICATION))

#define JOY_GFX3D_APPLICATION_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_APPLICATION, \
		JoyGfx3dApplicationClass))

typedef struct JoyGfx3dApplicationClass_ JoyGfx3dApplicationClass;

struct JoyGfx3dApplication_ {
	/*< private >*/
	JoyApplication parent_instance;
	gpointer priv;
};

struct JoyGfx3dApplicationClass_ {
	/*< private >*/
	JoyApplicationClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_application_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif // JOY_GFX3D_APPLICATION_H
