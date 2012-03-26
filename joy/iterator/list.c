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
