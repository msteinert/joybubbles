/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Single-item iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ITERATOR_PTR_H
#define JOY_ITERATOR_PTR_H

#include <joy/iterator/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ITERATOR_PTR \
	(joy_iterator_ptr_get_type())

#define JOY_ITERATOR_PTR(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ITERATOR_PTR, \
		JoyIteratorPtr))

#define JOY_IS_ITERATOR_PTR(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ITERATOR_PTR))

#define JOY_ITERATOR_PTR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ITERATOR_PTR, \
		JoyIteratorPtrClass))

#define JOY_IS_ITERATOR_PTR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ITERATOR_PTR))

#define JOY_ITERATOR_PTR_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ITERATOR_PTR, \
		JoyIteratorPtrClass))

typedef struct JoyIteratorPtrClass_ JoyIteratorPtrClass;

struct JoyIteratorPtr_ {
	/*< private >*/
	JoyIterator parent_instance;
};

struct JoyIteratorPtrClass_ {
	/*< private >*/
	JoyIteratorClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_iterator_ptr_get_type(void) G_GNUC_CONST;

/**
 * \brief Create an iterator for a single item.
 *
 * \param data [in] A pointer to something.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_ptr_new(gconstpointer data);

G_END_DECLS

#endif // JOY_ITERATOR_PTR_H
