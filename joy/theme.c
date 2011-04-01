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
#include "joy/marshal.h"
#include "joy/style.h"
#include "joy/theme.h"
#include <pango/pangocairo.h>

G_DEFINE_ABSTRACT_TYPE(JoyTheme, joy_theme, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_THEME, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyTheme *)instance)->priv)

enum Signals {
	SIGNAL_CONTEXT_CHANGED,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	GHashTable *styles;
	PangoContext *context;
	cairo_pattern_t *font;
};

static void
joy_theme_init(JoyTheme *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->styles = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, g_object_unref);
	PangoFontMap *map = pango_cairo_font_map_get_default();
	if (G_LIKELY(map)) {
		priv->context = pango_font_map_create_context(map);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->context) {
		g_object_unref(priv->context);
		priv->context = NULL;
	}
	G_OBJECT_CLASS(joy_theme_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->styles) {
		g_hash_table_destroy(priv->styles);
	}
	G_OBJECT_CLASS(joy_theme_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_FONT_DESCRIPTION,
	PROP_FONT_PATTERN,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	switch (id) {
	case PROP_FONT_DESCRIPTION:
		joy_theme_set_font_description((JoyTheme *)base,
				g_value_get_pointer(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_theme_class_init(JoyThemeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyTheme::context-changed
	signals[SIGNAL_CONTEXT_CHANGED] =
		g_signal_new(g_intern_static_string("context-changed"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST, 0,
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	g_object_class_install_property(object_class, PROP_FONT_DESCRIPTION,
		g_param_spec_pointer("font-description",
		Q_("Font Description"),
		Q_("The default font description for this theme"),
		G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyStyle *
joy_theme_get_style(JoyTheme *self, JoyBubble *widget)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	struct Private *priv = GET_PRIVATE(self);
	const gchar *name = joy_bubble_get_name(widget);
	if (G_UNLIKELY(!name)) {
		return NULL;
	}
	JoyStyle *style = g_hash_table_lookup(priv->styles, name);
	if (G_LIKELY(style)) {
		return style;
	}
	style = JOY_THEME_GET_CLASS(self)->get_style(self, widget);
	if (G_UNLIKELY(!style)) {
		return NULL;
	}
	g_hash_table_insert(priv->styles, (gpointer)name, style);
	return style;
}

void
joy_theme_set_font_description(JoyTheme *self,
		const PangoFontDescription *desc)
{
	g_return_if_fail(JOY_IS_THEME(self));
	g_return_if_fail(desc);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->context)) {
		return;
	}
	pango_context_set_font_description(priv->context, desc);
	g_signal_emit(self, signals[SIGNAL_CONTEXT_CHANGED], 0);
}

static void
on_context_changed(JoyTheme *theme, PangoLayout *layout)
{
	pango_layout_context_changed(layout);
}

PangoLayout *
joy_theme_pango_layout_create(JoyTheme *self)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->context)) {
		return NULL;
	}
	PangoLayout *layout = pango_layout_new(priv->context);
	if (G_UNLIKELY(!layout)) {
		return NULL;
	}
	g_signal_connect(self, "context-changed",
			G_CALLBACK(on_context_changed), layout);
	return layout;
}

gboolean
joy_theme_cairo_set_source_font(JoyTheme *self, cairo_t *cr)
{
	g_return_val_if_fail(JOY_IS_THEME(self), FALSE);
	g_return_val_if_fail(cr, FALSE);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->font) {
		cairo_set_source(cr, priv->font);
		return TRUE;
	}
	return FALSE;
}
