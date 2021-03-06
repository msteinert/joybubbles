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
#include "joy/accumulator.h"
#include "joy/application.h"
#include "joy/bubble.h"
#include "joy/buffer.h"
#include "joy/container.h"
#include "joy/marshal.h"
#include "joy/screen.h"
#include "joy/style.h"
#include "joy/theme.h"
#include <pango/pangocairo.h>

G_DEFINE_ABSTRACT_TYPE(JoyBubble, joy_bubble, G_TYPE_INITIALLY_UNOWNED)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_BUBBLE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyBubble *)instance)->priv)

enum Signals {
	SIGNAL_MOVE,
	SIGNAL_RESIZE,
	SIGNAL_EXPOSE,
	SIGNAL_KEY_DOWN,
	SIGNAL_KEY_UP,
	SIGNAL_SHOW,
	SIGNAL_HIDE,
	SIGNAL_MOTION,
	SIGNAL_BUTTON_DOWN,
	SIGNAL_BUTTON_UP,
	SIGNAL_SCROLL,
	SIGNAL_ENTER,
	SIGNAL_LEAVE,
	SIGNAL_DRAW,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	gchar *name;
	JoyBubble *parent;
	cairo_region_t *area;
	cairo_region_t *draw;
	gboolean visible;
	gint x, y;
	gint width;
	gint height;
	gboolean horizontal_expand;
	gboolean vertical_expand;
	JoyBuffer *buffer;
	gdouble alpha;
	JoyStyle *style;
	JoyBubbleState state;
};

static void
joy_bubble_init(JoyBubble *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->area = cairo_region_create();
	priv->draw = cairo_region_create();
	priv->alpha = 1.;
	priv->state = JOY_BUBBLE_STATE_DEFAULT;
}

