/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/iterator/ptr.h"

G_DEFINE_TYPE(JoyIteratorPtr, joy_iterator_ptr, JOY_TYPE_ITERATOR)

static void
joy_iterator_ptr_init(JoyIteratorPtr *self)
{
}

static gpointer
item(JoyIterator *self)
{
	return NULL;
}

static void
joy_iterator_ptr_class_init(JoyIteratorPtrClass *klass)
{
	JoyIteratorClass *iterator_class = JOY_ITERATOR_CLASS(klass);
	iterator_class->next_item = item;
	iterator_class->previous_item = item;
}

JoyIterator *
joy_iterator_ptr_new(gconstpointer data)
{
	if (G_LIKELY(data)) {
		return g_object_new(JOY_TYPE_ITERATOR_PTR,
				"item", data,
				NULL);
	}
	return NULL;
}
