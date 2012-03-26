/* Copyright 2011, 2012 Michael Steinert
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the Joybubbles project nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/bubble.h"
#include "joy/theme/default/button.h"
#include "joy/theme/default/default.h"

G_DEFINE_TYPE(JoyThemeDefault, joy_theme_default, JOY_TYPE_THEME)

static void
joy_theme_default_init(G_GNUC_UNUSED JoyThemeDefault *self)
{
}

static void
constructed(GObject *base)
{
	JoyStyle *self = JOY_STYLE(base);
	PangoFontDescription *desc = pango_font_description_new();
	if (G_LIKELY(desc)) {
		pango_font_description_set_family(desc, "Droid Sans");
		pango_font_description_set_size(desc, 12 * PANGO_SCALE);
		joy_style_set_font_description(self, desc);
		pango_font_description_free(desc);
	}
	if (G_OBJECT_CLASS(joy_theme_default_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_theme_default_parent_class)->
			constructed(base);
	}
}

static JoyStyle *
style_create(G_GNUC_UNUSED JoyStyle *self, JoyBubble *widget)
{
	const gchar *name = joy_bubble_get_name(widget);
	if (G_LIKELY(!name)) {
		name = G_OBJECT_TYPE_NAME(widget);
		if (G_UNLIKELY(!name)) {
			return NULL;
		}
	}
	if (!g_strcmp0("JoyButton", name)) {
		return joy_style_default_button_new(widget);
	}
	return NULL;
}

static void
joy_theme_default_class_init(JoyThemeDefaultClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	JoyThemeClass *theme_class = JOY_THEME_CLASS(klass);
	theme_class->style_create = style_create;
}

JoyStyle *
joy_theme_default_new(void)
{
	return g_object_new(JOY_TYPE_THEME_DEFAULT,
			NULL);
}
