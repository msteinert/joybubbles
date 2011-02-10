/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Doubly-linked list iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ITERATOR_LIST_H
#define JOY_ITERATOR_LIST_H

#include <joy/iterator/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ITERATOR_LIST \
	(joy_iterator_list_get_type())

#define JOY_ITERATOR_LIST(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ITERATOR_LIST, \
		JoyIteratorList))

#define JOY_IS_ITERATOR_LIST(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ITERATOR_LIST))

#define JOY_ITERATOR_LIST_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ITERATOR_LIST, \
		JoyIteratorListClass))

#define JOY_IS_ITERATOR_LIST_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ITERATOR_LIST))

#define JOY_ITERATOR_LIST_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ITERATOR_LIST, \
		JoyIteratorListClass))

typedef struct JoyIteratorListClass_ JoyIteratorListClass;

struct JoyIteratorList_ {
	/*< private >*/
	JoyIterator parent_instance;
	gpointer priv;
};

struct JoyIteratorListClass_ {
	/*< private >*/
	JoyIteratorClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_iterator_list_get_type(void) G_GNUC_CONST;

/**
 * \brief Create an iterator at the given list node.
 *
 * \param node [in] A list node.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_list_new(const GList *node);

/**
 * \brief Create an iterator at the beginning of a list.
 *
 * \param node [in] A list node.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_list_begin(const GList *node);

/**
 * \brief Create an iterator at the end of a list.
 *
 * \param node [in] A list node.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_list_end(const GList *node);

G_END_DECLS

#endif // JOY_ITERATOR_LIST_H
