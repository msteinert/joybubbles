/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/accumulator.h"
#include "joy/marshal.h"
#include "joy/source.h"

G_DEFINE_TYPE(JoySource, joy_source, G_TYPE_INITIALLY_UNOWNED)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_SOURCE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoySource *)instance)->priv)

enum Signals {
	SIGNAL_PREPARE,
	SIGNAL_INPUT,
	SIGNAL_HANGUP,
	SIGNAL_ERROR,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	gint fd;
};

static void
joy_source_init(JoySource *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->fd = -1;
}

enum Properties {
	PROP_0 = 0,
	PROP_DESCRIPTOR,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_DESCRIPTOR:
		priv->fd = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_source_class_init(JoySourceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoySource::prepare
	signals[SIGNAL_PREPARE] =
		g_signal_new(g_intern_static_string("prepare"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, prepare),
			joy_boolean_accumulator, NULL,
			joy_marshal_BOOLEAN__VOID, G_TYPE_BOOLEAN, 0);
	// JoySource::in
	signals[SIGNAL_INPUT] =
		g_signal_new(g_intern_static_string("input"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, input),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// JoySource::hangup
	signals[SIGNAL_HANGUP] =
		g_signal_new(g_intern_static_string("hangup"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, error),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// JoySource::error
	signals[SIGNAL_ERROR] =
		g_signal_new(g_intern_static_string("error"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, error),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// properties
	g_object_class_install_property(object_class, PROP_DESCRIPTOR,
		g_param_spec_int("descriptor", Q_("File Descriptor"),
			Q_("A selectable file descriptor"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoySource *
joy_source_new(gint descriptor)
{
	g_return_val_if_fail(0 > descriptor, NULL);
	return g_object_new(JOY_TYPE_SOURCE,
			"descriptor", descriptor,
			NULL);
}

gboolean
joy_source_prepare(JoySource *self)
{
	g_return_val_if_fail(JOY_IS_SOURCE(self), FALSE);
	gboolean ready = FALSE;
	g_signal_emit(self, signals[SIGNAL_PREPARE], 0, &ready);
	return ready;
}

gint
joy_source_get_descriptor(JoySource *self)
{
	g_return_val_if_fail(JOY_IS_SOURCE(self), 0);
	return GET_PRIVATE(self)->fd;
}

void
joy_source_dispatch(JoySource *self, gushort revents)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	if (revents & G_IO_IN) {
		g_signal_emit(self, signals[SIGNAL_INPUT], 0);
	} else if (revents & G_IO_HUP) {
		g_signal_emit(self, signals[SIGNAL_HANGUP], 0);
	} else if (revents & G_IO_ERR) {
		g_signal_emit(self, signals[SIGNAL_ERROR], 0);
	}
}

void
joy_source_input(JoySource *self)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_signal_emit(self, signals[SIGNAL_INPUT], 0);
}

void
joy_source_hangup(JoySource *self)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_signal_emit(self, signals[SIGNAL_HANGUP], 0);
}

void
joy_source_error(JoySource *self)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_signal_emit(self, signals[SIGNAL_ERROR], 0);
}
