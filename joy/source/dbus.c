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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/source/dbus.h"

G_DEFINE_TYPE(JoySourceDBus, joy_source_dbus, JOY_TYPE_SOURCE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_SOURCE_DBUS, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoySourceDBus *)instance)->priv)

struct Private {
	JoyDBus *dbus;
	DBusWatch *watch;
};

static void
joy_source_dbus_init(JoySourceDBus *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->dbus) {
		g_object_unref(priv->dbus);
		priv->dbus = NULL;
	}
	G_OBJECT_CLASS(joy_source_dbus_parent_class)->dispose(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_DBUS,
	PROP_WATCH,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_DBUS:
		priv->dbus = g_value_dup_object(value);
		break;
	case PROP_WATCH:
		priv->watch = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
dispatch(JoySource *self, GIOCondition condition)
{
	struct Private *priv = GET_PRIVATE(self);
	DBusConnection *connection = joy_dbus_get_connection(priv->dbus);
	if (G_UNLIKELY(!connection)) {
		return;
	}
	guint flags = 0;
	if (condition & G_IO_IN) {
		flags |= DBUS_WATCH_READABLE;
	}
	if (condition & G_IO_OUT) {
		flags |= DBUS_WATCH_WRITABLE;
	}
	if (condition & G_IO_HUP) {
		flags |= DBUS_WATCH_HANGUP;
	}
	if (condition & G_IO_ERR) {
		flags |= DBUS_WATCH_ERROR;
	}
	dbus_connection_ref(connection);
	dbus_watch_handle(priv->watch, flags);
	DBusDispatchStatus status =
		dbus_connection_get_dispatch_status(connection);
	while (DBUS_DISPATCH_DATA_REMAINS == status) {
		status = dbus_connection_dispatch(connection);
	}
	dbus_connection_unref(connection);
}

static void
joy_source_dbus_class_init(JoySourceDBusClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	JoySourceClass *source_class = JOY_SOURCE_CLASS(klass);
	source_class->dispatch = dispatch;
	g_type_class_add_private(klass, sizeof(struct Private));
        // properties
	g_object_class_install_property(object_class, PROP_DBUS,
		g_param_spec_object("d-bus", Q_("D-Bus Context"),
			Q_("The D-Bus context"), JOY_TYPE_DBUS,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_WATCH,
		g_param_spec_pointer("watch", Q_("D-Bus Watch"),
			Q_("A D-Bus watch object"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoySource *
joy_source_dbus_new(JoyDBus *dbus, DBusWatch *watch)
{
	g_return_val_if_fail(JOY_IS_DBUS(dbus), NULL);
	g_return_val_if_fail(watch, NULL);
	return g_object_new(JOY_TYPE_SOURCE_DBUS,
			"descriptor", dbus_watch_get_unix_fd(watch),
			"d-bus", dbus,
			"watch", watch,
			NULL);
}
