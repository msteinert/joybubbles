/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief D-Bus context
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_DBUS_H
#define JOY_DBUS_H

#include <dbus/dbus.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_DBUS \
	(joy_dbus_get_type())

#define JOY_DBUS(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_DBUS, JoyDBus))

#define JOY_IS_DBUS(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_DBUS))

#define JOY_DBUS_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_DBUS, JoyDBusClass))

#define JOY_IS_DBUS_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_DBUS))

#define JOY_DBUS_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_DBUS, JoyDBusClass))

typedef struct JoyDBus_ JoyDBus;

typedef struct JoyDBusClass_ JoyDBusClass;

struct JoyDBus_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

struct JoyDBusClass_ {
	/*< private >*/
	GObjectClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_dbus_get_type(void) G_GNUC_CONST;

JoyDBus *
joy_dbus_new(JoyApplication *app, const gchar *address, GError **error);

DBusConnection *
joy_dbus_get_connection(JoyDBus *self);

G_END_DECLS

#endif // JOY_DBUS_H
