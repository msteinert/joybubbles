/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/iterator/list.h"

G_DEFINE_TYPE(JoyIteratorList, joy_iterator_list, JOY_TYPE_ITERATOR)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ITERATOR_LIST, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyIteratorList *)instance)->priv)

struct Private {
	const GList *node;
};

static void
joy_iterator_list_init(JoyIteratorList *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_NODE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_NODE:
		priv->node = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gpointer
next_item(JoyIterator *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->node) {
		priv->node = priv->node->next;
	}
	return priv->node ? priv->node->data : NULL;
}

static gpointer
previous_item(JoyIterator *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->node) {
		priv->node = priv->node->prev;
	}
	return priv->node ? priv->node->data : NULL;
}

static void
joy_iterator_list_class_init(JoyIteratorListClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyIteratorClass *iterator_class = JOY_ITERATOR_CLASS(klass);
	iterator_class->next_item = next_item;
	iterator_class->previous_item = previous_item;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_NODE,
		g_param_spec_pointer("node", Q_("Node"), Q_("The first node"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyIterator *
joy_iterator_list_new(const GList *node)
{
	if (G_LIKELY(node)) {
		return g_object_new(JOY_TYPE_ITERATOR_LIST,
				"item", node->data,
				"node", node,
				NULL);
	}
	return NULL;
}

JoyIterator *
joy_iterator_list_begin(const GList *node)
{
	if (G_LIKELY(node)) {
		node = g_list_first((GList *)node);
		return g_object_new(JOY_TYPE_ITERATOR_LIST,
				"item", node->data,
				"node", node,
				NULL);
	}
	return NULL;
}

JoyIterator *
joy_iterator_list_end(const GList *node)
{
	if (G_LIKELY(node)) {
		node = g_list_last((GList *)node);
		return g_object_new(JOY_TYPE_ITERATOR_LIST,
				"item", node->data,
				"node", node,
				NULL);
	}
	return NULL;
}

