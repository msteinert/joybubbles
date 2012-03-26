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
	SIGNAL_OUTPUT,
	SIGNAL_HANGUP,
	SIGNAL_ERROR,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	gint fd;
	GIOCondition flags;
};

static void
joy_source_init(JoySource *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->fd = -1;
	priv->flags = G_IO_HUP | G_IO_ERR;
}

enum Properties {
	PROP_0 = 0,
	PROP_DESCRIPTOR,
	PROP_CONDITION,
	PROP_MAX
};

static GParamSpec *properties[PROP_MAX];

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	JoySource *self = JOY_SOURCE(base);
	switch (id) {
	case PROP_DESCRIPTOR:
		joy_source_set_descriptor(self, g_value_get_int(value));
		break;
	case PROP_CONDITION:
		joy_source_set_condition(self, g_value_get_int(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	JoySource *self = JOY_SOURCE(base);
	switch (id) {
	case PROP_CONDITION:
		g_value_set_int(value, joy_source_get_condition(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
dispatch(JoySource *self, GIOCondition condition)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	if (G_LIKELY(condition & G_IO_IN)) {
		g_signal_emit(self, signals[SIGNAL_INPUT], 0);
	} else {
		if (condition & G_IO_OUT) {
			g_signal_emit(self, signals[SIGNAL_OUTPUT], 0);
		} else if (condition & G_IO_HUP) {
			g_signal_emit(self, signals[SIGNAL_HANGUP], 0);
		} else if (condition & G_IO_ERR) {
			g_signal_emit(self, signals[SIGNAL_ERROR], 0);
		}
	}
}

static void
joy_source_class_init(JoySourceClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	klass->dispatch = dispatch;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoySource::prepare
	signals[SIGNAL_PREPARE] =
		g_signal_new(g_intern_static_string("prepare"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, prepare),
			joy_boolean_accumulator, NULL,
			joy_marshal_BOOLEAN__VOID, G_TYPE_BOOLEAN, 0);
	// JoySource::input
	signals[SIGNAL_INPUT] =
		g_signal_new(g_intern_static_string("input"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, input),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// JoySource::output
	signals[SIGNAL_OUTPUT] =
		g_signal_new(g_intern_static_string("output"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoySourceClass, output),
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
			G_PARAM_WRITABLE));
	properties[PROP_CONDITION] =
		g_param_spec_int("condition", Q_("Watch Conditions"),
			Q_("The poll conditions to watch for"),
			G_MININT, G_MAXINT, G_IO_HUP | G_IO_ERR,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_CONDITION,
			properties[PROP_CONDITION]);
}

JoySource *
joy_source_new(gint descriptor)
{
	g_return_val_if_fail(0 <= descriptor, NULL);
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

void
joy_source_set_descriptor(JoySource *self, gint descriptor)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_return_if_fail(0 <= descriptor);
	GET_PRIVATE(self)->fd = descriptor;
}

gint
joy_source_get_descriptor(JoySource *self)
{
	g_return_val_if_fail(JOY_IS_SOURCE(self), 0);
	return GET_PRIVATE(self)->fd;
}

void
joy_source_set_condition(JoySource *self, GIOCondition flags)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	GET_PRIVATE(self)->flags |= flags;
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_CONDITION]);
}

void
joy_source_clear_condition(JoySource *self, GIOCondition flags)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	GET_PRIVATE(self)->flags &= ~flags;
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_CONDITION]);
}

GIOCondition
joy_source_get_condition(JoySource *self)
{
	g_return_val_if_fail(JOY_IS_SOURCE(self), 0);
	return GET_PRIVATE(self)->flags;
}

void
joy_source_dispatch(JoySource *self, GIOCondition condition)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	JOY_SOURCE_GET_CLASS(self)->dispatch(self, condition);
}

void
joy_source_input(JoySource *self)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_signal_emit(self, signals[SIGNAL_INPUT], 0);
}

void
joy_source_output(JoySource *self)
{
	g_return_if_fail(JOY_IS_SOURCE(self));
	g_signal_emit(self, signals[SIGNAL_OUTPUT], 0);
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
