/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Hash table iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
 *
 * This class can be used to iterate over the keys of a hash table.
 */

#ifndef JOY_ITERATOR_HASH_H
#define JOY_ITERATOR_HASH_H

#include <joy/iterator/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ITERATOR_HASH \
	(joy_iterator_hash_get_type())

#define JOY_ITERATOR_HASH(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ITERATOR_HASH, \
		JoyIteratorHash))

#define JOY_IS_ITERATOR_HASH(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ITERATOR_HASH))

#define JOY_ITERATOR_HASH_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ITERATOR_HASH, \
		JoyIteratorHashClass))

#define JOY_IS_ITERATOR_HASH_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ITERATOR_HASH))

#define JOY_ITERATOR_HASH_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ITERATOR_HASH, \
		JoyIteratorHashClass))

typedef struct JoyIteratorHashClass_ JoyIteratorHashClass;

struct JoyIteratorHash_ {
	/*< private >*/
	JoyIterator parent_instance;
	gpointer priv;
};

struct JoyIteratorHashClass_ {
	/*< private >*/
	JoyIteratorClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_iterator_hash_get_type(void) G_GNUC_CONST;

/**
 * \brief Create an iterator at the beginning of a hash table.
 *
 * \param hash [in] A hash table.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_hash_begin(const GHashTable *hash);

G_END_DECLS

#endif // JOY_ITERATOR_HASH_H

