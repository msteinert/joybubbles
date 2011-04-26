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
#include "joy/theme/default/button.h"
#include "joy/theme/default/default.h"

G_DEFINE_TYPE(JoyThemeDefault, joy_theme_default, JOY_TYPE_THEME)

static void
joy_theme_default_init(JoyThemeDefault *self)
{
}

static JoyStyle *
style_create(JoyStyle *self, JoyBubble *widget)
{
	const gchar *name = joy_bubble_get_name(widget);
	if (G_LIKELY(!name)) {
		name = G_OBJECT_TYPE_NAME(widget);
		if (G_UNLIKELY(!name)) {
			return NULL;
		}
	}
	if (!g_strcmp0("JoyButton", name)) {
		return joy_style_default_button_new();
	}
	return NULL;
}

static void
joy_theme_default_class_init(JoyThemeDefaultClass *klass)
{
	JoyThemeClass *theme_class = JOY_THEME_CLASS(klass);
	theme_class->style_create = style_create;
}

JoyStyle *
joy_theme_default_new(void)
{
	return g_object_new(JOY_TYPE_THEME_DEFAULT,
			NULL);
}
