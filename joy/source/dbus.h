/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief D-Bus input source
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_SOURCE_DBUS_H
#define JOY_SOURCE_DBUS_H

#include <joy/dbus.h>
#include <joy/source.h>

G_BEGIN_DECLS

#define JOY_TYPE_SOURCE_DBUS \
	(joy_source_dbus_get_type())

#define JOY_SOURCE_DBUS(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_SOURCE_DBUS, \
		JoySourceDBus))

#define JOY_IS_SOURCE_DBUS(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_SOURCE_DBUS))

#define JOY_SOURCE_DBUS_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_SOURCE_DBUS, \
		JoySourceDBusClass))

#define JOY_IS_SOURCE_DBUS_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_SOURCE_DBUS))

#define JOY_SOURCE_DBUS_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_SOURCE_DBUS, \
		JoySourceDBusClass))

typedef struct JoySourceDBus_ JoySourceDBus;

typedef struct JoySourceDBusClass_ JoySourceDBusClass;

struct JoySourceDBus_ {
	/*< private >*/
	JoySource parent_instance;
	gpointer priv;
};

struct JoySourceDBusClass_ {
	/*< private >*/
	JoySourceClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_source_dbus_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new D-Bus source object.
 *
 * \return A new D-Bus source object.
 */
JoySource *
joy_source_dbus_new(JoyDBus *dbus, DBusWatch *watch);

G_END_DECLS

#endif // JOY_SOURCE_DBUS_H

