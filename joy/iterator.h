/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Generic iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
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