static void
constructed(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyBubble *self = JOY_BUBBLE(base);
	gint width = joy_bubble_get_width(self);
	gint height = joy_bubble_get_height(self);
	if (width || height) {
		cairo_rectangle_int_t rect = {
			0, 0, width, height
		};
		cairo_region_union_rectangle(priv->area, &rect);
	}
	if (G_OBJECT_CLASS(joy_bubble_parent_class)->constructed) {
		G_OBJECT_CLASS(joy_bubble_parent_class)->constructed(base);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyBubble *self = JOY_BUBBLE(base);
	if (priv->parent) {
		joy_container_remove(priv->parent, self);
		g_object_unref(priv->parent);
		priv->parent = NULL;
	}
	if (priv->buffer) {
		joy_buffer_destroy(priv->buffer);
		priv->buffer = NULL;
	}
	if (priv->style) {
		g_object_unref(priv->style);
		priv->style = NULL;
	}
	G_OBJECT_CLASS(joy_bubble_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->name);
	if (priv->area) {
		cairo_region_destroy(priv->area);
	}
	if (priv->draw) {
		cairo_region_destroy(priv->draw);
	}
	G_OBJECT_CLASS(joy_bubble_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_X_COORDINATE,
	PROP_Y_COORDINATE,
	PROP_WIDTH,
	PROP_HEIGHT,
	PROP_EXPAND,
	PROP_HORIZONTAL_EXPAND,
	PROP_VERTICAL_EXPAND,
	PROP_BUFFERED,
	PROP_THEME,
	PROP_STYLE,
	PROP_PARENT,
	PROP_STATE,
	PROP_MAX
};

static GParamSpec *properties[PROP_MAX];

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_X_COORDINATE:
		priv->x = g_value_get_int(value);
		break;
	case PROP_Y_COORDINATE:
		priv->y = g_value_get_int(value);
		break;
	case PROP_WIDTH:
		priv->width = g_value_get_int(value);
		break;
	case PROP_HEIGHT:
		priv->height = g_value_get_int(value);
		break;
	case PROP_EXPAND:
		joy_bubble_set_expand(self, g_value_get_boolean(value));
		break;
	case PROP_HORIZONTAL_EXPAND:
		joy_bubble_set_horizontal_expand(self,
				g_value_get_boolean(value));
		break;
	case PROP_VERTICAL_EXPAND:
		joy_bubble_set_vertical_expand(self,
				g_value_get_boolean(value));
		break;
	case PROP_BUFFERED:
		joy_bubble_set_buffered(self, g_value_get_boolean(value));
		break;
	case PROP_THEME:
		joy_bubble_set_theme(self, g_value_get_object(value));
		break;
	case PROP_STYLE:
		joy_bubble_set_style(self, g_value_get_object(value));
		break;
	case PROP_PARENT:
		joy_bubble_set_parent(self, g_value_get_object(value));
		break;
	case PROP_STATE:
		joy_bubble_set_state(self, g_value_get_int(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
get_property(GObject *base, guint id, GValue *value, GParamSpec *pspec)
{
	JoyBubble *self = JOY_BUBBLE(base);
	switch (id) {
	case PROP_THEME:
		g_value_set_object(value, joy_bubble_get_theme(self));
		break;
	case PROP_STYLE:
		g_value_set_object(value, joy_bubble_get_style(self));
		break;
	case PROP_PARENT:
		g_value_set_object(value, joy_bubble_get_parent(self));
		break;
	case PROP_STATE:
		g_value_set_int(value, joy_bubble_get_state(self));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
damage(JoyBubble *self, gint x, gint y, gint width, gint height,
		gboolean buffer)
{
	struct Private *priv = GET_PRIVATE(self);
	cairo_region_t *region = NULL;
	if (!priv->visible) {
		goto exit;
	}
	cairo_rectangle_int_t rect = {
		x, y, width, height
	};
	region = cairo_region_create_rectangle(&rect);
	if (G_UNLIKELY(!region)) {
		goto exit;
	}
	cairo_region_intersect(region, priv->area);
	if (cairo_region_is_empty(region)) {
		goto exit;
	}
	if (buffer && priv->buffer) {
		joy_buffer_damage(priv->buffer, region);
	}
	JoyBubble *window = self;
	while (priv->parent) {
		cairo_region_translate(region, priv->x, priv->y);
		window = priv->parent;
		priv = GET_PRIVATE(priv->parent);
	}
	for (gint i = 0; i < cairo_region_num_rectangles(region); ++i) {
		cairo_rectangle_int_t rect;
		cairo_region_get_rectangle(region, i, &rect);
		joy_bubble_expose(window, &rect);
	}
exit:
	if (region) {
		cairo_region_destroy(region);
	}
}

static void
set_theme(JoyBubble *self, JoyStyle *theme)
{
	joy_bubble_set_style(self, joy_theme_get_style(theme, self));
}

static JoyApplication *
get_application(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->parent)) {
		return joy_bubble_get_application(priv->parent);
	}
	return NULL;
}

static JoyScreen *
get_screen(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->parent)) {
		return joy_bubble_get_screen(priv->parent);
	}
	return NULL;
}

static JoyBubble *
get_window(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_LIKELY(priv->parent)) {
		return joy_bubble_get_window(priv->parent);
	}
	return NULL;
}

static void
set_buffered(JoyBubble *self, gboolean buffered)
{
	struct Private *priv = GET_PRIVATE(self);
	if (buffered && !priv->buffer) {
		priv->buffer = joy_buffer_new();
	} else if (!buffered && priv->buffer
			&& (1. == joy_buffer_get_alpha(priv->buffer))) {
		joy_buffer_destroy(priv->buffer);
		priv->buffer = NULL;
	}
}

static gboolean
get_buffered(JoyBubble *self)
{
	return GET_PRIVATE(self)->buffer ? TRUE : FALSE;
}

static JoyBubble *
at(JoyBubble *self, gint x, gint y)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->visible) {
		if (cairo_region_contains_point(priv->area, x, y)) {
			return self;
		}
	}
	return NULL;
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->horizontal_expand) {
		priv->width = width;
	}
	if (!priv->vertical_expand) {
		priv->height = height;
	}
	damage(self, 0, 0, joy_bubble_get_width(self),
			joy_bubble_get_height(self), FALSE);
}

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->style) {
		joy_style_draw_background(priv->style, self, cr);
		joy_style_draw_foreground(priv->style, self, cr);
	} else {
		gdouble width = joy_bubble_get_width(self);
		gdouble height = joy_bubble_get_height(self);
		cairo_set_line_width(cr, 2.);
		cairo_rectangle(cr, 1., 1., width - 2., height - 2.);
		cairo_set_source_rgb(cr, 1., 1., 0.);
		cairo_fill_preserve(cr);
		cairo_move_to(cr, 0., 0.);
		cairo_line_to(cr, width, height);
		cairo_move_to(cr, 0., height);
		cairo_line_to(cr, width, 0.);
		switch (joy_bubble_get_state(self)) {
		case JOY_BUBBLE_STATE_DEFAULT:
			cairo_set_source_rgb(cr, 1., 0., 0.);
			break;
		case JOY_BUBBLE_STATE_FOCUSED:
			cairo_set_source_rgb(cr, 0., 1., 0.);
			break;
		case JOY_BUBBLE_STATE_ACTIVE:
			cairo_set_source_rgb(cr, 0., 0., 1.);
			break;
		case JOY_BUBBLE_STATE_DISABLED:
		default:
			cairo_set_source_rgb(cr, 0., 0., 0.);
		}
		cairo_stroke(cr);
	}
	return TRUE;
}

