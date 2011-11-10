/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cairo.h>
#include "joy/animation/fade.h"
#include "joy/macros.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/window.h"

G_DEFINE_TYPE(JoyGfx3dWindow, joy_gfx3d_window, JOY_TYPE_WINDOW)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_WINDOW, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dWindow *)instance)->priv)

struct Private {
	GFX3D_Image image;
	cairo_surface_t *surface;
	cairo_region_t *area;
	cairo_region_t *expose;
	JoyAnimation *fade;
};

static void
joy_gfx3d_window_init(JoyGfx3dWindow *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->expose = cairo_region_create();
	priv->area = cairo_region_create();
	priv->fade = joy_animation_fade_new((JoyBubble *)self, 1.);
	if (priv->fade) {
		joy_animation_set_duration(priv->fade, 0.1);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->fade) {
		g_object_unref(priv->fade);
		priv->fade = NULL;
	}
	G_OBJECT_CLASS(joy_gfx3d_window_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->expose) {
		cairo_region_destroy(priv->expose);
	}
	if (priv->area) {
		cairo_region_destroy(priv->area);
	}
	if (priv->surface) {
		cairo_surface_destroy(priv->surface);
	}
	G_OBJECT_CLASS(joy_gfx3d_window_parent_class)->finalize(base);
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->surface)) {
		cairo_surface_destroy(priv->surface);
		priv->surface = NULL;
		priv->image = NULL;
	}
	cairo_region_subtract(priv->area, priv->area);
	JOY_BUBBLE_CLASS(joy_gfx3d_window_parent_class)->
		resize(self, width, height);
}

static void
expose(JoyBubble *self, const cairo_rectangle_int_t *rect)
{
	struct Private *priv = GET_PRIVATE(self);
	if (0 < rect->width && 0 < rect->height) {
		cairo_region_union_rectangle(priv->expose, rect);
	}
	JOY_BUBBLE_CLASS(joy_gfx3d_window_parent_class)->expose(self, rect);
}

static void
show(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	JOY_BUBBLE_CLASS(joy_gfx3d_window_parent_class)->show(self);
	if (priv->fade) {
		joy_animation_fade_set_alpha(priv->fade, 1.);
		joy_animation_start(priv->fade);
	}
	cairo_rectangle_int_t rect = {
		0, 0,
		joy_bubble_get_width(self),
		joy_bubble_get_height(self)
	};
	joy_bubble_expose(self, &rect);
}

static cairo_surface_t *
cairo_surface_create(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->surface)) {
		return cairo_surface_reference(priv->surface);
	}
	cairo_rectangle_int_t rect = {
		0, 0,
		joy_bubble_get_width(self),
		joy_bubble_get_height(self)
	};
	cairo_region_union_rectangle(priv->area, &rect);
	// create a GFX3D native surface
	JoyScreen *screen = joy_bubble_get_screen(self);
	GFX3D_Display display = joy_gfx3d_screen_get_display(screen);
	priv->image = GFX3D_Image_Create_Generic(display, rect.width,
			rect.height);
	if (G_UNLIKELY(!priv->image)) {
		goto error;
	}
	// create a cairo surface
	priv->surface = joy_gfx3d_screen_cairo_surface_create(screen,
			priv->image);
	if (G_UNLIKELY(!priv->surface)) {
		goto error;
	}
	return cairo_surface_reference(priv->surface);
error:
	if (priv->image) {
		GFX3D_Image_Destroy(display, priv->image);
		priv->image = NULL;
	}
	return NULL;
}

static void
raise_(JoyBubble *self)
{
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_raise_window(screen, self);
	}
}

static void
lower(JoyBubble *self)
{
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_gfx3d_screen_lower_window(screen, self);
	}
}

