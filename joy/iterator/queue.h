/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Queue iterator
 * \author Mike Steinert <michael.steinert@echostar.com>
 *
 * The queue iterator is a special case of the list iterator.
 */

#ifndef JOY_ITERATOR_QUEUE_H
#define JOY_ITERATOR_QUEUE_H

#include <joy/iterator/list.h>

G_BEGIN_DECLS

/**
 * \brief Create a new queue iterator from the given item.
 *
 * \param queue [in] A queue.
 * \param n [in] The item to begin at.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_queue_new(const GQueue *queue, guint n);

/**
 * \brief Create a new queue iterator from the beginning of the queue.
 *
 * \param queue [in] A queue.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_queue_begin(const GQueue *queue);

/**
 * \brief Create a new queue iterator from the end of the queue.
 *
 * \param queue [in] A queue.
 *
 * \return A new iterator object. The returned value is owned by the caller
 *         and must destroyed by iterating to the end by calling
 *         joy_iterator_next() or joy_iterator_previous(),  or calling
 *         g_object_destroy().
 */
G_GNUC_WARN_UNUSED_RESULT
JoyIterator *
joy_iterator_queue_end(const GQueue *queue);

G_END_DECLS

#endif // JOY_ITERATOR_QUEUE_H