static void
joy_bubble_class_init(JoyBubbleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructed = constructed;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	object_class->get_property = get_property;
	klass->set_theme = set_theme;
	klass->get_application = get_application;
	klass->get_screen = get_screen;
	klass->get_window = get_window;
	klass->set_buffered = set_buffered;
	klass->get_buffered = get_buffered;
	klass->at = at;
	klass->resize = resize;
	klass->draw = draw;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyBubble::move
	signals[SIGNAL_MOVE] =
		g_signal_new(g_intern_static_string("move"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyBubbleClass, move),
			NULL, NULL, joy_marshal_VOID__INT_INT, G_TYPE_NONE,
			2, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::resize
	signals[SIGNAL_RESIZE] =
		g_signal_new(g_intern_static_string("resize"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyBubbleClass, resize),
			NULL, NULL, joy_marshal_VOID__INT_INT, G_TYPE_NONE,
			2, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::expose
	signals[SIGNAL_EXPOSE] =
		g_signal_new(g_intern_static_string("expose"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyBubbleClass, expose),
			NULL, NULL, joy_marshal_VOID__POINTER, G_TYPE_NONE,
			1, G_TYPE_POINTER);
	// JoyBubble::key-down
	signals[SIGNAL_KEY_DOWN] =
		g_signal_new(g_intern_static_string("key-down"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, key_down),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT,
			G_TYPE_NONE, 5, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::key-up
	signals[SIGNAL_KEY_UP] =
		g_signal_new(g_intern_static_string("key-up"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, key_up),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT,
			G_TYPE_NONE, 5, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::show
	signals[SIGNAL_SHOW] =
		g_signal_new(g_intern_static_string("show"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyBubbleClass, show),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// JoyBubble::hide
	signals[SIGNAL_HIDE] =
		g_signal_new(g_intern_static_string("hide"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyBubbleClass, show),
			NULL, NULL, joy_marshal_VOID__VOID, G_TYPE_NONE, 0);
	// JoyBubble::motion
	signals[SIGNAL_MOTION] =
		g_signal_new(g_intern_static_string("motion"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, motion),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT,
			G_TYPE_NONE, 4, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::button-down
	signals[SIGNAL_BUTTON_DOWN] =
		g_signal_new(g_intern_static_string("button-down"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, button_down),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT,
			G_TYPE_NONE, 5, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::button-up
	signals[SIGNAL_BUTTON_UP] =
		g_signal_new(g_intern_static_string("button-up"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, button_up),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT,
			G_TYPE_NONE, 5, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::scroll
	signals[SIGNAL_SCROLL] =
		g_signal_new(g_intern_static_string("scroll"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, scroll),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT_INT,
			G_TYPE_NONE, 5, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::enter
	signals[SIGNAL_ENTER] =
		g_signal_new(g_intern_static_string("enter"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, enter),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT,
			G_TYPE_NONE, 4, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::leave
	signals[SIGNAL_LEAVE] =
		g_signal_new(g_intern_static_string("leave"),
			G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
			G_STRUCT_OFFSET(JoyBubbleClass, leave),
			NULL, NULL, joy_marshal_VOID__OBJECT_ULONG_INT_INT,
			G_TYPE_NONE, 4, G_TYPE_OBJECT, G_TYPE_ULONG,
			G_TYPE_INT, G_TYPE_INT);
	// JoyBubble::draw
	signals[SIGNAL_DRAW] =
		g_signal_new(g_intern_static_string("draw"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_LAST,
			G_STRUCT_OFFSET(JoyBubbleClass, draw),
			joy_boolean_accumulator, NULL,
			joy_marshal_BOOLEAN__POINTER, G_TYPE_BOOLEAN,
			1, G_TYPE_POINTER);
	// properties
	g_object_class_install_property(object_class, PROP_X_COORDINATE,
		g_param_spec_int("x-coordinate", Q_("X-coordinate"),
			Q_("X-coordinate relative to the parent"),
			G_MININT, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_Y_COORDINATE,
		g_param_spec_int("y-coordinate", Q_("Y-coordinate"),
			Q_("Y-coordinate relative the parent"),
			G_MININT, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_WIDTH,
		g_param_spec_int("width", Q_("Width"),
			Q_("The width of this widget"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_HEIGHT,
		g_param_spec_int("height", Q_("Height"),
			Q_("The height of this widget"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_EXPAND,
		g_param_spec_boolean("expand", Q_("Expand"),
			Q_("Indicates if this widget expands"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_HORIZONTAL_EXPAND,
		g_param_spec_boolean("horizontal-expand",
			Q_("Horizontal Expand"),
			Q_("Indicates if this widget expands horizontally"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_VERTICAL_EXPAND,
		g_param_spec_boolean("vertical-expand", Q_("Vertical Expand"),
			Q_("Indicates if this widget expands vertically"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_BUFFERED,
		g_param_spec_boolean("buffered", Q_("Buffered"),
			Q_("Indicates if this widget is double buffered"),
			FALSE, G_PARAM_WRITABLE));
	properties[PROP_THEME] =
		g_param_spec_object("theme", Q_("Theme"),
			Q_("The theme for this widget"), JOY_TYPE_THEME,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_THEME,
			properties[PROP_THEME]);
	properties[PROP_STYLE] =
		g_param_spec_object("style", Q_("Style"),
			Q_("The style for this widget"), JOY_TYPE_STYLE,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_STYLE,
			properties[PROP_STYLE]);
	properties[PROP_PARENT] =
		g_param_spec_object("parent", Q_("Parent"),
			Q_("The parent of this widget"), JOY_TYPE_BUBBLE,
			G_PARAM_READWRITE);
	g_object_class_install_property(object_class, PROP_PARENT,
			properties[PROP_PARENT]);
}

void
joy_bubble_set_name(JoyBubble *self, const gchar *name)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	g_free(priv->name);
	priv->name = g_strdup(name);
	JoyStyle *theme = joy_bubble_get_theme(self);
	if (G_LIKELY(theme)) {
		joy_bubble_set_theme(self, theme);
	}
}

const gchar *
joy_bubble_get_name(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return GET_PRIVATE(self)->name;
}

void
joy_bubble_set_theme(JoyBubble *self, JoyStyle *theme)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(!theme || JOY_IS_THEME(theme));
	JOY_BUBBLE_GET_CLASS(self)->set_theme(self, theme);
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_THEME]);
}

JoyStyle *
joy_bubble_get_theme(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	JoyApplication *app = joy_bubble_get_application(self);
	if (G_UNLIKELY(!app)) {
		return NULL;
	}
	return joy_application_get_theme(app);
}

void
joy_bubble_set_style(JoyBubble *self, JoyStyle *style)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(!style || JOY_IS_STYLE(style));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->style) {
		g_object_unref(priv->style);
	}
	priv->style = style ? g_object_ref(style) : NULL;
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_STYLE]);
}

JoyStyle *
joy_bubble_get_style(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return GET_PRIVATE(self)->style;
}

void
joy_bubble_set_buffered(JoyBubble *self, gboolean buffered)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	JOY_BUBBLE_GET_CLASS(self)->set_buffered(self, buffered);
}

gboolean
joy_bubble_get_buffered(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), FALSE);
	return JOY_BUBBLE_GET_CLASS(self)->get_buffered(self);
}

void
joy_bubble_set_alpha(JoyBubble *self, gdouble alpha)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	alpha = CLAMP(alpha, 0., 1.);
	if (0. == alpha) {
		joy_bubble_hide(self);
		return;
	} else {
		if (!priv->visible) {
			joy_bubble_show(self);
		} else {
			damage(self, 0, 0, joy_bubble_get_width(self),
					joy_bubble_get_height(self), FALSE);
		}
		if (priv->buffer) {
			joy_buffer_set_alpha(priv->buffer, alpha);
		}
		priv->alpha = alpha;
	}
}

gdouble
joy_bubble_get_alpha(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), 0.);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->visible) {
		return priv->alpha;
	}
	return 0.;
}

void
joy_bubble_set_parent(JoyBubble *self, JoyBubble *parent)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(!parent || JOY_IS_BUBBLE(parent));
	g_return_if_fail(self != parent);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->parent) {
		g_object_unref(priv->parent);
	}
	priv->parent = parent ? g_object_ref(parent) : NULL;
	cairo_rectangle_int_t rect = {
		0, 0,
		joy_bubble_get_width(self),
		joy_bubble_get_height(self)
	};
	cairo_region_subtract(priv->area, priv->area);
	cairo_region_union_rectangle(priv->area, &rect);
	g_object_notify_by_pspec(G_OBJECT(self), properties[PROP_PARENT]);
}

JoyBubble *
joy_bubble_get_parent(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return GET_PRIVATE(self)->parent;
}

void
joy_bubble_set_default(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	joy_bubble_set_state(self, JOY_BUBBLE_STATE_DEFAULT);
}

void
joy_bubble_set_focused(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	joy_bubble_set_state(self, JOY_BUBBLE_STATE_FOCUSED);
}

void
joy_bubble_set_active(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	joy_bubble_set_state(self, JOY_BUBBLE_STATE_ACTIVE);
}

void
joy_bubble_set_disabled(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	joy_bubble_set_state(self, JOY_BUBBLE_STATE_DISABLED);
}

void
joy_bubble_set_state(JoyBubble *self, JoyBubbleState state)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	switch (state) {
	case JOY_BUBBLE_STATE_DEFAULT:
	case JOY_BUBBLE_STATE_FOCUSED:
	case JOY_BUBBLE_STATE_ACTIVE:
	case JOY_BUBBLE_STATE_DISABLED:
		if (state == priv->state) {
			return;
		}
		priv->state = state;
		break;
	default:
		g_assert_not_reached();
	}
	damage(priv->parent, priv->x, priv->y, joy_bubble_get_width(self),
			joy_bubble_get_height(self), TRUE);
}

JoyBubbleState
joy_bubble_get_state(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), JOY_BUBBLE_STATE_DEFAULT);
	return GET_PRIVATE(self)->state;
}

void
joy_bubble_set_expand(JoyBubble *self, gboolean expand)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->parent) {
		return;
	}
	priv->horizontal_expand = expand;
	priv->vertical_expand = expand;
}

void
joy_bubble_set_horizontal_expand(JoyBubble *self, gboolean expand)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->parent) {
		return;
	}
	priv->horizontal_expand = expand;
}

