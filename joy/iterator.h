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
 * \brief Generic iterator
 */

#ifndef JOY_ITERATOR_H
#define JOY_ITERATOR_H

#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ITERATOR \
	(joy_iterator_get_type())

#define JOY_ITERATOR(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ITERATOR, \
		JoyIterator))

#define JOY_IS_ITERATOR(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ITERATOR))

#define JOY_ITERATOR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ITERATOR, \
		JoyIteratorClass))

#define JOY_IS_ITERATOR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ITERATOR))

#define JOY_ITERATOR_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ITERATOR, \
		JoyIteratorClass))

typedef struct JoyIteratorClass_ JoyIteratorClass;

struct JoyIterator_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

typedef gpointer
(*JoyIteratorNextItem)(JoyIterator *self);

typedef gpointer
(*JoyIteratorPreviousItem)(JoyIterator *self);

struct JoyIteratorClass_ {
	/*< private >*/
	GObjectClass parent_class;
	JoyIteratorNextItem next_item;
	JoyIteratorPreviousItem previous_item;
};

G_GNUC_NO_INSTRUMENT
GType
joy_iterator_get_type(void) G_GNUC_CONST;

/**
 * \brief Move to the next iterator value.
 *
 * \param self [in] An iterator object.
 *
 * \return The new iterator value. If this function returns NULL then
 *         \e self has been destroyed and should no longer be referenced.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_next(JoyIterator *self);

/**
 * \brief Move to the previous iterator value.
 *
 * \param self [in] An iterator object.
 *
 * \return The new iterator value. If this function returns NULL then
 *         \e self has been destroyed and should no longer be referenced.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_previous(JoyIterator *self);

/**
 * \brief Get the current iterator item.
 *
 * \param self [in] An iterator object.
 *
 * \return The current iterator item.
 */
gpointer
joy_iterator_item(JoyIterator *self);

G_END_DECLS

#endif // JOY_ITERATOR_H
