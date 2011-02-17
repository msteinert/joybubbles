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

G_DEFINE_ABSTRACT_TYPE(JoyTheme, joy_theme, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_THEME, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyTheme *)instance)->priv)

struct Private {
	GHashTable *styles;
};

static void
joy_theme_init(JoyTheme *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->styles = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, NULL);
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

static void
joy_theme_class_init(JoyThemeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

void
joy_theme_set_style(JoyTheme *self, const gchar *name, GType style)
{
	g_return_if_fail(JOY_IS_THEME(self));
	g_return_if_fail(name);
	g_return_if_fail(style);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->styles)) {
		return;
	}
	g_hash_table_insert(priv->styles, g_strdup(name),
			GSIZE_TO_POINTER(style));
}

JoyStyle *
joy_theme_get_style(JoyTheme *self, JoyBubble *widget)
{
	g_return_val_if_fail(JOY_IS_THEME(self), NULL);
	g_return_val_if_fail(JOY_IS_BUBBLE(widget), NULL);
	return JOY_THEME_GET_CLASS(self)->get_style(self, widget);
}