gboolean
joy_bubble_get_horizontal_expand(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), FALSE);
	return GET_PRIVATE(self)->horizontal_expand;
}

void
joy_bubble_set_vertical_expand(JoyBubble *self, gboolean expand)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->parent) {
		return;
	}
	priv->vertical_expand = expand;
}

gboolean
joy_bubble_get_vertical_expand(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), FALSE);
	return GET_PRIVATE(self)->vertical_expand;
}

gboolean
joy_bubble_get_visible(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), FALSE);
	return GET_PRIVATE(self)->visible;
}

gint
joy_bubble_get_x(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), 0);
	return GET_PRIVATE(self)->x;
}

gint
joy_bubble_get_y(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), 0);
	return GET_PRIVATE(self)->y;
}

gint
joy_bubble_get_width(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), 0);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->horizontal_expand) {
		return priv->parent ? joy_bubble_get_width(priv->parent) : 0;
	}
	return priv->width;
}

gint
joy_bubble_get_height(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), 0);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->vertical_expand) {
		return priv->parent ? joy_bubble_get_height(priv->parent) : 0;
	}
	return priv->height;
}

JoyApplication *
joy_bubble_get_application(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return JOY_BUBBLE_GET_CLASS(self)->get_application(self);
}

JoyScreen *
joy_bubble_get_screen(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return JOY_BUBBLE_GET_CLASS(self)->get_screen(self);
}

