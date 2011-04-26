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
#include "joy/style/button.h"

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
on_notify(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	JoyStyle *style = joy_bubble_get_style(self);
	if (priv->image) {
		joy_bubble_set_style(priv->image, style);
		if (style && G_LIKELY(JOY_IS_STYLE_BUTTON(style))) {
			joy_style_button_configure_image(style, self,
					priv->image);
		}
	}
	if (priv->label) {
		joy_bubble_set_style(priv->label, style);
		if (style && G_LIKELY(JOY_IS_STYLE_BUTTON(style))) {
			joy_style_button_configure_label(style, self,
					priv->label);
		}
	}
}

static void
constructed(GObject *base)
{
	g_signal_connect(base, "notify::style", G_CALLBACK(on_notify), NULL);
	g_signal_connect_after(base, "resize", G_CALLBACK(on_notify), NULL);
	if (G_OBJECT_CLASS(joy_button_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_button_parent_class)->constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->label) {
		g_object_run_dispose(G_OBJECT(priv->label));
		g_object_unref(priv->label);
		priv->label = NULL;
	}
	if (priv->image) {
		g_object_run_dispose(G_OBJECT(priv->image));
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

static GParamSpec *properties[PROP_MAX];

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
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_LABEL:
		g_value_set_object(value, joy_button_get_label(self));
		break;
	case PROP_IMAGE:
		g_value_set_object(value, joy_button_get_image(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
expose(JoyBubble *self, const cairo_rectangle_int_t *rect)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		joy_bubble_expose(priv->label, rect);
	}
	if (priv->image) {
		joy_bubble_expose(priv->image, rect);
	}
}

static void
show(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		joy_bubble_show(priv->label);
	}
	if (priv->image) {
		joy_bubble_show(priv->image);
	}
}

static void
hide(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->label) {
		joy_bubble_hide(priv->label);
	}
	if (priv->image) {
		joy_bubble_hide(priv->image);
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

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	JoyStyle *style = joy_bubble_get_style(self);
	if (G_UNLIKELY(!style)) {
		return JOY_BUBBLE_CLASS(joy_button_parent_class)->
			draw(self, cr);
	}
	joy_style_draw_background(style, self, cr);
	if (G_LIKELY(JOY_IS_STYLE_BUTTON(style))) {
		struct Private *priv = GET_PRIVATE(self);
		if (priv->image) {
			joy_style_button_draw_image(style, self, cr,
					priv->image);
		}
		if (priv->label) {
			joy_style_button_draw_label(style, self, cr,
					priv->label);
		}
	}
	joy_style_draw_foreground(style, self, cr);
	return TRUE;
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
	bubble_class->expose = expose;
	bubble_class->show = show;
	bubble_class->hide = hide;
	bubble_class->key_down = key_down;
	bubble_class->key_up = key_up;
	bubble_class->button_down = button_down;
	bubble_class->button_up = button_up;
	bubble_class->enter = enter;
	bubble_class->leave = leave;
	bubble_class->draw = draw;
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
	properties[PROP_LABEL] =
		g_param_spec_object("label", Q_("Label"),
			Q_("The button label"), JOY_TYPE_LABEL,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_LABEL,
			properties[PROP_LABEL]);
	properties[PROP_IMAGE] =
		g_param_spec_object("image", Q_("Image"),
			Q_("The image icon"), JOY_TYPE_IMAGE,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_IMAGE,
			properties[PROP_IMAGE]);
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

/**
 * \brief Set a new label for this button.
 *
 * \param self [in] A button object.
 * \param label [in] The new label to set for \e self.
 */
static void
set_label(JoyBubble *self, JoyBubble *label)
{
	struct Private *priv = GET_PRIVATE(self);
	joy_bubble_set_parent(label, self);
	JoyStyle *style = joy_bubble_get_style(self);
	if (style) {
		joy_bubble_set_style(label, style);
		if (JOY_IS_STYLE_BUTTON(style)) {
			joy_style_button_configure_label(style, self, label);
			if (priv->image) {
				joy_style_button_configure_image(style, self,
						priv->image);
			}
		}
	}
	priv->label = label;
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
	if (label) {
		set_label(self, g_object_ref(label));
	} else {
		priv->label = NULL;
	}
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_LABEL]);
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
			JoyBubble *label = joy_label_new(text);
			if (G_UNLIKELY(!label)) {
				return;
			}
			set_label(self, label);
		}
	} else {
		if (priv->label) {
			g_object_unref(priv->label);
			priv->label = NULL;
		}
	}
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_LABEL]);
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
			JoyBubble *label = joy_label_new(NULL);
			if (G_UNLIKELY(!label)) {
				return;
			}
			set_label(self, label);
		}
		joy_label_set_markup(priv->label, markup);
	} else {
		if (priv->label) {
			g_object_unref(priv->label);
			priv->label = NULL;
		}
	}
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_LABEL]);
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
	if (image) {
		JoyStyle *style = joy_bubble_get_style(self);
		if (style && JOY_IS_STYLE_BUTTON(style)) {
			joy_style_button_configure_image(style, self, image);
			if (priv->label) {
				joy_style_button_configure_label(style, self,
						priv->label);
			}
		}
		priv->image = g_object_ref(image);
	} else {
		priv->image = NULL;
	}
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_IMAGE]);
}

JoyBubble *
joy_button_get_image(const JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUTTON(self), NULL);
	return GET_PRIVATE(self)->image;
}