static void
joy_gfx3d_window_class_init(JoyGfx3dWindowClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->resize = resize;
	bubble_class->expose = expose;
	bubble_class->show = show;
	JoyWindowClass *window_class = JOY_WINDOW_CLASS(klass);
	window_class->cairo_surface_create = cairo_surface_create;
	window_class->raise = raise_;
	window_class->lower = lower;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyBubble *
joy_gfx3d_window_new(JoyScreen *screen)
{
	g_return_val_if_fail(JOY_IS_GFX3D_SCREEN(screen), NULL);
	return g_object_new(JOY_TYPE_GFX3D_WINDOW,
			"screen", screen,
			"x-coordinate", 0,
			"y-coordinate", 0,
			"width", joy_screen_get_width(screen),
			"height", joy_screen_get_height(screen),
			"horizontal-expand", FALSE,
			"vertical-expand", FALSE,
			NULL);
}

JOY_GNUC_HOT
void
joy_gfx3d_window_submit(JoyBubble *self, GFX3D_NATIVE_Display display,
		GFX3D_NATIVE_Surface fb, cairo_region_t *area)
{
	g_return_if_fail(JOY_IS_GFX3D_WINDOW(self));
	g_return_if_fail(display);
	g_return_if_fail(fb);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->image)) {
		return;
	}
	gint x = joy_bubble_get_x(self);
	gint y = joy_bubble_get_y(self);
	cairo_region_intersect(priv->expose, priv->area);
	cairo_region_intersect(priv->expose, area);
	if (cairo_region_is_empty(priv->expose)) {
		return;
	}
	GFX3D_NATIVE_Surface surface =
		GFX3D_Image_Get_NATIVE_Surface(priv->image);
	gint n = cairo_region_num_rectangles(priv->expose);
	gdouble alpha = joy_bubble_get_alpha(self);
	if (1. == alpha) {
		GFX3D_NATIVE_Blit_Blend_Generic_Parms_t parameters = {
			{
				GFX3D_NATIVE_BLIT_BLEND_CONST_eOne,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eSourceColor,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eDestinationColor,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eInverseSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eZero
			},
			{
				GFX3D_NATIVE_BLIT_BLEND_CONST_eOne,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eDestinationAlpha,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eInverseSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eZero,
			},
			0,
			0,
			0,
			GFX3D_NATIVE_BLIT_FilterCoeffs_eNone,
			GFX3D_NATIVE_BLIT_FilterCoeffs_eNone,
			0,
			0
		};
		for (gint i = 0; i < n; ++i) {
			cairo_rectangle_int_t rect;
			cairo_region_get_rectangle(priv->expose, i, &rect);
			GFX3D_NATIVE_Rect dst = {
				rect.x + x,
				rect.y + y,
				rect.width,
				rect.height
			};
			GFX3D_NATIVE_Blit_Blend_Generic(display,
					fb, &dst,
					surface, (gpointer)&rect,
					fb, &dst,
					&parameters);
		}
	} else {
		GFX3D_NATIVE_Blit_Blend_Generic_Parms_t parameters = {
			{
				GFX3D_NATIVE_BLIT_BLEND_CONST_eConstantAlpha,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eSourceColor,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eDestinationColor,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eInverseSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eZero
			},
			{
				GFX3D_NATIVE_BLIT_BLEND_CONST_eOne,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eDestinationAlpha,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eInverseSourceAlpha,
				0,
				GFX3D_NATIVE_BLIT_BLEND_CONST_eZero,
			},
			(gint)(0xff * alpha) << 24,
			0,
			0,
			GFX3D_NATIVE_BLIT_FilterCoeffs_eNone,
			GFX3D_NATIVE_BLIT_FilterCoeffs_eNone,
			0,
			0
		};
		for (gint i = 0; i < n; ++i) {
			cairo_rectangle_int_t rect;
			cairo_region_get_rectangle(priv->expose, i, &rect);
			GFX3D_NATIVE_Rect dst = {
				rect.x + x,
				rect.y + y,
				rect.width,
				rect.height
			};
			GFX3D_NATIVE_Blit_Blend_Generic(display,
					fb, &dst,
					surface, (gpointer)&rect,
					fb, &dst,
					&parameters);
		}
	}
	cairo_region_subtract(priv->expose, priv->expose);
}