JoyBubble *
joy_bubble_get_window(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return JOY_BUBBLE_GET_CLASS(self)->get_window(self);
}

PangoLayout *
joy_bubble_pango_layout_create(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	struct Private *priv = GET_PRIVATE(self);
	PangoLayout *layout = NULL;
	if (priv->style) {
		layout = joy_style_pango_layout_create(priv->style);
		if (layout) {
			return layout;
		}
	}
	if (G_LIKELY(priv->parent)) {
		JoyStyle *style = joy_bubble_get_style(priv->parent);
		if (style) {
			layout = joy_style_pango_layout_create(style);
			if (layout) {
				return layout;
			}
		}
	}
	JoyApplication *app = joy_bubble_get_application(self);
	if (G_LIKELY(app)) {
		JoyStyle *style = joy_application_get_theme(app);
		if (style) {
			layout = joy_style_pango_layout_create(style);
			if (layout) {
				return layout;
			}
		}
	}
	PangoFontMap *map = pango_cairo_font_map_get_default();
	if (G_UNLIKELY(!map)) {
		return NULL;
	}
	PangoContext *context = pango_font_map_create_context(map);
	if (G_UNLIKELY(!context)) {
		return NULL;
	}
	layout = pango_layout_new(context);
	g_object_unref(context);
	return layout;
}

