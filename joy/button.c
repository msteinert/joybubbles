/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/button.h"

G_DEFINE_TYPE(JoyButton, joy_button, JOY_TYPE_BUBBLE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_BUTTON, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyButton *)instance)->priv)

struct Private {
	gchar *text;
};

static void
joy_button_init(JoyButton *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->text);
	G_OBJECT_CLASS(joy_button_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_TEXT,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_TEXT:
		joy_button_set_text(self, g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_button_class_init(JoyButtonClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	// properties
	g_object_class_install_property(object_class, PROP_TEXT,
		g_param_spec_string("text", Q_("Text"),
			Q_("The button text"), NULL, G_PARAM_WRITABLE));
}

JoyBubble *
joy_button_new(const gchar *text)
{
	return g_object_new(JOY_TYPE_BUTTON,
			"horizontal-expand", TRUE,
			"vertical-expand", TRUE,
			"text", text,
			NULL);
}

void
joy_button_set_text(JoyBubble *self, const gchar *text)
{
	g_return_if_fail(JOY_IS_BUTTON(self));
	// TODO
}
