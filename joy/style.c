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
#include "joy/style.h"

G_DEFINE_ABSTRACT_TYPE(JoyStyle, joy_style, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_STYLE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyStyle *)instance)->priv)

struct Private {
	JoyStyle *parent;
	PangoFontDescription *desc;
	cairo_pattern_t *font;
};

static void
joy_style_init(JoyStyle *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->parent) {
		g_object_unref(priv->parent);
		priv->parent = NULL;
	}
	G_OBJECT_CLASS(joy_style_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->desc) {
		pango_font_description_free(priv->desc);
	}
	if (priv->font) {
		cairo_pattern_destroy(priv->font);
	}
	G_OBJECT_CLASS(joy_style_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_PARENT,
	PROP_FONT_DESCRIPTION,
	PROP_FONT_SOURCE,
	PROP_MAX
};

static GParamSpec *properties[PROP_MAX];

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	JoyStyle *self = JOY_STYLE(base);
	switch (id) {
	case PROP_PARENT:
		joy_style_set_parent(self, g_value_dup_object(value));
		break;
	case PROP_FONT_DESCRIPTION:
		joy_style_set_font_description(self,
				g_value_get_pointer(value));
		break;
	case PROP_FONT_SOURCE:
		joy_style_set_font_source(self, g_value_get_pointer(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	JoyStyle *self = JOY_STYLE(base);
	switch (id) {
	case PROP_FONT_DESCRIPTION:
		g_value_set_pointer(value,
				joy_style_get_font_description(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
on_notify_font_description(JoyStyle *self, GParamSpec *pspec,
		PangoLayout *layout)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->desc)) {
		return;
	}
	pango_layout_set_font_description(layout, priv->desc);
}

static PangoLayout *
pango_layout_create(JoyStyle *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->parent)) {
		return NULL;
	}
	PangoLayout *layout = joy_style_pango_layout_create(priv->parent);
	if (G_UNLIKELY(!layout)) {
		return NULL;
	}
	g_signal_connect(self, "notify::font-description",
			G_CALLBACK(on_notify_font_description), layout);
	if (priv->desc) {
		pango_layout_set_font_description(layout, priv->desc);
	}
	return layout;
}

static gboolean
set_font_source(JoyStyle *self, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		cairo_set_source(cr, priv->font);
		return TRUE;
	}
	if (priv->parent) {
		return joy_style_cairo_set_font_source(priv->parent, cr);
	}
	return FALSE;
}

static void
joy_style_class_init(JoyStyleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	klass->pango_layout_create = pango_layout_create;
	klass->set_font_source = set_font_source;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_PARENT,
		g_param_spec_object("parent", Q_("Parent"),
			Q_("The parent style, i.e., the theme"),
			JOY_TYPE_STYLE,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	properties[PROP_FONT_DESCRIPTION] =
		g_param_spec_pointer("font-description",
			Q_("Font Description"),
			Q_("The font description for this style"),
			G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class, PROP_FONT_DESCRIPTION,
			properties[PROP_FONT_DESCRIPTION]);
	g_object_class_install_property(object_class, PROP_FONT_SOURCE,
		g_param_spec_pointer("font-source", Q_("Font Source"),
			Q_("The font source for this style"),
			G_PARAM_WRITABLE));
}

void
joy_style_set_parent(JoyStyle *self, JoyStyle *parent)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	g_return_if_fail(!parent || JOY_IS_STYLE(parent));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->parent) {
		g_object_unref(priv->parent);
	}
	priv->parent = parent ? g_object_ref(parent) : NULL;
}

JoyStyle *
joy_style_get_parent(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	return GET_PRIVATE(self)->parent;
}

void
joy_style_set_font_description(JoyStyle *self,
		const PangoFontDescription *desc)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->desc) {
		pango_font_description_free(priv->desc);
	}
	priv->desc = desc ? pango_font_description_copy(desc) : NULL;
	g_object_notify_by_pspec(G_OBJECT(self),
			properties[PROP_FONT_DESCRIPTION]);
}

PangoFontDescription *
joy_style_get_font_description(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->desc) {
		return priv->desc;
	} else {
		if (priv->parent) {
			return joy_style_get_font_description(priv->parent);
		}
		return NULL;
	}
}

void
joy_style_set_font_source(JoyStyle *self, cairo_pattern_t *font)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		cairo_pattern_destroy(priv->font);
	}
	priv->font = font ? cairo_pattern_reference(font) : NULL;
}

void
joy_style_set_font_source_rgb(JoyStyle *self, double red, double green,
		double blue)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	cairo_pattern_t *source = cairo_pattern_create_rgb(red, green, blue);
	if (G_UNLIKELY(!source)) {
		return;
	}
	joy_style_set_font_source(self, source);
	cairo_pattern_destroy(source);
}

void
joy_style_set_font_source_rgba(JoyStyle *self, double red, double green,
		double blue, double alpha)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	cairo_pattern_t *source =
		cairo_pattern_create_rgba(red, green, blue, alpha);
	if (G_UNLIKELY(!source)) {
		return;
	}
	joy_style_set_font_source(self, source);
	cairo_pattern_destroy(source);
}

void
joy_style_set_font_source_surface(JoyStyle *self, cairo_surface_t *surface)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	g_return_if_fail(surface);
	cairo_pattern_t *source = cairo_pattern_create_for_surface(surface);
	if (G_UNLIKELY(!source)) {
		return;
	}
	joy_style_set_font_source(self, source);
	cairo_pattern_destroy(source);
}

cairo_pattern_t *
joy_style_get_font_source(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		return priv->font;
	} else {
		if (priv->parent) {
			return joy_style_get_font_source(priv->parent);
		}
		return NULL;
	}
}

PangoLayout *
joy_style_pango_layout_create(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	return JOY_STYLE_GET_CLASS(self)->pango_layout_create(self);
}

gboolean
joy_style_cairo_set_font_source(JoyStyle *self, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), FALSE);
	g_return_val_if_fail(cr, FALSE);
	return JOY_STYLE_GET_CLASS(self)->set_font_source(self, cr);
}

void
joy_style_draw_background(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	g_return_if_fail(JOY_IS_BUBBLE(widget));
	g_return_if_fail(cr);
	JoyStyleClass *klass = JOY_STYLE_GET_CLASS(self);
	if (klass->draw_background) {
		klass->draw_background(self, widget, cr);
	}
}

void
joy_style_draw_foreground(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	g_return_if_fail(JOY_IS_BUBBLE(widget));
	g_return_if_fail(cr);
	JoyStyleClass *klass = JOY_STYLE_GET_CLASS(self);
	if (klass->draw_foreground) {
		klass->draw_foreground(self, widget, cr);
	}
}