void
joy_bubble_cairo_set_font_source(JoyBubble *self, cairo_t *cr)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(cr);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->style) {
		if (joy_style_cairo_set_font_source(priv->style, cr)) {
			return;
		}
	}
	if (G_LIKELY(priv->parent)) {
		JoyStyle *style = joy_bubble_get_style(priv->parent);
		if (style) {
			if (joy_style_cairo_set_font_source(style, cr)) {
				return;
			}
		}
	}
	JoyApplication *app = joy_bubble_get_application(self);
	if (G_LIKELY(app)) {
		JoyStyle *style = joy_application_get_theme(app);
		if (style) {
			if (joy_style_cairo_set_font_source(style, cr)) {
				return;
			}
		}
	}
	cairo_set_source_rgb(cr, 1., 1., 1.);
}

JoyBubble *
joy_bubble_at(JoyBubble *self, gint x, gint y)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	return JOY_BUBBLE_GET_CLASS(self)->at(self, x, y);
}

JoyBubble *
joy_bubble_at_device(JoyBubble *self, JoyDevice *device)
{
	g_return_val_if_fail(JOY_IS_BUBBLE(self), NULL);
	g_return_val_if_fail(JOY_IS_DEVICE(device), NULL);
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_UNLIKELY(!screen)) {
		return NULL;
	}
	return joy_screen_get_at_device(screen, device);
}

void
joy_bubble_move(JoyBubble *self, gint x, gint y)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->horizontal_expand && priv->vertical_expand) {
		return;
	}
	if (G_UNLIKELY(priv->x == x && priv->y == y)) {
		return;
	}
	if (priv->horizontal_expand) {
		x = 0;
	}
	if (priv->vertical_expand) {
		y = 0;
	}
	gint width = joy_bubble_get_width(self);
	gint height = joy_bubble_get_height(self);
	if (priv->parent) {
		damage(priv->parent, priv->x, priv->y, width, height, FALSE);
	}
	g_signal_emit(self, signals[SIGNAL_MOVE], 0, x, y);
	priv->x = x;
	priv->y = y;
	damage(self, 0, 0, width, height, FALSE);
}

