/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "joy/image.h"
#include "joy/screen.h"
#include <math.h>

G_DEFINE_TYPE(JoyImage, joy_image, JOY_TYPE_BUBBLE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_IMAGE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyImage *)instance)->priv)

struct Private {
	gchar *filename;
	cairo_surface_t *surface;
	cairo_surface_t *image;
	gint width, height;
	gdouble dx, dy;
};

static void
joy_image_init(JoyImage *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->dx = 1.;
	priv->dy = 1.;
}

static void
on_notify(JoyBubble *self)
{
	joy_image_get_surface(self);
}

static void
constructed(GObject *base)
{
	g_signal_connect(base, "notify::parent", G_CALLBACK(on_notify), NULL);
	if (G_OBJECT_CLASS(joy_image_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_image_parent_class)->constructed(base);
	}
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->filename);
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
	}
	if (priv->image) {
		cairo_surface_destroy(priv->surface);
	}
	G_OBJECT_CLASS(joy_image_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_FILENAME,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_FILENAME:
		priv->filename = g_value_dup_string(value);
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
	JOY_BUBBLE_CLASS(joy_image_parent_class)->resize(self, width, height);
	if (G_LIKELY(width != priv->width)) {
		priv->dx = (gdouble)width / priv->width;
	} else {
		priv->dx = 1.;
	}
	if (G_LIKELY(height != priv->height)) {
		priv->dy = (gdouble)height / priv->height;
	} else {
		priv->dy = 1.;
	}
}

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	cairo_surface_t *surface = priv->surface;
	if (G_UNLIKELY(!surface)) {
		return FALSE;
	}
	if (1. != priv->dx || 1. != priv->dy) {
		cairo_scale(cr, priv->dx, priv->dy);
	}
	cairo_set_source_surface(cr, surface, 0., 0.);
	cairo_paint(cr);
	return TRUE;
}

static void
joy_image_class_init(JoyImageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->resize = resize;
	bubble_class->draw = draw;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_FILENAME,
		g_param_spec_string("filename", Q_("Filename"),
			Q_("The name of the image file"), NULL,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyBubble *
joy_image_new(const gchar *filename)
{
	return g_object_new(JOY_TYPE_IMAGE,
			"horizontal-expand", FALSE,
			"vertical-expand", FALSE,
			"filename", filename,
			NULL);
}

void
joy_image_set_surface(JoyBubble *self, cairo_surface_t *surface,
		gint width, gint height)
{
	g_return_if_fail(JOY_IS_IMAGE(self));
	struct Private *priv = GET_PRIVATE(self);
	g_free(priv->filename);
	priv->filename = NULL;
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
	}
	if (priv->image) {
		cairo_surface_destroy(priv->image);
		priv->image = NULL;
	}
	if (surface && 0 < width && 0 < height) {
		JoyScreen *screen = joy_bubble_get_screen(self);
		if (G_UNLIKELY(!screen)) {
			goto error;
		}
		cairo_surface_type_t type = cairo_surface_get_type(surface);
		cairo_surface_type_t screen_type =
			joy_screen_cairo_surface_type(screen);
		if (type == screen_type) {
			priv->surface = cairo_surface_reference(surface);
			priv->width = width;
			priv->height = height;
			return;
		}
		priv->surface = joy_screen_cairo_surface_create(screen,
				width, height);
		if (G_UNLIKELY(!priv->surface)) {
			goto error;
		}
		cairo_t *cr = cairo_create(priv->surface);
		cairo_status_t status = cairo_status(cr);
		if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
			goto error;
		}
		cairo_set_source_surface(cr, surface, 0., 0.);
		cairo_paint(cr);
		cairo_destroy(cr);
		return;
error:
		if (priv->surface) {
			cairo_surface_destroy(priv->surface);
			priv->surface = NULL;
		}
		priv->image = cairo_surface_reference(surface);
		priv->width = width;
		priv->height = height;
	} else {
		priv->surface = NULL;
		priv->width = 0;
		priv->height = 0; 
	}
	if (!joy_bubble_get_width(self) || !joy_bubble_get_height(self)) {
		joy_bubble_resize(self, priv->width, priv->height);
	}
}

