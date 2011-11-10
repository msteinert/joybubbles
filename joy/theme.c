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
#include "joy/theme.h"
#include <pango/pangocairo.h>

G_DEFINE_ABSTRACT_TYPE(JoyTheme, joy_theme, JOY_TYPE_STYLE)

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
destroy(gpointer object)
{
	g_object_run_dispose(object);
	g_object_unref(object);
}

static void
joy_theme_init(JoyTheme *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->styles = g_hash_table_new_full(g_str_hash, g_str_equal,
			NULL, destroy);
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
	if (priv->styles) {
		g_hash_table_destroy(priv->styles);
		priv->styles = NULL;
	}
	G_OBJECT_CLASS(joy_theme_parent_class)->dispose(base);
}

static void
on_context_changed(JoyStyle *theme, PangoLayout *layout)
{
	pango_layout_context_changed(layout);
}

static PangoLayout *
pango_layout_create(JoyStyle *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->context)) {
		goto error;
	}
	PangoLayout *layout = pango_layout_new(priv->context);
	if (G_UNLIKELY(!layout)) {
		goto error;
	}
	g_signal_connect(self, "context-changed",
			G_CALLBACK(on_context_changed), layout);
exit:
	{
		PangoFontDescription *desc =
			joy_style_get_font_description(self);
		if (desc) {
			pango_layout_set_font_description(layout, desc);
		}
		return layout;
	}
error:
	{
		JoyStyle *parent = joy_style_get_parent(self);
		if (!parent) {
			return NULL;
		}
		layout = joy_style_pango_layout_create(parent);
		goto exit;
	}
}

static void
joy_theme_class_init(JoyThemeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	JoyStyleClass *style_class = JOY_STYLE_CLASS(klass);
	style_class->pango_layout_create = pango_layout_create;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyTheme::context-changed
	signals[SIGNAL_CONTEXT_CHANGED] =
		g_signal_new(g_intern_static_string("context-changed"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST, 0,
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

void
joy_theme_set_style(JoyStyle *self, const gchar *name, JoyStyle *style)
{
	g_return_if_fail(JOY_IS_THEME(self));
	g_return_if_fail(name);
	g_return_if_fail(JOY_IS_STYLE(style));
	joy_style_set_parent(style, self);
	g_hash_table_insert(GET_PRIVATE(self)->styles, (gpointer)name, style);
}

JoyStyle *
joy_theme_get_style(JoyStyle *self, JoyBubble *widget)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	struct Private *priv = GET_PRIVATE(self);
	const gchar *name = joy_bubble_get_name(widget);
	if (G_LIKELY(!name)) {
		name = G_OBJECT_TYPE_NAME(widget);
		if (G_UNLIKELY(!name)) {
			return NULL;
		}
	}
	JoyStyle *style = g_hash_table_lookup(priv->styles, name);
	if (G_LIKELY(style)) {
		return style;
	}
	style = joy_theme_style_create(self, widget);
	if (G_UNLIKELY(!style)) {
		JoyStyle *parent = joy_style_get_parent(self);
		if (!parent || G_UNLIKELY(!JOY_IS_THEME(parent))) {
			return NULL;
		}
		style = joy_theme_get_style(parent, widget);
		if (G_UNLIKELY(!style)) {
			return NULL;
		}
	}
	g_hash_table_insert(priv->styles, (gpointer)name, style);
	return style;
}

JoyStyle *
joy_theme_style_create(JoyStyle *self, JoyBubble *widget)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	return JOY_THEME_GET_CLASS(self)->style_create(self, widget);
}