void
joy_bubble_translate(JoyBubble *self, gint x, gint y)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	if (G_UNLIKELY((0 == x) && (0 == y))) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	joy_bubble_move(self, priv->x + x, priv->y + y);
}

void
joy_bubble_resize(JoyBubble *self, gint width, gint height)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->horizontal_expand && priv->vertical_expand) {
		return;
	}
	if (G_UNLIKELY(0 > width)) {
		width = 0;
	}
	if (G_UNLIKELY(0 > height)) {
		height = 0;
	}
	gint my_width = joy_bubble_get_width(self);
	gint my_height = joy_bubble_get_height(self);
	if ((my_width == width) && (my_height == height)) {
		return;
	}
	if (priv->horizontal_expand) {
		width = my_width;
	}
	if (priv->vertical_expand) {
		height = my_height;
	}
	if ((width < my_width || height < my_height) && priv->parent) {
		damage(priv->parent, priv->x, priv->y, my_width, my_height,
				FALSE);
	}
	cairo_rectangle_int_t rect = {
		0, 0, width, height
	};
	cairo_region_subtract(priv->area, priv->area);
	cairo_region_union_rectangle(priv->area, &rect);
	cairo_region_intersect(priv->draw, priv->area);
	g_signal_emit(self, signals[SIGNAL_RESIZE], 0, width, height);
}

void
joy_bubble_expose(JoyBubble *self, const cairo_rectangle_int_t *rect)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(rect);
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->visible)) {
		return;
	}
	if (cairo_region_contains_rectangle(priv->area, rect)
			== CAIRO_REGION_OVERLAP_OUT) {
		return;
	}
	if (cairo_region_contains_rectangle(priv->draw, rect)
			== CAIRO_REGION_OVERLAP_IN) {
		return;
	}
	cairo_region_union_rectangle(priv->draw, rect);
	cairo_region_intersect(priv->draw, priv->area);
	g_signal_emit(self, signals[SIGNAL_EXPOSE], 0, rect);
}

void
joy_bubble_key_down(JoyBubble *self, JoyDevice *device,
		gulong timestamp, gint x, gint y, JoyKeySym sym)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(JOY_BUBBLE_STATE_DISABLED == priv->state)) {
		return;
	}
	if (G_UNLIKELY(!priv->visible)) {
		return;
	}
	JoyBubble *current = joy_bubble_at(self, x, y);
	if (G_UNLIKELY(!current)) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(current, signals[SIGNAL_KEY_DOWN], detail,
			device, timestamp, x, y, sym);
}

void
joy_bubble_key_up(JoyBubble *self, JoyDevice *device,
		gulong timestamp, gint x, gint y, JoyKeySym sym)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(JOY_BUBBLE_STATE_DISABLED == priv->state)) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	JoyBubble *current = joy_bubble_at(self, x, y);
	if (G_UNLIKELY(!current)) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(current, signals[SIGNAL_KEY_UP], detail,
			device, timestamp, x, y, sym);
}

void
joy_bubble_show(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->visible) {
		return;
	}
	priv->visible = TRUE;
	if (0. == priv->alpha) {
		priv->alpha = 1.;
	}
	g_signal_emit(self, signals[SIGNAL_SHOW], 0);
	if (priv->parent) {
		damage(priv->parent, priv->x, priv->y,
				joy_bubble_get_width(self),
				joy_bubble_get_height(self),
				FALSE);
	}
}

void
joy_bubble_hide(JoyBubble *self)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->visible) {
		return;
	}
	priv->visible = FALSE;
	priv->alpha = 0.;
	g_signal_emit(self, signals[SIGNAL_HIDE], 0);
	if (priv->parent) {
		damage(priv->parent, priv->x, priv->y,
				joy_bubble_get_width(self),
				joy_bubble_get_height(self),
				FALSE);
	}
}

