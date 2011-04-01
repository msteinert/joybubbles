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
#include "joy/theme.h"

G_DEFINE_ABSTRACT_TYPE(JoyStyle, joy_style, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_STYLE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyStyle *)instance)->priv)

struct Private {
	JoyTheme *theme;
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
	if (priv->theme) {
		g_object_unref(priv->theme);
		priv->theme = NULL;
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
	PROP_THEME,
	PROP_FONT_DESCRIPTION,
	PROP_FONT_PATTERN,
	PROP_MAX
};

static GParamSpec *properties[PROP_MAX];

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_THEME:
		priv->theme = g_value_dup_object(value);
		break;
	case PROP_FONT_DESCRIPTION:
		joy_style_set_font_description(JOY_STYLE(base),
				g_value_get_pointer(value));
		break;
	case PROP_FONT_PATTERN:
		joy_style_set_font_pattern(JOY_STYLE(base),
				g_value_get_pointer(value));
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
	case PROP_FONT_DESCRIPTION:
		g_value_set_pointer(value, priv->desc);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_style_class_init(JoyStyleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_THEME,
		g_param_spec_object("theme", Q_("Theme"),
			Q_("The theme this style belongs to"), JOY_TYPE_THEME,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	properties[PROP_FONT_DESCRIPTION] =
		g_param_spec_pointer("font-description",
			Q_("Font Description"),
			Q_("The font description for this style"),
			G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class, PROP_FONT_DESCRIPTION,
			properties[PROP_FONT_DESCRIPTION]);
	g_object_class_install_property(object_class, PROP_FONT_PATTERN,
		g_param_spec_pointer("font-pattern", Q_("Font Pattern"),
			Q_("The font pattern for this style"),
			G_PARAM_WRITABLE));
}

gboolean
joy_style_draw(JoyStyle *self, JoyBubble *widget, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), FALSE);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), FALSE);
	g_return_val_if_fail(cr, FALSE);
	return JOY_STYLE_GET_CLASS(self)->draw(self, widget, cr);
}

JoyTheme *
joy_style_get_theme(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	return GET_PRIVATE(self)->theme;
}

PangoFontDescription *
joy_style_get_font_description(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	return GET_PRIVATE(self)->desc;
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
	if (desc) {
		priv->desc = pango_font_description_copy(desc);
	} else {
		priv->desc = NULL;
	}
	g_object_notify_by_pspec(G_OBJECT(self),
			properties[PROP_FONT_DESCRIPTION]);
}

cairo_pattern_t *
joy_style_get_font_pattern(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	return GET_PRIVATE(self)->font;
}

void
joy_style_set_font_pattern(JoyStyle *self, cairo_pattern_t *font)
{
	g_return_if_fail(JOY_IS_STYLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		cairo_pattern_destroy(priv->font);
	}
	if (font) {
		priv->font = cairo_pattern_reference(font);
	} else {
		priv->font = NULL;
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

PangoLayout *
joy_style_pango_layout_create(JoyStyle *self)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->theme)) {
		return NULL;
	}
	PangoLayout *layout = joy_theme_pango_layout_create(priv->theme);
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

gboolean
joy_style_cairo_set_source_font(JoyStyle *self, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_STYLE(self), FALSE);
	g_return_val_if_fail(cr, FALSE);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		cairo_set_source(cr, priv->font);
	} else {
		if (G_UNLIKELY(!priv->theme)) {
			return FALSE;
		}
		return joy_theme_cairo_set_source_font(priv->theme, cr);
	}
	return TRUE;
}