static inline gint
premultiply(gint alpha, gint color)
{
	gint tmp = color * alpha + 0x80;
	return ((tmp >> 8) + tmp) >> 8;
}

static void
get_image_surface(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->image) {
		return;
	}
	if (!priv->filename) {
		return;
	}
	GError *error = NULL;
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(priv->filename, &error);
	if (G_UNLIKELY(!pixbuf)) {
		g_message("%s: %s", priv->filename, error->message);
		g_free(priv->filename);
		priv->filename = NULL;
		g_error_free(error);
		return;
	}
	cairo_format_t format;
	gint n = gdk_pixbuf_get_n_channels(pixbuf);
	switch (n) {
	case 3:
		format = CAIRO_FORMAT_RGB24;
		break;
	case 4:
		format = CAIRO_FORMAT_ARGB32;
		break;
	default:
		goto error;
	}
	priv->width = gdk_pixbuf_get_width(pixbuf);
	priv->height = gdk_pixbuf_get_height(pixbuf);
	gint gdk_stride = gdk_pixbuf_get_rowstride(pixbuf);
	guchar *gdk_pixels = gdk_pixbuf_get_pixels(pixbuf);
	priv->image = cairo_image_surface_create(format, priv->width,
			priv->height);
	cairo_status_t status = cairo_surface_status(priv->image);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		goto error;
	}
	gint cairo_stride = cairo_image_surface_get_stride(priv->image);
	guchar *cairo_pixels = cairo_image_surface_get_data(priv->image);
	cairo_surface_flush(priv->image);
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
#define A (3)
#define R (2)
#define G (1)
#define B (0)
#else
#define A (0)
#define R (1)
#define G (2)
#define B (3)
#endif
	guchar *in, *out;
	for (gint i = 0; i < priv->height; ++i) {
		in = gdk_pixels + i * gdk_stride;
		out = cairo_pixels + i * cairo_stride;
		for (gint j = 0; j < priv->width; ++j) {
			switch (n) {
			case 4:
				out[A] = in[3];
				switch (out[A]) {
				case 0x0:
					out[R] = out[G] = out[B] = 0;
					break;
				case 0xff:
					out[R] = in[0];
					out[G] = in[1];
					out[B] = in[2];
					break;
				default:
					out[R] = premultiply(out[A], in[0]);
					out[G] = premultiply(out[A], in[1]);
					out[B] = premultiply(out[A], in[2]);
					break;
				}
				in += 4;
				break;
			default: // 3
				out[R] = in[0];
				out[G] = in[1];
				out[B] = in[2];
				in += 3;
				break;
			}
			out += 4;
		}
	}
#undef A
#undef R
#undef G
#undef B
	cairo_surface_mark_dirty(priv->image);
exit:
	if (pixbuf) {
		g_object_unref(pixbuf);
	}
	return;
error:
	if (priv->image) {
		cairo_surface_destroy(priv->image);
		priv->image = NULL;
	}
	goto exit;
}

cairo_surface_t *
joy_image_get_surface(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_IMAGE(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->surface)) {
		if (!priv->image) {
			get_image_surface(self);
			if (G_UNLIKELY(!priv->image)) {
				goto exit;
			}
		}
		if (!joy_bubble_get_width(self)
				|| !joy_bubble_get_height(self)) {
			joy_bubble_resize(self, priv->width, priv->height);
		}
		JoyScreen *screen = joy_bubble_get_screen(self);
		if (G_UNLIKELY(!screen)) {
			goto exit;
		}
		priv->surface = joy_screen_cairo_surface_create(screen,
				priv->width, priv->height);
		if (G_UNLIKELY(!priv->surface)) {
			goto exit;
		}
		cairo_t *cr = cairo_create(priv->surface);
		cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
		cairo_set_source_surface(cr, priv->image, 0., 0.);
		cairo_paint(cr);
		cairo_destroy(cr);
		cairo_surface_destroy(priv->image);
		priv->image = NULL;
	}
exit:
	return priv->surface ? priv->surface : priv->image;
}

gint
joy_image_get_width(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_IMAGE(self), 0);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->surface)) {
		joy_image_get_surface(self);
	}
	return priv->width;
}

gint
joy_image_get_height(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_IMAGE(self), 0);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->surface)) {
		joy_image_get_surface(self);
	}
	return priv->height;
}
