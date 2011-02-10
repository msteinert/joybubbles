/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/iterator/queue.h"

JoyIterator *
joy_iterator_queue_new(const GQueue *queue, guint n)
{
	g_return_val_if_fail(queue, NULL);
	GList *node = g_queue_peek_nth_link((GQueue *)queue, n);
	return G_LIKELY(node) ? joy_iterator_list_new(node) : NULL;
}

JoyIterator *
joy_iterator_queue_begin(const GQueue *queue)
{
	g_return_val_if_fail(queue, NULL);
	GList *node = g_queue_peek_head_link((GQueue *)queue);
	return G_LIKELY(node) ? joy_iterator_list_new(node) : NULL;
}

JoyIterator *
joy_iterator_queue_end(const GQueue *queue)
{
	g_return_val_if_fail(queue, NULL);
	GList *node = g_queue_peek_tail_link((GQueue *)queue);
	return G_LIKELY(node) ? joy_iterator_list_new(node) : NULL;
}
