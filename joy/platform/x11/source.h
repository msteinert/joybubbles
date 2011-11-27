/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 input source
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_SOURCE_H
#define JOY_X11_SOURCE_H

#include "glib-object.h"
#include "joy/platform/x11/types.h"
#include "joy/source.h"
#include <X11/Xlib.h>

G_BEGIN_DECLS

#define JOY_TYPE_X11_SOURCE \
	(joy_x11_source_get_type())

#define JOY_X11_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_SOURCE, \
		JoyX11Source))

#define JOY_IS_X11_SOURCE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_SOURCE))

#define JOY_X11_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_SOURCE, \
		JoyX11SourceClass))

#define JOY_IS_X11_SOURCE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_SOURCE))

#define JOY_X11_SOURCE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_SOURCE, \
		JoyX11SourceClass))

typedef struct JoyX11SourceClass_ JoyX11SourceClass;

struct JoyX11Source_ {
	/*< private >*/
	JoySource parent_instance;
	gpointer priv;
};

struct JoyX11SourceClass_ {
	/*< private >*/
	JoySourceClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_source_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new X11 source object.
 *
 * \param display [in] An X11 application.
 *
 * \return A new X11 source object.
 */
G_GNUC_INTERNAL
JoySource *
joy_x11_source_new(JoyApplication *display);

G_END_DECLS

#endif // JOY_X11_SOURCE_H
