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
#include "joy/iterator.h"
#include "joy/label.h"
#include "joy/marshal.h"
#include "joy/theme.h"
#include "joy/style.h"

G_DEFINE_TYPE(JoyButton, joy_button, JOY_TYPE_CONTAINER)

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
};

static void
joy_button_init(JoyButton *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
on_notify(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		g_object_notify(G_OBJECT(priv->label), "style");
	}
}

static void
constructed(GObject *base)
{
	g_signal_connect(base, "notify::style", G_CALLBACK(on_notify), NULL);
	if (G_OBJECT_CLASS(joy_button_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_button_parent_class)->constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->label) {
		g_object_unref(priv->label);
		priv->label = NULL;
	}
	G_OBJECT_CLASS(joy_button_parent_class)->dispose(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_TEXT,
	PROP_MAX
};

static GParamSpec *properties[PROP_MAX];

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
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_TEXT:
		g_value_set_string(value, joy_button_get_text(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static JoyBubble *
at(JoyBubble *self, gint x, gint y)
{
	if (JOY_BUBBLE_CLASS(joy_button_parent_class)->at(self, x, y)) {
		return self;
	}
	return NULL;
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	JOY_BUBBLE_CLASS(joy_button_parent_class)->resize(self, width, height);
	if (priv->label) {
		joy_bubble_resize(priv->label, width,
				joy_bubble_get_height(priv->label));
		gint y = height / 2. - joy_bubble_get_height(priv->label) / 2.;
		joy_bubble_move(priv->label, 0, y);
	}
}

/**
 * \brief Process a button/key down event
 *
 * \param self [in] A button object.
 */
static void
down(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_STATE_FOCUSED == joy_bubble_get_state(self)) {
		joy_bubble_set_active(self);
		if (priv->label) {
			joy_bubble_translate(priv->label, 1, 1);
		}
	}
}

/**
 * \brief Process a button/key up event
 *
 * \param self [in] A button object.
 */
static void
up(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_STATE_ACTIVE == joy_bubble_get_state(self)) {
		joy_bubble_set_focused(self);
		if (priv->label) {
			joy_bubble_translate(priv->label, -1, -1);
		}
		g_signal_emit(self, signals[SIGNAL_CLICKED], 0);
	}
}

static void
key_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym)
{
	switch (sym) {
	case JOY_KEY_Select:
	case JOY_KEY_Return:
	case JOY_KEY_space:
		down(self);
		break;
	default:
		break;
	}
}

static void
key_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym)
{
	switch (sym) {
	case JOY_KEY_Select:
	case JOY_KEY_Return:
	case JOY_KEY_space:
		up(self);
		break;
	default:
		break;
	}
}

static void
button_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button)
{
	if (JOY_MOUSE_BUTTON_LEFT == button) {
		down(self);
	}
}

static void
button_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button)
{
	if (JOY_MOUSE_BUTTON_LEFT == button) {
		up(self);
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
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label && JOY_BUBBLE_STATE_ACTIVE
			== joy_bubble_get_state(self)) {
		joy_bubble_translate(priv->label, -1, -1);
	}
	joy_bubble_set_default(self);
}

static void
joy_button_class_init(JoyButtonClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->at = at;
	bubble_class->resize = resize;
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
	properties[PROP_TEXT] =
		g_param_spec_string("text", Q_("Text"),
			Q_("The button text"), NULL, G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_TEXT,
			properties[PROP_TEXT]);
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
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		g_object_unref(priv->label);
	}
	if (text) {
		priv->label = joy_label_new(text);
		if (G_LIKELY(priv->label)) {
			joy_label_set_alignment(priv->label,
					PANGO_ALIGN_CENTER);
			joy_container_add(self, priv->label);
		}
	} else {
		priv->label = NULL;
	}
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_TEXT]);
}

const gchar *
joy_button_get_text(const JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUTTON(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	return priv->label ? joy_label_get_text(priv->label) : NULL;
}
