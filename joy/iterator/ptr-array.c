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
#include "joy/iterator/ptr-array.h"

G_DEFINE_TYPE(JoyIteratorPtrArray, joy_iterator_ptr_array, JOY_TYPE_ITERATOR)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ITERATOR_PTR_ARRAY, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyIteratorPtrArray *)instance)->priv)

struct Private {
	const GPtrArray *array;
	guint index;
};

static void
joy_iterator_ptr_array_init(JoyIteratorPtrArray *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_ARRAY,
	PROP_INDEX,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_ARRAY:
		priv->array = g_value_get_pointer(value);
		break;
	case PROP_INDEX:
		priv->index = g_value_get_uint(value);
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
	if (++priv->index < priv->array->len) {
		return priv->array->pdata[priv->index];
	}
	return NULL;
}

static gpointer
previous_item(JoyIterator *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->index > 0) {
		return priv->array->pdata[--priv->index];
	}
	return NULL;
}

static void
joy_iterator_ptr_array_class_init(JoyIteratorPtrArrayClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	JoyIteratorClass *iterator_class = JOY_ITERATOR_CLASS(klass);
	iterator_class->next_item = next_item;
	iterator_class->previous_item = previous_item;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_ARRAY,
		g_param_spec_pointer("array", Q_("Array"),
			Q_("The pointer array"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property(object_class, PROP_INDEX,
		g_param_spec_uint("index", Q_("Index"),
			Q_("The current index"), 0, G_MAXUINT, 0,
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyIterator *
joy_iterator_ptr_array_new(const GPtrArray *array, guint index)
{
	if (G_LIKELY(array && (0 < array->len))) {
		return g_object_new(JOY_TYPE_ITERATOR_PTR_ARRAY,
				"item", array->pdata[index],
				"array", array,
				"index", index,
				NULL);
	}
	return NULL;
}

JoyIterator *
joy_iterator_ptr_array_begin(const GPtrArray *array)
{
	return joy_iterator_ptr_array_new(array, 0);
}

JoyIterator *
joy_iterator_ptr_array_end(const GPtrArray *array)
{
	return joy_iterator_ptr_array_new(array, array->len - 1);
}
