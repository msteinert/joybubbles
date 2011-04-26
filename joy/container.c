/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/application.h"
#include "joy/container.h"
#include "joy/iterator/queue.h"
#include "joy/marshal.h"

G_DEFINE_TYPE(JoyContainer, joy_container, JOY_TYPE_BUBBLE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_CONTAINER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyContainer *)instance)->priv)

enum Signals {
	SIGNAL_ADD,
	SIGNAL_REMOVE,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

struct Private {
	GQueue *children;
};

static void
joy_container_init(JoyContainer *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->children = g_queue_new();
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->children) {
		JoyBubble *self = JOY_BUBBLE(base);
		for (GList *node = g_queue_peek_head_link(priv->children);
				node; node = node->next) {
			JoyBubble *child = node->data;
			joy_container_remove(self, child);
		}
		g_queue_free(priv->children);
		priv->children = NULL;
	}
	G_OBJECT_CLASS(joy_container_parent_class)->dispose(base);
}

static void
set_theme(JoyBubble *self, JoyStyle *theme)
{
	struct Private *priv = GET_PRIVATE(self);
	if (JOY_BUBBLE_CLASS(joy_container_parent_class)->set_theme) {
		JOY_BUBBLE_CLASS(joy_container_parent_class)->
			set_theme(self, theme);
	}
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		joy_bubble_set_theme(child, theme);
	}
}

static JoyBubble *
at(JoyBubble *self, gint x, gint y)
{
	if (JOY_BUBBLE_CLASS(joy_container_parent_class)->at(self, x, y)) {
		struct Private *priv = GET_PRIVATE(self);
		for (GList *node = g_queue_peek_tail_link(priv->children);
				node; node = node->prev) {
			JoyBubble *child = node->data;
			JoyBubble *at = joy_bubble_at(child,
					x - joy_bubble_get_x(child),
					y - joy_bubble_get_y(child));
			if (at) {
				return at;
			}
		}
		return self;
	}
	return NULL;
}

static void
resize(JoyBubble *self, gint width, gint height)
{
	struct Private *priv = GET_PRIVATE(self);
	JOY_BUBBLE_CLASS(joy_container_parent_class)->
		resize(self, width, height);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		if (joy_bubble_get_horizontal_expand(child)
				|| joy_bubble_get_vertical_expand(child)) {
			joy_bubble_resize(child, width, height);
		}
	}
}

static void
show(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		joy_bubble_show(child);
	}
}

static void
hide(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		joy_bubble_hide(child);
	}
}

static void
expose(JoyBubble *self, const cairo_rectangle_int_t *rect)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		cairo_rectangle_int_t expose = *rect;
		expose.x -= joy_bubble_get_x(child);
		expose.y -= joy_bubble_get_y(child);
		joy_bubble_expose(child, &expose);
	}
}

static gboolean
draw(JoyBubble *self, cairo_t *cr)
{
	struct Private *priv = GET_PRIVATE(self);
	for (GList *node = g_queue_peek_head_link(priv->children); node;
			node = node->next) {
		JoyBubble *child = node->data;
		joy_bubble_draw(child, cr);
	}
	return TRUE;
}

static JoyIterator *
begin(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->children)) {
		return NULL;
	}
	return joy_iterator_queue_begin(priv->children);
}

static JoyIterator *
end(JoyBubble *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->children)) {
		return NULL;
	}
	return joy_iterator_queue_end(priv->children);
}

static void
add(JoyBubble *self, JoyBubble *child)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->children)) {
		return;
	}
	if (joy_bubble_get_parent(child)) {
		return;
	}
	joy_bubble_set_parent(child, self);
	JoyStyle *theme = joy_bubble_get_theme(self);
	if (theme) {
		joy_bubble_set_theme(child, theme);
	}
	g_queue_push_tail(priv->children, g_object_ref_sink(child));
}

static void
remove_(JoyBubble *self, JoyBubble *child)
{
	struct Private *priv = GET_PRIVATE(self);
	if (G_UNLIKELY(!priv->children)) {
		return;
	}
	if (joy_bubble_get_parent(child) != self) {
		return;
	}
	joy_bubble_set_parent(child, NULL);
	g_object_run_dispose(G_OBJECT(child));
	g_object_unref(child);
	g_queue_remove(priv->children, child);
}

static void
joy_container_class_init(JoyContainerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	JoyBubbleClass *bubble_class = JOY_BUBBLE_CLASS(klass);
	bubble_class->set_theme = set_theme;
	bubble_class->at = at;
	bubble_class->resize = resize;
	bubble_class->show = show;
	bubble_class->hide = hide;
	bubble_class->expose = expose;
	bubble_class->draw = draw;
	klass->begin = begin;
	klass->end = end;
	klass->add = add;
	klass->remove = remove_;
	g_type_class_add_private(klass, sizeof(struct Private));
	// JoyContainer::add
	signals[SIGNAL_ADD] =
		g_signal_new(g_intern_static_string("add"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyContainerClass, add),
			NULL, NULL, joy_marshal_VOID__OBJECT, G_TYPE_NONE,
			1, G_TYPE_OBJECT);
	// JoyContainer::remove
	signals[SIGNAL_REMOVE] =
		g_signal_new(g_intern_static_string("remove"),
			G_OBJECT_CLASS_TYPE(klass), G_SIGNAL_RUN_FIRST,
			G_STRUCT_OFFSET(JoyContainerClass, remove),
			NULL, NULL, joy_marshal_VOID__OBJECT, G_TYPE_NONE,
			1, G_TYPE_OBJECT);
}

JoyIterator *
joy_container_begin(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_CONTAINER(self), NULL);
	return JOY_CONTAINER_GET_CLASS(self)->begin(self);
}

JoyIterator *
joy_container_end(JoyBubble *self)
{
	g_return_val_if_fail(JOY_IS_CONTAINER(self), NULL);
	return JOY_CONTAINER_GET_CLASS(self)->end(self);
}

void
joy_container_add(JoyBubble *self, JoyBubble *child)
{
	g_return_if_fail(JOY_IS_CONTAINER(self));
	g_return_if_fail(JOY_IS_BUBBLE(child));
	g_return_if_fail(joy_bubble_get_parent(child) == NULL);
	g_signal_emit(self, signals[SIGNAL_ADD], 0, child);
}

void
joy_container_remove(JoyBubble *self, JoyBubble *child)
{
	g_return_if_fail(JOY_IS_CONTAINER(self));
	g_return_if_fail(JOY_IS_BUBBLE(child));
	g_return_if_fail(joy_bubble_get_parent(child) == self);
	g_signal_emit(self, signals[SIGNAL_REMOVE], 0, child);
}
