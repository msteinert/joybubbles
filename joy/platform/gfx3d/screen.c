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
#if CAIRO_HAS_GFX3D_SURFACE
#include <cairo-gfx3d.h>
#endif // CAIRO_HAS_GFX3D_SURFACE
#include "joy/cursor.h"
#include "joy/error.h"
#include "joy/iterator/queue.h"
#include "joy/macros.h"
#include "joy/platform/gfx3d/application.h"
#include "joy/platform/gfx3d/mouse.h"
#include "joy/platform/gfx3d/keyboard.h"
#include "joy/platform/gfx3d/screen.h"
#include "joy/platform/gfx3d/window.h"
#include <gfx3d_cursor.h>

G_DEFINE_TYPE(JoyGfx3dScreen, joy_gfx3d_screen, JOY_TYPE_SCREEN)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_GFX3D_SCREEN, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyGfx3dScreen *)instance)->priv)

struct Private {
	GFX3D_Display display;
	cairo_region_t *area;
	cairo_region_t *expose;
	GQueue *windows;
	GArray *rects;
	JoyDevice *keyboard;
	JoyDevice *mouse;
	GFX3D_Display cursor;
	gint x, y, x_hot, y_hot;
	gboolean moved;
};

static void
joy_gfx3d_screen_init(JoyGfx3dScreen *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->windows = g_queue_new();
	priv->area = cairo_region_create();
	priv->expose = cairo_region_create();
	priv->rects = g_array_sized_new(FALSE, FALSE,
			sizeof(GFX3D_Rect), 128);
}

static void
draw_cursor(cairo_t *cr)
{
	cairo_set_source_rgba(cr, 0., 0., 0., 1.);
	cairo_set_line_width(cr, 2.);
	cairo_move_to(cr, 1., 1.);
	cairo_line_to(cr, 1., 17.);
	cairo_line_to(cr, 5., 15.);
	cairo_line_to(cr, 7., 19.);
	cairo_line_to(cr, 11., 17.);
	cairo_line_to(cr, 8., 13.);
	cairo_line_to(cr, 13., 11.);
	cairo_line_to(cr, 1., 1.);
	cairo_stroke_preserve(cr);
	cairo_set_source_rgba(cr, 1., 1., 1., 1.);
	cairo_fill(cr);
}

