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

/**
 * \file
 * \brief Hash table iterator
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
