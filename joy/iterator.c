/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
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
	/* properties */
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
