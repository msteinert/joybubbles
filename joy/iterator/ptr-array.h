/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Pointer array iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ITERATOR_PTR_ARRAY_H
#define JOY_ITERATOR_PTR_ARRAY_H

#include <joy/iterator/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ITERATOR_PTR_ARRAY \
	(joy_iterator_ptr_array_get_type())

#define JOY_ITERATOR_PTR_ARRAY(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ITERATOR_PTR_ARRAY, \
		JoyIteratorPtrArray))

#define JOY_IS_ITERATOR_PTR_ARRAY(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ITERATOR_PTR_ARRAY))

#define JOY_ITERATOR_PTR_ARRAY_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ITERATOR_PTR_ARRAY, \
		JoyIteratorPtrArrayClass))

#define JOY_IS_ITERATOR_PTR_ARRAY_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ITERATOR_PTR_ARRAY))

#define JOY_ITERATOR_PTR_ARRAY_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ITERATOR_PTR_ARRAY, \
		JoyIteratorPtrArrayClass))

typedef struct JoyIteratorPtrArrayClass_ JoyIteratorPtrArrayClass;

struct JoyIteratorPtrArray_ {
	/*< private >*/
	JoyIterator parent_instance;
	gpointer priv;
};

struct JoyIteratorPtrArrayClass_ {
	/*< private >*/
	JoyIteratorClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_iterator_ptr_array_get_type(void) G_GNUC_CONST;

/**
 * \brief Create an iterator at the given index of a pointer array.
 *
 * \param node [in] A pointer array.
 * \param index [in] The index to start iterating at.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_ptr_array_new(const GPtrArray *array, guint index);

/**
 * \brief Create an iterator at the beginning of a pointer array.
 *
 * \param node [in] A pointer array.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_ptr_array_begin(const GPtrArray *array);

/**
 * \brief Create an iterator at the end of a pointer array.
 *
 * \param array [in] A pointer array.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_ptr_array_end(const GPtrArray *array);

G_END_DECLS

#endif // JOY_ITERATOR_PTR_ARRAY_H
