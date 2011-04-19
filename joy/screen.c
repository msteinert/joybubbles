/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/animation.h"
#include "joy/application.h"
#include "joy/iterator.h"
#include "joy/macros.h"
#include "joy/screen.h"
#include "joy/theme.h"
#include "joy/window.h"

G_DEFINE_ABSTRACT_TYPE(JoyScreen, joy_screen, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_SCREEN, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyScreen *)instance)->priv)

struct Private {
	JoyApplication *app;
	gint width;
	gint height;
	GHashTable *focus;
	gpointer cache[2];
	gint id;
	GPtrArray *animations;
};

static void
joy_screen_init(JoyScreen *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->focus = g_hash_table_new(g_direct_hash, g_direct_equal);
	priv->animations = g_ptr_array_new_with_free_func(g_object_unref);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->app) {
		g_object_unref(priv->app);
		priv->app = NULL;
	}
	if (priv->animations) {
		g_ptr_array_unref(priv->animations);
		priv->animations = NULL;
	}
	G_OBJECT_CLASS(joy_screen_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->focus) {
		g_hash_table_destroy(priv->focus);
	}
	G_OBJECT_CLASS(joy_screen_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_APPLICATION,
	PROP_SCREEN_ID,
	PROP_WIDTH,
	PROP_HEIGHT,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_APPLICATION:
		priv->app = g_value_dup_object(value);
		break;
	case PROP_SCREEN_ID:
		priv->id = g_value_get_int(value);
		break;
	case PROP_WIDTH:
		priv->width = g_value_get_int(value);
		break;
	case PROP_HEIGHT:
		priv->height = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_screen_class_init(JoyScreenClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
	g_object_class_install_property(object_class, PROP_APPLICATION,
		g_param_spec_object("application", Q_("Application"),
			Q_("The application this screen belongs to"),
			JOY_TYPE_APPLICATION,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_SCREEN_ID,
		g_param_spec_int("screen-id", Q_("Screen ID"),
			Q_("The screen number"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_WIDTH,
		g_param_spec_int("width", Q_("Width"),
			Q_("The screen width"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_HEIGHT,
		g_param_spec_int("height", Q_("Height"),
			Q_("The screen height"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyApplication *
joy_screen_get_application(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	return GET_PRIVATE(self)->app;
}

void
joy_screen_set_theme(JoyScreen *self, JoyTheme *theme)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	g_return_if_fail(JOY_IS_THEME(theme));
	for (JoyIterator *iter = joy_screen_begin(self); iter;
			iter = joy_iterator_next(iter)) {
		JoyBubble *window = joy_iterator_item(iter);
		joy_bubble_set_style(window, theme);
	}
}

gint
joy_screen_get_id(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), 0);
	return GET_PRIVATE(self)->id;
}

gint
joy_screen_get_width(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), 0);
	return GET_PRIVATE(self)->width;
}

gint
joy_screen_get_height(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), 0);
	return GET_PRIVATE(self)->height;
}

JoyBubble *
joy_screen_window_at(JoyScreen *self, gint x, gint y)
{
	cairo_region_t *region = cairo_region_create();
	if (G_UNLIKELY(!region)) {
		return NULL;
	}
	JoyBubble *window = NULL;
	for (JoyIterator *iter = joy_screen_end(self); iter;
			iter = joy_iterator_previous(iter)) {
		cairo_region_subtract(region, region);
		window = joy_iterator_item(iter);
		cairo_rectangle_int_t rect = {
			joy_bubble_get_x(window),
			joy_bubble_get_y(window),
			joy_bubble_get_width(window),
			joy_bubble_get_height(window)
		};
		cairo_region_union_rectangle(region, &rect);
		if (cairo_region_contains_point(region, x, y)) {
			g_object_unref(iter);
			break;
		}
	}
	cairo_region_destroy(region);
	return window;
}

JoyIterator *
joy_screen_begin(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	return JOY_SCREEN_GET_CLASS(self)->begin(self);
}

JoyIterator *
joy_screen_end(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	return JOY_SCREEN_GET_CLASS(self)->end(self);
}

JoyBubble *
joy_screen_window_create(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	return JOY_SCREEN_GET_CLASS(self)->window_create(self);
}

cairo_surface_type_t
joy_screen_cairo_surface_type(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), -1);
	return JOY_SCREEN_GET_CLASS(self)->cairo_surface_type(self);
}

cairo_surface_t *
joy_screen_cairo_surface_create(JoyScreen *self,
		gint width, gint height)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	return JOY_SCREEN_GET_CLASS(self)->
		cairo_surface_create(self, width, height);
}