static void
cursor_init(JoyScreen *self)
{
	JoyCursor *cursor = NULL;
	cairo_t *cr = NULL;
	cairo_surface_t *image =
		joy_screen_cairo_surface_create(self, 20, 20);
	if (!image) {
		goto exit;
	}
	cr = cairo_create(image);
	cairo_save(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_restore(cr);
	draw_cursor(cr);
#if !CAIRO_HAS_GFX3D_SURFACE
	GFX3D_Display_Cache_Flush(GET_PRIVATE(self)->display);
#endif // !CAIRO_HAS_GFX3D_SURFACE
	cursor = joy_cursor_new(image, 0, 0);
	if (!cursor) {
		goto exit;
	}
	joy_gfx3d_screen_set_cursor(self, cursor);
	joy_gfx3d_screen_show_cursor(self);
exit:
	if (cursor) {
		g_object_unref(cursor);
	}
	if (cr) {
		cairo_destroy(cr);
	}
	if (image) {
		cairo_surface_destroy(image);
	}
}

static void
constructed(GObject *base)
{
	JoyScreen *self = JOY_SCREEN(base);
	struct Private *priv = GET_PRIVATE(base);
	gint width = joy_screen_get_width(self);
	gint height = joy_screen_get_height(self);
	// create the display
	priv->display = GFX3D_Display_Create(width, height,
			width, height, 8, joy_screen_get_id(self));
	if (G_UNLIKELY(!priv->display)) {
		goto exit;
	}
	// disable automatic cache flushing
	GFX3D_Display_Cache_AutoFlush_Set(priv->display, 0);
	// sync with the vertical blank
	GFX3D_Display_Show_Mode_Set(priv->display,
			GFX3D_DISPLAY_SHOW_MODE_VSYNC_WAIT_ONLY);
	// clear the display
	GFX3D_Rect rect = { 0, 0, width, height };
	GFX3D_Display_ClearAlpha2D(priv->display, NULL, &rect,
			0., 0., 0., 0.);
	// show the display
	GFX3D_Display_Visibility_Set(priv->display, 1);
	// set the display location
	GFX3D_Display_Location_Set(priv->display, 0, 0, 0);
	// setup the screen area
	cairo_region_union_rectangle(priv->area, (gpointer)&rect);
	priv->x = (gdouble)width / 2.;
	priv->y = (gdouble)height / 2.;
	// setup devices
	priv->keyboard = joy_gfx3d_keyboard_new(self);
	priv->mouse = joy_gfx3d_mouse_new(self);
	joy_device_keyboard_set_mouse(priv->keyboard, priv->mouse);
	joy_device_mouse_set_keyboard(priv->mouse, priv->keyboard);
	// create the cursor
	cursor_init(self);
exit:
	if (G_OBJECT_CLASS(joy_gfx3d_screen_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_gfx3d_screen_parent_class)->
			constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->windows) {
		for (GList *node = g_queue_peek_head_link(priv->windows);
				node; node = node->next) {
			JoyBubble *window = node->data;
			g_object_run_dispose(G_OBJECT(window));
			g_object_unref(window);
		}
		g_queue_free(priv->windows);
		priv->windows = NULL;
	}
	if (priv->keyboard) {
		g_object_run_dispose(G_OBJECT(priv->keyboard));
		g_object_unref(priv->keyboard);
		priv->keyboard = NULL;
	}
	if (priv->mouse) {
		g_object_run_dispose(G_OBJECT(priv->mouse));
		g_object_unref(priv->mouse);
		priv->mouse = NULL;
	}
	G_OBJECT_CLASS(joy_gfx3d_screen_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->area) {
		cairo_region_destroy(priv->area);
	}
	if (priv->expose) {
		cairo_region_destroy(priv->expose);
	}
	if (priv->rects) {
		g_array_free(priv->rects, TRUE);
	}
	JoyScreen *self = (JoyScreen *)base;
	GFX3D_Rect rect = {
		0, 0,
		joy_screen_get_width(self),
		joy_screen_get_height(self)
	};
	if (priv->display) {
		GFX3D_Display_ClearAlpha2D(priv->display, NULL, &rect,
				0., 0., 0., 0.);
		GFX3D_Display_Show_Partial(priv->display, &rect, 1, 1);
		GFX3D_Display_Destroy(priv->display);
	}
	if (priv->cursor) {
		GFX3D_Display_ClearAlpha2D(priv->cursor, NULL, &rect,
				0., 0., 0., 0.);
		GFX3D_Display_Show_Ch_Reload(priv->cursor);
		GFX3D_Cursor_Destroy(priv->cursor);
	}
	G_OBJECT_CLASS(joy_gfx3d_screen_parent_class)->finalize(base);
}

static JoyIterator *
begin(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->windows)) {
		return joy_iterator_queue_begin(priv->windows);
	}
	return NULL;
}

static JoyIterator *
end(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->windows)) {
		return joy_iterator_queue_end(priv->windows);
	}
	return NULL;
}

/**
 * \brief Expose handler for windows on this screen.
 *
 * The screen aggregates all exposed areas so that they can be cleared prior
 * to the next draw.
 *
 * \param window [in] A window object.
 * \param rect [in] The exposed area of \e window.
 * \param self [in] The screen \e window is on.
 */
static void
on_expose(JoyBubble *window, const cairo_rectangle_int_t *rect,
		JoyScreen *self)
{
	cairo_rectangle_int_t expose = {
		rect->x + joy_bubble_get_x(window),
		rect->y + joy_bubble_get_y(window),
		rect->width,
		rect->height
	};
	cairo_region_union_rectangle(GET_PRIVATE(self)->expose, &expose);
}

static JoyBubble *
window_create(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	JoyBubble *window = joy_gfx3d_window_new(self);
	if (G_UNLIKELY(!window)) {
		return NULL;
	}
	g_queue_push_head(priv->windows, window);
	g_signal_connect(window, "expose", G_CALLBACK(on_expose), self);
	return window;
}

static cairo_surface_type_t
cairo_surface_type(JoyScreen *self)
{
#if CAIRO_HAS_GFX3D_SURFACE
	return CAIRO_SURFACE_TYPE_GFX3D;
#else // CAIRO_HAS_GFX3D_SURFACE
	return CAIRO_SURFACE_TYPE_IMAGE;
#endif // CAIRO_HAS_GFX3D_SURFACE
}

static cairo_surface_t *
cairo_surface_create(JoyScreen *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	GFX3D_Image image = NULL;
	cairo_surface_t *surface = NULL;
	// create a GFX3D native surface
	image = GFX3D_Image_Create_Generic(priv->display, width, height);
	if (G_UNLIKELY(!image)) {
		goto error;
	}
	// create cairo surface
	surface = joy_gfx3d_screen_cairo_surface_create(self, image);
	if (G_UNLIKELY(!surface)) {
		goto error;
	}
	return surface;
error:
	if (surface) {
		cairo_surface_destroy(surface);
	}
	if (image) {
		GFX3D_Image_Destroy(priv->display, image);
	}
	return NULL;
}

