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
#include "joy/image.h"
#include "joy/label.h"
#include "joy/marshal.h"

G_DEFINE_TYPE(JoyButton, joy_button, JOY_TYPE_BUBBLE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_BUTTON, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyButton *)instance)->priv)

enum Signals {
	SIGNAL_CLICKED,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	JoyBubble *label;
	JoyBubble *image;
};

static void
joy_button_init(JoyButton *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->label) {
		g_object_unref(priv->label);
		priv->label = NULL;
	}
	if (priv->image) {
		g_object_unref(priv->image);
		priv->image = NULL;
	}
	G_OBJECT_CLASS(joy_button_parent_class)->dispose(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_TEXT,
	PROP_MARKUP,
	PROP_LABEL,
	PROP_IMAGE,
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
	case PROP_MARKUP:
		joy_button_set_markup(self, g_value_get_string(value));
		break;
	case PROP_LABEL:
		joy_button_set_label(self, g_value_get_object(value));
		break;
	case PROP_IMAGE:
		joy_button_set_image(self, g_value_get_object(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
key_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym)
{
	JoyBubbleState state;
	switch (sym) {
	case JOY_KEY_Select:
	case JOY_KEY_Return:
	case JOY_KEY_space:
		state = joy_bubble_get_state(self);
		if (JOY_BUBBLE_STATE_FOCUSED == state) {
			joy_bubble_set_active(self);
		}
		break;
	default:
		break;
	}
}

static void
key_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym)
{
	JoyBubbleState state;
	switch (sym) {
	case JOY_KEY_Select:
	case JOY_KEY_Return:
	case JOY_KEY_space:
		state = joy_bubble_get_state(self);
		if (JOY_BUBBLE_STATE_ACTIVE == state) {
			g_signal_emit(self, signals[SIGNAL_CLICKED], 0);
			joy_bubble_set_focused(self);
		}
		break;
	default:
		break;
	}
}

static void
button_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button)
{
	JoyBubbleState state;
	switch (button) {
	case JOY_MOUSE_BUTTON_LEFT:
		state = joy_bubble_get_state(self);
		if (JOY_BUBBLE_STATE_FOCUSED == state) {
			joy_bubble_set_active(self);
		}
		break;
	default:
		break;
	}
}

static void
button_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button)
{
	JoyBubbleState state;
	switch (button) {
	case JOY_MOUSE_BUTTON_LEFT:
		state = joy_bubble_get_state(self);
		if (JOY_BUBBLE_STATE_ACTIVE == state) {
			g_signal_emit(self, signals[SIGNAL_CLICKED], 0);
			joy_bubble_set_focused(self);
		}
		break;
	default:
		break;
	}
}

static void
enter(JoyBubble *self, JoyDevice *device, gulong timestamp, gint x, gint y)
{
	joy_bubble_set_focused(self);
}

static void
leave(JoyBubble *self, JoyDevice *device, gulong timestamp, gint x, gint y)
{
	joy_bubble_set_default(self);
}

static void
joy_button_class_init(JoyButtonClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->key_down = key_down;
	bubble_class->key_up = key_up;
	bubble_class->button_down = button_down;
	bubble_class->button_up = button_up;
	bubble_class->enter = enter;
	bubble_class->leave = leave;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyButton::clicked
	signals[SIGNAL_CLICKED] =
		g_signal_new(g_intern_static_string("clicked"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			G_STRUCT_OFFSET(JoyButtonClass, clicked),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// properties
	g_object_class_install_property(object_class, PROP_TEXT,
		g_param_spec_string("text", Q_("Text"),
			Q_("The button text"), NULL,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_MARKUP,
		g_param_spec_string("markup", Q_("Markup"),
			Q_("The button markup"), NULL,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LABEL,
		g_param_spec_object("label", Q_("Label"),
			Q_("The button label"), JOY_TYPE_LABEL,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_IMAGE,
		g_param_spec_object("image", Q_("Image"),
			Q_("The image icon"), JOY_TYPE_IMAGE,
			G_PARAM_WRITABLE));
}

JoyBubble *
joy_button_new(const gchar *text)
{
	if (G_UNLIKELY(!text)) {
		return g_object_new(JOY_TYPE_BUTTON,
				"horizontal-expand", TRUE,
				"vertical-expand", TRUE,
				NULL);
	}
	JoyBubble *label = joy_label_new(text);
	if (G_UNLIKELY(!label)) {
		return NULL;
	}
	JoyBubble *self = g_object_new(JOY_TYPE_BUTTON,
			"horizontal-expand", TRUE,
			"vertical-expand", TRUE,
			"label", label,
			NULL);
	if (G_UNLIKELY(!self)) {
		g_object_unref(label);
	}
	return self;
}

void
joy_button_set_label(JoyBubble *self, JoyBubble *label)
{
	g_return_if_fail(JOY_IS_BUTTON(self));
	g_return_if_fail(JOY_IS_LABEL(label));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		g_object_unref(priv->label);
	}
	priv->label = label ? g_object_ref(label) : NULL;
}

JoyBubble *
joy_button_get_label(const JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUTTON(self), NULL);
	return GET_PRIVATE(self)->label;
}

void
joy_button_set_text(JoyBubble *self, const gchar *text)
{
	g_return_if_fail(JOY_IS_BUTTON(self));
	struct Private *priv = GET_PRIVATE(self);
	if (text) {
		if (priv->label) {
			joy_label_set_text(priv->label, text);
		} else {
			priv->label = joy_label_new(text);
		}
	} else {
		if (priv->label) {
			g_object_unref(priv->label);
			priv->label = NULL;
		}
	}
}

const gchar *
joy_button_get_text(const JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUTTON(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	const gchar *text = NULL;
	if (priv->label) {
		text = joy_label_get_text(priv->label);
	}
	return text;
}

void
joy_button_set_markup(JoyBubble *self, const gchar *markup)
{
	g_return_if_fail(JOY_IS_BUTTON(self));
	struct Private *priv = GET_PRIVATE(self);
	if (markup) {
		if (!priv->label) {
			priv->label = joy_label_new(NULL);
			if (G_UNLIKELY(!priv->label)) {
				return;
			}
		}
		joy_label_set_markup(priv->label, markup);
	} else {
		if (priv->label) {
			g_object_unref(priv->label);
			priv->label = NULL;
		}
	}
}

void
joy_button_set_image(JoyBubble *self, JoyBubble *image)
{
	g_return_if_fail(JOY_IS_BUTTON(self));
	g_return_if_fail(JOY_IS_IMAGE(image));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->image) {
		g_object_unref(priv->image);
	}
	priv->image = image ? g_object_ref(image) : NULL;
}

JoyBubble *
joy_button_get_image(const JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUTTON(self), NULL);
	return GET_PRIVATE(self)->image;
}