void
joy_screen_set_at_device(JoyScreen *self, JoyDevice *device, JoyBubble *at)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	priv->cache[0] = device;
	priv->cache[1] = at;
	g_hash_table_insert(priv->focus, device, at);
}

JoyBubble *
joy_screen_get_at_device(JoyScreen *self, JoyDevice *device)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(priv->cache[0] != device)) {
		priv->cache[0] = device;
		priv->cache[1] = g_hash_table_lookup(priv->focus, device);
	}
	return priv->cache[1];
}

void
joy_screen_add_animation(JoyScreen *self, JoyAnimation *animation)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	g_return_if_fail(JOY_IS_ANIMATION(animation));
	g_ptr_array_add(GET_PRIVATE(self)->animations,
			g_object_ref(animation));
}

void
joy_screen_remove_animation(JoyScreen *self, JoyAnimation *animation)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	g_return_if_fail(JOY_IS_ANIMATION(animation));
	struct Private *priv = GET_PRIVATE(self);
	for (gint i = 0; i < priv->animations->len; ++i) {
		if (priv->animations->pdata[i] == animation) {
			g_ptr_array_remove_index(priv->animations, i);
			return;
		}
	}
}

gboolean
joy_screen_in_animation(JoyScreen *self)
{
	g_return_val_if_fail(JOY_IS_SCREEN(self), FALSE);
	return GET_PRIVATE(self)->animations->len ? TRUE : FALSE;
}

JOY_GNUC_HOT
void
joy_screen_animate(JoyScreen *self)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	struct Private *priv = GET_PRIVATE(self);
	for (gint i = 0; i < priv->animations->len; ++i) {
		JoyAnimation *animation = priv->animations->pdata[i];
		joy_animation_frame(animation);
	}
}

JOY_GNUC_HOT
void
joy_screen_draw(JoyScreen *self)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	for (JoyIterator *iter = joy_screen_begin(self); iter;
			iter = joy_iterator_next(iter)) {
		JoyBubble *window = joy_iterator_item(iter);
		cairo_surface_t *surface =
			joy_window_cairo_surface_create(window);
		if (G_LIKELY(surface)) {
			cairo_t *cr = cairo_create(surface);
			joy_bubble_draw(window, cr);
			cairo_destroy(cr);
		}
	}
}

void
joy_screen_submit(JoyScreen *self)
{
	g_return_if_fail(JOY_IS_SCREEN(self));
	JOY_SCREEN_GET_CLASS(self)->submit(self);
}

gboolean
joy_screen_enable_mirroring(JoyScreen *self, JoyScreen *mirror,
		GError **error)
{
	joy_return_error_if_fail(JOY_IS_SCREEN(self), FALSE, error);
	joy_return_error_if_fail(JOY_IS_SCREEN(mirror), FALSE, error);
	return JOY_SCREEN_GET_CLASS(self)->
		enable_mirroring(self, mirror, error);
}

gboolean
joy_screen_disable_mirroring(JoyScreen *self, JoyScreen *mirror,
		GError **error)
{
	joy_return_error_if_fail(JOY_IS_SCREEN(self), FALSE, error);
	joy_return_error_if_fail(JOY_IS_SCREEN(mirror), FALSE, error);
	return JOY_SCREEN_GET_CLASS(self)->
		disable_mirroring(self, mirror, error);
}
