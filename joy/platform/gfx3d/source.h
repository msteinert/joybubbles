/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief GFX3D input source
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_GFX3D_SOURCE_H
#define JOY_GFX3D_SOURCE_H

#include "glib-object.h"
#include "joy/platform/gfx3d/types.h"
#include "joy/source.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_SOURCE \
	(joy_gfx3d_source_get_type())

#define JOY_GFX3D_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_SOURCE, \
		JoyGfx3dSource))

#define JOY_IS_GFX3D_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_SOURCE))

#define JOY_GFX3D_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_SOURCE, \
		JoyGfx3dSourceClass))

#define JOY_IS_GFX3D_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_SOURCE))

#define JOY_GFX3D_SOURCE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_SOURCE, \
		JoyGfx3dSourceClass))

typedef struct JoyGfx3dSourceClass_ JoyGfx3dSourceClass;

struct JoyGfx3dSource_ {
	/*< private >*/
	JoySource parent_instance;
	gpointer priv;
};

struct JoyGfx3dSourceClass_ {
	/*< private >*/
	JoySourceClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_source_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new GFX3D source object.
 *
 * \param app [in] A GFX3D application.
 *
 * \return A new GFX3D source object.
 */
G_GNUC_INTERNAL
JoySource *
joy_gfx3d_source_new(JoyApplication *app);

G_END_DECLS

#endif // JOY_GFX3D_SOURCE_H

