/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/application.h"
#include "joy/dbus.h"
#include "joy/macros.h"
#include "joy/source/dbus.h"

G_DEFINE_TYPE(JoyDBus, joy_dbus, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_DBUS, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyDBus *)instance)->priv)

struct Private {
	JoyApplication *app;
	DBusConnection *connection;
	GHashTable *sources;
};

static void
destroy(gpointer data)
{
	g_object_run_dispose(data);
	g_object_unref(data);
}

static void
joy_dbus_init(JoyDBus *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->sources = g_hash_table_new_full(g_direct_hash, g_direct_equal,
			NULL, destroy);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->app) {
		g_object_unref(priv->app);
		priv->app = NULL;
	}
	if (priv->sources) {
		g_hash_table_destroy(priv->sources);
		priv->sources = NULL;
	}
	G_OBJECT_CLASS(joy_dbus_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->connection) {
		if (dbus_connection_get_is_connected(priv->connection)) {
			dbus_connection_close(priv->connection);
		}
		dbus_connection_unref(priv->connection);
	}
	G_OBJECT_CLASS(joy_dbus_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_APPLICATION,
	PROP_CONNECTION,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_APPLICATION:
		priv->app = g_value_dup_object(value);
		break;
	case PROP_CONNECTION:
		priv->connection = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_CONNECTION:
		g_value_set_pointer(value, priv->connection);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_dbus_class_init(JoyDBusClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	g_type_class_add_private(klass, sizeof(struct Private));
        // properties
	g_object_class_install_property(object_class, PROP_APPLICATION,
		g_param_spec_object("application", Q_("Application"),
			Q_("The application"), JOY_TYPE_APPLICATION,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_CONNECTION,
		g_param_spec_pointer("connection", Q_("D-Bus Connection"),
			Q_("A connection to a D-Bus server"),
			G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));
}

static dbus_bool_t
add(DBusWatch *watch, void *data)
{
	JoyDBus *self = JOY_DBUS(data);
	if (G_UNLIKELY(!self)) {
		goto exit;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->app)) {
		goto exit;
	}
	if (!dbus_watch_get_enabled(watch)) {
		goto exit;
	}
	JoySource *source = joy_source_dbus_new(self, watch);
	if (G_UNLIKELY(!source)) {
		goto exit;
	}
	g_object_ref_sink(source);
	g_hash_table_insert(priv->sources, watch, source);
	guint condition = dbus_watch_get_flags(watch);
	if (condition & DBUS_WATCH_READABLE) {
		joy_source_set_condition(source, G_IO_IN);
	}
	if (condition & DBUS_WATCH_WRITABLE) {
		joy_source_set_condition(source, G_IO_OUT);
	}
	joy_application_add_source(priv->app, source);
exit:
	return TRUE;
}

static void
remove(DBusWatch *watch, void *data)
{
	JoyDBus *self = JOY_DBUS(data);
	if (G_UNLIKELY(!self)) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->app)) {
		return;
	}
	JoySource *source = g_hash_table_lookup(priv->sources, watch);
	if (!source) {
		return;
	}
	joy_application_remove_source(priv->app, source);
	g_hash_table_remove(priv->sources, watch);
}

static void
toggled(DBusWatch *watch, void *data)
{
	if (dbus_watch_get_enabled(watch)) {
		add(watch, data);
	} else {
		remove(watch, data);
	}
}

JoyDBus *
joy_dbus_new(JoyApplication *app, const gchar *address, GError **error)
{
	joy_return_error_if_fail(JOY_IS_APPLICATION(app), NULL, error);
	joy_return_error_if_fail(address, NULL, error);
	DBusError dbus_error;
	dbus_error_init(&dbus_error);
	DBusConnection *connection = dbus_connection_open_private(address,
			&dbus_error);
	if (!connection) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_DBUS,
				"%s", dbus_error.message);
		goto error;
	}
	if (!dbus_bus_register(connection, &dbus_error)) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_DBUS,
				"%s", dbus_error.message);
		goto error;
	}
	JoyDBus *self = g_object_new(JOY_TYPE_DBUS,
			"application", app,
			"connection", connection,
			NULL);
	if (!self) {
		g_set_error_literal(error, JOY_ERROR, JOY_ERROR_NO_MEMORY,
				Q_("out of memory"));
		goto error;
	}
	if (!dbus_connection_set_watch_functions(connection, add, remove,
				toggled, self, NULL)) {
		g_set_error_literal(error, JOY_ERROR, JOY_ERROR_NO_MEMORY,
				Q_("out of memory"));
		goto error;
	}
	return self;
error:
	if (connection) {
		if (dbus_connection_get_is_connected(connection)) {
			dbus_connection_close(connection);
		}
		dbus_connection_unref(connection);
	}
	return NULL;
}

DBusConnection *
joy_dbus_get_connection(JoyDBus *self)
{
	g_return_val_if_fail(JOY_IS_DBUS(self), NULL);
	return GET_PRIVATE(self)->connection;
}