void
joy_bubble_motion(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(JOY_BUBBLE_STATE_DISABLED == priv->state)) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	JoyBubble *current = joy_bubble_at(self, x, y);
	JoyBubble *previous = joy_bubble_at_device(self, device);
	if (previous == current) {
		return;
	}
	if (previous) {
		joy_bubble_leave(previous, device, timestamp, x, y);
	}
	if (current) {
		joy_bubble_enter(current, device, timestamp, x, y);
		GQuark detail = 0;
		const gchar *name = joy_device_get_name(device);
		if (name) {
			detail = g_quark_from_string(name);
		}
		g_signal_emit(current, signals[SIGNAL_MOTION], detail,
				device, timestamp, x, y);
	}
}

void
joy_bubble_button_down(JoyBubble *self, JoyDevice *device,
		gulong timestamp, gint x, gint y, JoyMouseButton button)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(JOY_BUBBLE_STATE_DISABLED == priv->state)) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	JoyBubble *current = joy_bubble_at_device(self, device);
	if (G_UNLIKELY(!current)) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(current, signals[SIGNAL_BUTTON_DOWN], detail,
			current, timestamp, x, y, button);
}

void
joy_bubble_button_up(JoyBubble *self, JoyDevice *device,
		gulong timestamp, gint x, gint y, JoyMouseButton button)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(JOY_BUBBLE_STATE_DISABLED == priv->state)) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	JoyBubble *current = joy_bubble_at_device(self, device);
	if (G_UNLIKELY(!current)) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(current, signals[SIGNAL_BUTTON_UP], detail,
			device, timestamp, x, y, button);
}

void
joy_bubble_scroll(JoyBubble *self, JoyDevice *device,
		gulong timestamp, gint x, gint y, JoyMouseScroll direction)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_STATE_DISABLED == priv->state) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	JoyBubble *current = joy_bubble_at_device(self, device);
	if (current) {
		GQuark detail = 0;
		const gchar *name = joy_device_get_name(device);
		if (name) {
			detail = g_quark_from_string(name);
		}
		g_signal_emit(current, signals[SIGNAL_SCROLL], detail,
				device, timestamp, x, y, direction);
	}
}

void
joy_bubble_enter(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_STATE_DISABLED == priv->state) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(self, signals[SIGNAL_ENTER], detail,
			device, timestamp, x, y);
	JoyScreen *screen = joy_bubble_get_screen(self);
	if (G_LIKELY(screen)) {
		joy_screen_set_at_device(screen, device, self);
	}
}

void
joy_bubble_leave(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	g_return_if_fail(JOY_IS_DEVICE(device));
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_STATE_DISABLED == priv->state) {
		return;
	}
	if (!priv->visible) {
		return;
	}
	GQuark detail = 0;
	const gchar *name = joy_device_get_name(device);
	if (name) {
		detail = g_quark_from_string(name);
	}
	g_signal_emit(self, signals[SIGNAL_LEAVE], detail,
			device, timestamp, x, y);
}

void
joy_bubble_draw(JoyBubble *self, cairo_t *cr)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->visible) {
		return;
	}
	if (cairo_region_is_empty(priv->draw)) {
		return;
	}
	cairo_save(cr);
	if (G_LIKELY(priv->parent)) {
		cairo_translate(cr, priv->x, priv->y);
	}
	for (gint i = 0; i < cairo_region_num_rectangles(priv->draw); ++i) {
		cairo_rectangle_int_t rect;
		cairo_region_get_rectangle(priv->draw, i, &rect);
		cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
	}
	gboolean status;
	cairo_clip(cr);
	if (!priv->buffer) {
		g_signal_emit(self, signals[SIGNAL_DRAW], 0,
				cr, &status);
	} else {
		joy_buffer_draw(priv->buffer, self, signals[SIGNAL_DRAW],
				cr, &status);
	}
	cairo_restore(cr);
	cairo_region_subtract(priv->draw, priv->draw);
}

void
joy_bubble_damage(JoyBubble *self, gint x, gint y, gint width, gint height)
{
	g_return_if_fail(JOY_IS_BUBBLE(self));
	damage(self, x, y, width, height, TRUE);
}
