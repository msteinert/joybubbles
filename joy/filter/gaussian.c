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
#include "joy/filter/gaussian.h"
#include <math.h>
#include <pixman.h>

G_DEFINE_TYPE(JoyFilterGaussian, joy_filter_gaussian, JOY_TYPE_FILTER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_FILTER_GAUSSIAN, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyFilterGaussian *)instance)->priv)

struct Private {
	pixman_fixed_t *kernel;
	gdouble radius;
	gint n;
};

static void
joy_filter_gaussian_init(JoyFilterGaussian *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->kernel);
	G_OBJECT_CLASS(joy_filter_gaussian_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_RADIUS,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	switch (id) {
	case PROP_RADIUS:
		joy_filter_gaussian_set_radius(JOY_FILTER(base),
				g_value_get_double(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
apply(JoyFilter *self, JoyBubble *widget, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->kernel || !priv->n)) {
		return;
	}
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	cairo_surface_t *surface = cairo_get_target(cr);
	cairo_surface_type_t type = cairo_surface_get_type(surface);
	if (CAIRO_SURFACE_TYPE_IMAGE != type) {
		cairo_surface_t *image_surface = cairo_image_surface_create(
				CAIRO_FORMAT_ARGB32, width, height);
		cairo_t *image_cr = cairo_create(surface);
		cairo_set_source_surface(image_cr, surface, 0., 0.);
		cairo_paint(image_cr);
		cairo_destroy(image_cr);
		cairo_surface_flush(image_surface);
		surface = image_surface;
	}
	gint stride = cairo_image_surface_get_stride(surface);
	gpointer pixels = cairo_image_surface_get_data(surface);
	pixman_image_t *image = pixman_image_create_bits(PIXMAN_a8r8g8b8,
			width, height, pixels, stride);
	if (!image) {
		goto error;
	}
	if (!pixman_image_set_filter(image, PIXMAN_FILTER_CONVOLUTION,
			priv->kernel, priv->n)) {
		goto error;
	}
	pixman_image_composite(PIXMAN_OP_SRC, image, NULL, image,
			0, 0, 0, 0, 0, 0, width, height);
	pixman_image_unref(image);
	if (CAIRO_SURFACE_TYPE_IMAGE != type) {
		cairo_set_source_surface(cr, surface, 0., 0.);
		cairo_paint(cr);
		cairo_surface_destroy(surface);
	}
	return;
error:
	if (CAIRO_SURFACE_TYPE_IMAGE != type) {
		cairo_surface_destroy(surface);
	}
}

static void
joy_filter_gaussian_class_init(JoyFilterGaussianClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	JoyFilterClass *filter_class = JOY_FILTER_CLASS(klass);
	filter_class->apply = apply;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_RADIUS,
		g_param_spec_double("radius", Q_("Radius"),
			Q_("The blur radius."), 0., G_MAXDOUBLE, 0.,
			G_PARAM_WRITABLE));
}

JoyFilter *
joy_filter_gaussian_new(gdouble radius)
{
	return g_object_new(JOY_TYPE_FILTER_GAUSSIAN,
			"radius", radius,
			NULL);
}

void
joy_filter_gaussian_set_radius(JoyFilter *self, gdouble radius)
{
	g_return_if_fail(JOY_IS_FILTER_GAUSSIAN(self));
	struct Private *priv = GET_PRIVATE(self);
	g_free(priv->kernel);
	gdouble fradius = fabs(radius) + 1.;
	gdouble sigma = sqrt(-(fradius * fradius) / (2. * log(1. / 255.)));
	const gdouble s2 = 2. * sigma * sigma;
	const gdouble s1 = 1. / (G_PI * s2);
	const gint size = 2 * radius + 1;
	gint n = size * size;
	gdouble kernel[n], sum;
	gint i, x, y;
	for (i = 0, sum = 0, x = -radius; x <= radius; ++x) {
		for (y = -radius; y <= radius; ++y, ++i) {
			const gdouble u = x * x;
			const gdouble v = y * y;
			kernel[i] = s1 * exp(-(u + v) / s2);
			sum += kernel[i];
		}
	}
	priv->kernel = g_new(pixman_fixed_t, n + 2);
	priv->kernel[0] = priv->kernel[1] = pixman_int_to_fixed(size);
	for (i = 2; i < n; ++i) {
		priv->kernel[i] = pixman_double_to_fixed(kernel[i] / sum);
	}
	priv->n = n + 2;
	priv->radius = radius;
}

gdouble
joy_filter_gaussian_get_radius(JoyFilter *self)
{
	g_return_val_if_fail(JOY_IS_FILTER_GAUSSIAN(self), 0.);
	return GET_PRIVATE(self)->radius;
}