JOY_GNUC_HOT
static void
submit(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	// update cursor
	if (priv->cursor && priv->moved) {
		GFX3D_Cursor_Position_Set(priv->cursor,
				priv->x - priv->x_hot,
				priv->y - priv->y_hot);
	}
	// clear exposed areas of the frame buffer
	cairo_region_intersect(priv->expose, priv->area);
	for (gint i = 0; i < cairo_region_num_rectangles(priv->expose); ++i) {
		cairo_rectangle_int_t rect;
		cairo_region_get_rectangle(priv->expose, i, &rect);
		GFX3D_Display_ClearAlpha2D(priv->display, NULL,
				(gpointer)&rect, 0., 0., 0., 0.);
		g_array_append_val(priv->rects, rect);
	}
	if (priv->rects->len) {
		cairo_region_subtract(priv->expose, priv->expose);
#if !CAIRO_HAS_GFX3D_SURFACE
		GFX3D_Display_Cache_Flush(priv->display);
#endif // !CAIRO_HAS_GFX3D_SURFACE
		GFX3D_NATIVE_Display display = 
			GFX3D_Display_Get_NATIVE_Display(priv->display);
		GFX3D_NATIVE_Surface surface =
			GFX3D_Display_FrameBuffer_Get_NATIVE_Surface(
					priv->display);
		// copy windows to the frame buffer
		for (GList *node = g_queue_peek_head_link(priv->windows);
				node; node = node->next) {
			JoyBubble *window = node->data;
			joy_gfx3d_window_submit(window, display, surface,
					priv->area);
		}
		// flip the display
		GFX3D_Display_Show_Partial(priv->display,
				(gpointer)priv->rects->data,
				priv->rects->len, 1);
		g_array_set_size(priv->rects, 0);
	} else {
		if (priv->moved) {
			GFX3D_Display_Show_Ch_Reload(priv->display);
		}
	}
	priv->moved = FALSE;
}

static gboolean
enable_mirroring(JoyScreen *self, JoyScreen *mirror, GError **error)
{
	struct Private *priv = GET_PRIVATE(self);
	gint id = joy_screen_get_id(self);
	gint mirror_id = joy_screen_get_id(mirror);
	if (id != 0 || mirror_id != 1) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: mirroring from screen %d to %d is "
				"not supported", id, mirror_id);
		return FALSE;
	}
	int status, enable;
	status = GFX3D_Display_MirrorSDConfig_Get(priv->display, &enable);
	if (!status && enable) {
		return TRUE;
	}
	status = GFX3D_Display_MirrorSDConfig_Set(priv->display, 1);
	if (status) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: failed to enable mirroring from "
				"screen %d to %d", id, mirror_id);
		return FALSE;
	}
	return TRUE;
}

static gboolean
disable_mirroring(JoyScreen *self, JoyScreen *mirror, GError **error)
{
	struct Private *priv = GET_PRIVATE(self);
	gint id = joy_screen_get_id(self);
	gint mirror_id = joy_screen_get_id(mirror);
	if (id != 0 || mirror_id != 1) {
		return TRUE;
	}
	int status, enable;
	status = GFX3D_Display_MirrorSDConfig_Get(priv->display, &enable);
	if (!status && !enable) {
		return TRUE;
	}
	status = GFX3D_Display_MirrorSDConfig_Set(priv->display, 0);
	if (status) {
		g_set_error(error, JOY_ERROR, JOY_ERROR_FAILURE,
				"gfx3d: failed to disable mirroring from "
				"screen %d to %d", id, mirror_id);
		return FALSE;
	}
	return TRUE;
}

static void
joy_gfx3d_screen_class_init(JoyGfx3dScreenClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	JoyScreenClass *screen_class = JOY_SCREEN_CLASS(klass);
	screen_class->begin = begin;
	screen_class->end = end;
	screen_class->window_create = window_create;
	screen_class->cairo_surface_type = cairo_surface_type;
	screen_class->cairo_surface_create = cairo_surface_create;
	screen_class->submit = submit;
	screen_class->enable_mirroring = enable_mirroring;
	screen_class->disable_mirroring = disable_mirroring;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoyBubble *
joy_gfx3d_screen_new(JoyApplication *app, guint id, gint width, gint height)
{
	g_return_val_if_fail(JOY_IS_GFX3D_APPLICATION(app), NULL);
	return g_object_new(JOY_TYPE_GFX3D_SCREEN,
			"application", app,
			"screen-id", id,
			"width", width,
			"height", height,
			NULL);
}

GFX3D_Display
joy_gfx3d_screen_get_display(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_GFX3D_SCREEN(self), 0UL);
	return GET_PRIVATE(self)->display;
}

