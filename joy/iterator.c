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
#include "joy/iterator.h"

G_DEFINE_ABSTRACT_TYPE(JoyIterator, joy_iterator, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_ITERATOR, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoyIterator *)instance)->priv)

struct Private {
	gpointer item;
};

static void
joy_iterator_init(JoyIterator *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

enum Properties {
	PROP_0 = 0,
	PROP_ITEM,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_ITEM:
		priv->item = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
joy_iterator_class_init(JoyIteratorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	g_type_class_add_private(klass, sizeof(struct Private));
	// properties
	g_object_class_install_property(object_class, PROP_ITEM,
		g_param_spec_pointer("item", Q_("Item"), Q_("The first item"),
			G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

JoyIterator *
joy_iterator_next(JoyIterator *self)
{
	g_return_val_if_fail(JOY_IS_ITERATOR(self), NULL);
	gpointer next = NULL;
	JoyIteratorClass *klass = JOY_ITERATOR_GET_CLASS(self);
	if (G_LIKELY(klass->next_item)) {
		next = klass->next_item(self);
	} else {
		g_warning(Q_("%s: unsupported operation for type '%s'"),
				G_STRFUNC, G_OBJECT_TYPE_NAME(self));
	}
	if (next) {
		GET_PRIVATE(self)->item = next;
		return self;
	} else {
		g_object_unref(self);
		return NULL;
	}
}

JoyIterator *
joy_iterator_previous(JoyIterator *self)
{
	g_return_val_if_fail(JOY_IS_ITERATOR(self), NULL);
	gpointer previous = NULL;
	JoyIteratorClass *klass = JOY_ITERATOR_GET_CLASS(self);
	if (G_LIKELY(klass->previous_item)) {
		previous = klass->previous_item(self);
	} else {
		g_warning(Q_("%s: unsupported operation for type '%s'"),
				G_STRFUNC, G_OBJECT_TYPE_NAME(self));
	}
	if (previous) {
		GET_PRIVATE(self)->item = previous;
		return self;
	} else {
		g_object_unref(self);
		return NULL;
	}
}

gpointer
joy_iterator_item(JoyIterator *self)
{
	g_return_val_if_fail(JOY_IS_ITERATOR(self), NULL);
	return GET_PRIVATE(self)->item;
}
