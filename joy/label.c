/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/bubble.h"
#include "joy/label.h"
#include "joy/style.h"
#include <pango/pangocairo.h>

G_DEFINE_TYPE(JoyLabel, joy_label, JOY_TYPE_BUBBLE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_LABEL, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyLabel *)instance)->priv)

struct Private {
	PangoLayout *layout;
	gchar *text;
	gboolean markup;
	PangoWrapMode wrap;
	PangoEllipsizeMode ellipsize;
	PangoAlignment alignment;
	gboolean justify;
};

static void
joy_label_init(JoyLabel *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->wrap = PANGO_WRAP_WORD;
	priv->ellipsize = PANGO_ELLIPSIZE_NONE;
	priv->alignment = PANGO_ALIGN_LEFT;
	priv->justify = FALSE;
}

static void
on_notify(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->text)) {
		return;
	}
	if (G_UNLIKELY(!priv->layout)) {
		priv->layout = joy_bubble_pango_layout_create(self);
		if (G_UNLIKELY(!priv->layout)) {
			return;
		}
	}
	if (priv->markup) {
		pango_layout_set_attributes(priv->layout, NULL);
		pango_layout_set_markup(priv->layout, priv->text, -1);
	} else {
		pango_layout_set_text(priv->layout, priv->text, -1);
	}
	gboolean resize = FALSE;
	gint width = joy_bubble_get_width(self);
	gint height = joy_bubble_get_height(self);
	if (width && height) {
		pango_layout_set_width(priv->layout, width * PANGO_SCALE);
		pango_layout_set_height(priv->layout, height * PANGO_SCALE);
		pango_layout_set_wrap(priv->layout, priv->wrap);
		pango_layout_set_ellipsize(priv->layout, priv->ellipsize);
	} else {
		resize = TRUE;
	}
	pango_layout_set_alignment(priv->layout, priv->alignment);
	pango_layout_set_justify(priv->layout, priv->justify);
	if (resize) {
		pango_layout_get_pixel_size(priv->layout, &width, &height);
		joy_bubble_resize(self, width, height);
	}
}

static void
constructed(GObject *base)
{
	g_signal_connect(base, "notify::style", G_CALLBACK(on_notify), NULL);
	g_signal_connect(base, "notify::parent", G_CALLBACK(on_notify), NULL);
	if (G_OBJECT_CLASS(joy_label_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_label_parent_class)->constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->layout) {
		g_object_unref(priv->layout);
		priv->layout = NULL;
	}
	G_OBJECT_CLASS(joy_label_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->text);
	G_OBJECT_CLASS(joy_label_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_TEXT,
	PROP_MARKUP,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_TEXT:
		joy_label_set_text(self, g_value_get_string(value));
		break;
	case PROP_MARKUP:
		joy_label_set_markup(self, g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->layout)) {
		return;
	}
	pango_layout_set_width(priv->layout, width * PANGO_SCALE);
	pango_layout_set_height(priv->layout, height * PANGO_SCALE);
	pango_layout_set_wrap(priv->layout, priv->wrap);
	pango_layout_set_ellipsize(priv->layout, priv->ellipsize);
}

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->layout)) {
		return FALSE;
	}
	joy_bubble_cairo_set_font_source(self, cr);
	pango_cairo_update_layout(cr, priv->layout);
	pango_cairo_show_layout(cr, priv->layout);
	return TRUE;
}

static void
joy_label_class_init(JoyLabelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->resize = resize;
	bubble_class->draw = draw;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_TEXT,
		g_param_spec_string("text", Q_("Text"),
			Q_("The label text"), NULL, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_MARKUP,
		g_param_spec_string("markup", Q_("Markup"),
			Q_("The label markup"), NULL, G_PARAM_WRITABLE));
}

JoyBubble *
joy_label_new(const gchar *text)
{
	return g_object_new(JOY_TYPE_LABEL,
			"horizontal-expand", FALSE,
			"vertical-expand", FALSE,
			"text", text,
			NULL);
}

void
joy_label_set_text(JoyBubble *self, const gchar *text)
{
	g_return_if_fail(JOY_IS_LABEL(self));
	if (!text) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (priv->layout) {
		if (priv->markup) {
			pango_layout_set_attributes(priv->layout, NULL);
		}
		pango_layout_set_text(priv->layout, text, -1);
	}
	g_free(priv->text);
	priv->text = g_strdup(text);
	priv->markup = FALSE;
}

const gchar *
joy_label_get_text(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_LABEL(self), NULL);
	return GET_PRIVATE(self)->text;
}

void
joy_label_set_markup(JoyBubble *self, const gchar *markup)
{
	g_return_if_fail(JOY_IS_LABEL(self));
	if (!markup) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (priv->layout) {
		pango_layout_set_markup(priv->layout, markup, -1);
	}
	g_free(priv->text);
	priv->text = g_strdup(markup);
	priv->markup = TRUE;
}

gboolean
joy_label_get_markup(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_LABEL(self), FALSE);
	return GET_PRIVATE(self)->markup;
}

void
joy_label_set_wrap(JoyBubble *self, PangoWrapMode wrap)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->layout)) {
		pango_layout_set_wrap(priv->layout, wrap);
	}
	priv->wrap = wrap;
}

PangoWrapMode
joy_label_get_wrap(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), PANGO_WRAP_WORD);
	return GET_PRIVATE(self)->wrap;
}

void
joy_label_set_ellipsize(JoyBubble *self, PangoEllipsizeMode ellipsize)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->layout)) {
		pango_layout_set_ellipsize(priv->layout, ellipsize);
	}
	priv->ellipsize = ellipsize;
}

PangoEllipsizeMode
joy_label_get_ellipsize(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), PANGO_ELLIPSIZE_NONE);
	return GET_PRIVATE(self)->ellipsize;
}

void
joy_label_set_alignment(JoyBubble *self, PangoAlignment alignment)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->layout)) {
		pango_layout_set_alignment(priv->layout, alignment);
	}
	priv->alignment = alignment;
}

PangoAlignment
joy_label_get_alignment(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), PANGO_ALIGN_LEFT);
	return GET_PRIVATE(self)->alignment;
}

void
joy_label_set_justify(JoyBubble *self, gboolean justify)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->layout)) {
		pango_layout_set_justify(priv->layout, justify);
	}
	priv->justify = justify;
}

gboolean
joy_label_get_justify(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), FALSE);
	return GET_PRIVATE(self)->justify;
}