#if !CAIRO_HAS_GFX3D_SURFACE
static const cairo_user_data_key_t key;

struct Data {
	GFX3D_Display display;
	GFX3D_Image image;
};

static void
data_destroy(struct Data *data)
{
	if (data) {
		GFX3D_Image_Destroy(data->display, data->image);
		g_slice_free(struct Data, data);
	}
}
#endif // !CAIRO_HAS_GFX3D_SURFACE

cairo_surface_t *
joy_gfx3d_screen_cairo_surface_create(JoyScreen *self, GFX3D_Image image)
{
	g_return_val_if_fail(JOY_IS_GFX3D_SCREEN(self), NULL);
	g_return_val_if_fail(image, NULL);
	struct Private *priv = GET_PRIVATE(self);
#if CAIRO_HAS_GFX3D_SURFACE
	cairo_surface_t *surface = cairo_gfx3d_surface_create(priv->display,
			image, CAIRO_FORMAT_ARGB32);
	cairo_status_t status = cairo_surface_status(surface);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		return NULL;
	}
	cairo_gfx3d_surface_set_owner(surface, TRUE);
	return surface;
#else // CAIRO_HAS_GFX3D_SURFACE
	GFX3D_Rect rect;
	GFX3D_Image_GetRect(image, &rect);
	gint stride;
	void *pixels = GFX3D_NATIVE_Surface_GetAddress(
			GFX3D_Image_Get_NATIVE_Surface(image), &stride);
	if (G_UNLIKELY(!pixels)) {
		return NULL;
	}
	stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32,
			rect.iWidth);
	cairo_surface_t *surface = cairo_image_surface_create_for_data(pixels,
			CAIRO_FORMAT_ARGB32, rect.iWidth, rect.iHeight,
			stride);
	cairo_status_t status = cairo_surface_status(surface);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		cairo_surface_destroy(surface);
		return NULL;
	}
	struct Data *data = g_slice_new0(struct Data);
	if (G_UNLIKELY(!data)) {
		cairo_surface_destroy(surface);
		return NULL;
	}
	data->display = priv->display;
	data->image = image;
	status = cairo_surface_set_user_data(surface, &key, data,
			(cairo_destroy_func_t)data_destroy);
	if (G_UNLIKELY(CAIRO_STATUS_SUCCESS != status)) {
		cairo_surface_destroy(surface);
		g_slice_free(struct Data, data);
		return NULL;
	}
	return surface;
#endif // CAIRO_HAS_GFX3D_SURFACE
}

JoyDevice *
joy_gfx3d_screen_get_mouse(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_GFX3D_SCREEN(self), NULL);
	return GET_PRIVATE(self)->mouse;
}

JoyDevice *
joy_gfx3d_screen_get_keyboard(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_GFX3D_SCREEN(self), NULL);
	return GET_PRIVATE(self)->keyboard;
}

void
joy_gfx3d_screen_set_cursor(JoyScreen *self, JoyCursor *cursor)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	g_return_if_fail(JOY_IS_CURSOR(cursor));
	struct Private *priv = GET_PRIVATE(self);
	cairo_surface_t *surface = joy_cursor_get_image(cursor);
	if (!surface) {
		return;
	}
#if CAIRO_HAS_GFX3D_SURFACE
	cairo_surface_type_t type = cairo_surface_get_type(surface);
	if (CAIRO_SURFACE_TYPE_GFX3D != type) {
		if (CAIRO_SURFACE_TYPE_IMAGE != type) {
			return;
		}
		gint width = cairo_image_surface_get_width(surface);
		gint height = cairo_image_surface_get_height(surface);
		cairo_surface_t *gfx3d_surface =
			joy_screen_cairo_surface_create(self, width, height);
		if (G_UNLIKELY(!gfx3d_surface)) {
			return;
		}
		cairo_t *cr = cairo_create(gfx3d_surface);
		cairo_set_source_surface(cr, surface, 0., 0.);
		cairo_paint(cr);
		cairo_destroy(cr);
		joy_cursor_set_image(cursor, gfx3d_surface);
		surface = gfx3d_surface;
	}
	GFX3D_Image image = cairo_gfx3d_surface_get_image(surface);
