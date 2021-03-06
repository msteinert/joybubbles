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
#include "joy/iterator/hash.h"

G_DEFINE_TYPE(JoyIteratorHash, joy_iterator_hash, JOY_TYPE_ITERATOR)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ITERATOR_HASH, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyIteratorHash *)instance)->priv)

struct Private {
	GHashTableIter *iterator;
};

static void
joy_iterator_hash_init(JoyIteratorHash *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->iterator) {
		g_slice_free(GHashTableIter, priv->iterator);
	}
	G_OBJECT_CLASS(joy_iterator_hash_parent_class)->finalize(base);
}

enum Properties {
	PROP_0 = 0,
	PROP_HASH_ITER,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_HASH_ITER:
		priv->iterator = g_value_get_pointer(value);
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
	if (priv->iterator) {
		gpointer key;
		if (g_hash_table_iter_next(priv->iterator, &key, NULL)) {
			return key;
		}
		g_slice_free(GHashTableIter, priv->iterator);
		priv->iterator = NULL;
	}
	return NULL;
}

static void
joy_iterator_hash_class_init(JoyIteratorHashClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	JoyIteratorClass *iterator_class = JOY_ITERATOR_CLASS(klass);
	iterator_class->next_item = next_item;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_HASH_ITER,
		g_param_spec_pointer("hash-iterator", Q_("Hash Iterator"),
			Q_("The hash table iterator"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyIterator *
joy_iterator_hash_begin(const GHashTable *hash)
{
	g_return_val_if_fail(hash, NULL);
	GHashTableIter *iterator = g_slice_new(GHashTableIter);
	if (G_LIKELY(iterator)) {
		g_hash_table_iter_init(iterator, (GHashTable *)hash);
		gpointer key;
		if (G_LIKELY(g_hash_table_iter_next(iterator, &key, NULL))) {
			JoyIterator *self =
				g_object_new(JOY_TYPE_ITERATOR_HASH,
					"item", key,
					"hash-iterator", iterator,
					NULL);
			if (G_LIKELY(self)) {
				return self;
			}
		}
		g_slice_free(GHashTableIter, iterator);
	}
	return NULL;
}
