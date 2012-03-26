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
#include "joy/cursor.h"

G_DEFINE_TYPE(JoyCursor, joy_cursor, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_CURSOR, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyCursor *)instance)->priv)

struct Private {
	cairo_surface_t *image;
	gint x;
	gint y;
};

static void
joy_cursor_init(JoyCursor *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->image) {
		cairo_surface_destroy(priv->image);
	}
	G_OBJECT_CLASS(joy_cursor_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_IMAGE,
	PROP_X_HOTSPOT,
	PROP_Y_HOTSPOT,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyCursor *self = (JoyCursor *)base;
	switch (id) {
	case PROP_IMAGE:
		joy_cursor_set_image(self, g_value_get_pointer(value));
		break;
	case PROP_X_HOTSPOT:
		priv->x = g_value_get_int(value);
		break;
	case PROP_Y_HOTSPOT:
		priv->y = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_cursor_class_init(JoyCursorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_IMAGE,
		g_param_spec_pointer("image", Q_("Image"),
			Q_("The image for this cursor"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT));
	g_object_class_install_property(object_class, PROP_X_HOTSPOT,
		g_param_spec_int("x-hotspot", Q_("X Hotspot"),
			Q_("The X-axis hotspot for this cursor"),
			G_MININT, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_Y_HOTSPOT,
		g_param_spec_int("y-hotspot", Q_("Y Hotspot"),
			Q_("The Y-axis hotspot for this cursor"),
			G_MININT, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyCursor *
joy_cursor_new(cairo_surface_t *image, gint x, gint y)
{
	g_return_val_if_fail(image, NULL);
	JoyCursor *self = g_object_new(JOY_TYPE_CURSOR,
			"image", image,
			"x-hotspot", x,
			"y-hotspot", y,
			NULL);
	return self;
}

void
joy_cursor_set_image(JoyCursor *self, cairo_surface_t *image)
{
	g_return_if_fail(JOY_IS_CURSOR(self));
	g_return_if_fail(image);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->image) {
		cairo_surface_destroy(priv->image);
	}
	priv->image = cairo_surface_reference(image);
}

cairo_surface_t *
joy_cursor_get_image(JoyCursor *self)
{
	g_return_val_if_fail(JOY_IS_CURSOR(self), NULL);
	return GET_PRIVATE(self)->image;
}

gint
joy_cursor_get_hotspot_x(JoyCursor *self)
{
	g_return_val_if_fail(JOY_IS_CURSOR(self), 0);
	return GET_PRIVATE(self)->x;
}

gint
joy_cursor_get_hotspot_y(JoyCursor *self)
{
	g_return_val_if_fail(JOY_IS_CURSOR(self), 0);
	return GET_PRIVATE(self)->y;
}