#else // CAIRO_HAS_GFX3D_SURFACE
	struct Data *data = cairo_surface_get_user_data(surface, &key);
	if (!data) {
		cairo_surface_type_t type = cairo_surface_get_type(surface);
		if (CAIRO_SURFACE_TYPE_IMAGE != type) {
			return;
		}
		gint width = cairo_image_surface_get_width(surface);
		gint height = cairo_image_surface_get_height(surface);
		cairo_surface_t *gfx3d_surface =
			joy_screen_cairo_surface_create(self, width, height);
		if (G_UNLIKELY(!gfx3d_surface)) {
			return;
		}
		cairo_t *cr = cairo_create(gfx3d_surface);
		cairo_set_source_surface(cr, surface, 0., 0.);
		cairo_paint(cr);
		cairo_destroy(cr);
		joy_cursor_set_image(cursor, gfx3d_surface);
		data = cairo_surface_get_user_data(gfx3d_surface, &key);
		if (G_UNLIKELY(!data)) {
			return;
		}
	}
	GFX3D_Image image = data->image;
#endif // CAIRO_HAS_GFX3D_SURFACE
	GFX3D_Rect rect;
	GFX3D_Image_GetRect(image, &rect);
	if (!priv->cursor) {
		gint width = joy_screen_get_width(self);
		gint height = joy_screen_get_height(self);
		gint n = joy_screen_get_id(self);
		priv->cursor = GFX3D_Cursor_Create_Ext(width, height,
				image, &rect, n);
		if (!priv->cursor) {
			return;
		}
		GFX3D_Display_Show_Mode_Set(priv->cursor,
				GFX3D_DISPLAY_SHOW_MODE_ASYNCHRONOUS);
	} else {
		GFX3D_Cursor_Image_Swap(priv->cursor, image, &rect);
	}
	priv->x_hot = joy_cursor_get_hotspot_x(cursor);
	priv->y_hot = joy_cursor_get_hotspot_y(cursor);
	priv->moved = TRUE;
}

void
joy_gfx3d_screen_warp_cursor(JoyScreen *self, gint x, gint y)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->x != x) {
		priv->x = CLAMP(x, 0, joy_screen_get_width(self));
		priv->moved = TRUE;
	}
	if (priv->y != y) {
		priv->y = CLAMP(y, 0, joy_screen_get_height(self));
		priv->moved = TRUE;
	}
}

void
joy_gfx3d_screen_move_cursor(JoyScreen *self, gint x, gint y)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	priv->x = CLAMP(priv->x + x, 0, joy_screen_get_width(self));
	priv->y = CLAMP(priv->y + y, 0, joy_screen_get_height(self));
	priv->moved = TRUE;
}

void
joy_gfx3d_screen_get_cursor_position(JoyScreen *self, gint *x, gint *y)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	if (x) {
		*x = priv->x;
	}
	if (y) {
		*y = priv->y;
	}
}

void
joy_gfx3d_screen_show_cursor(JoyScreen *self)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->cursor) {
		return;
	}
	GFX3D_Cursor_State_Set(priv->cursor, 1);
}

void
joy_gfx3d_screen_hide_cursor(JoyScreen *self)
{
	g_return_if_fail(JOY_IS_GFX3D_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->cursor) {
		return;
	}
	GFX3D_Cursor_State_Set(priv->cursor, 0);
}

gboolean
joy_gfx3d_screen_cursor_visible(JoyScreen *self)
{
	struct Private *priv = GET_PRIVATE(self);
	gint enable;
	if (GFX3D_Cursor_State_Get(priv->cursor, &enable)) {
		return FALSE;
	}
	return enable ? TRUE : FALSE;
}

void
joy_gfx3d_screen_raise_window(JoyScreen *self, JoyBubble *window)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->windows)) {
		return;
	}
	gint n = g_queue_index(priv->windows, window);
	if (G_UNLIKELY(-1 == n)) {
		return;
	}
	GList *node = g_queue_pop_nth_link(priv->windows, n);
	if (G_UNLIKELY(!node)) {
		return;
	}
	g_queue_push_head_link(priv->windows, node);
}

void
joy_gfx3d_screen_lower_window(JoyScreen *self, JoyBubble *window)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->windows)) {
		return;
	}
	gint n = g_queue_index(priv->windows, window);
	if (G_UNLIKELY(-1 == n)) {
		return;
	}
	GList *node = g_queue_pop_nth_link(priv->windows, n);
	if (G_UNLIKELY(!node)) {
		return;
	}
	g_queue_push_tail_link(priv->windows, node);
}
