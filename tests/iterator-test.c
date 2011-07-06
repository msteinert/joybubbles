/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <glib.h>
#include "joy/iterator/hash.h"
#include "joy/iterator/ptr.h"
#include "joy/iterator/ptr-array.h"
#include "joy/iterator/queue.h"

#define CLASS "/joy/iterator"

// some items to insert
static const gchar *const items[] = {
	"zero",
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

// number of items in "items"
static const gint n = sizeof(items) / sizeof(gchar *);

static void
test_001(gpointer *fixture, gconstpointer data)
{
	gint i;
	GQueue *queue = g_queue_new();
	g_assert(queue);
	for (i = 0; i < n; ++i) {
		g_queue_push_tail(queue, (gpointer)items[i]);
	}
	i = 0;
	for (JoyIterator *iter = joy_iterator_queue_begin(queue); iter;
			iter = joy_iterator_next(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[i++]);
	}
	g_assert_cmpint(i, ==, n);
	i = n - 1;
	for (JoyIterator *iter = joy_iterator_queue_end(queue); iter;
			iter = joy_iterator_previous(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[i--]);
	}
	g_assert_cmpint(i, ==, -1);
	g_queue_free(queue);
}

static void
test_002(gpointer *fixture, gconstpointer data)
{
	GHashTable *hash = g_hash_table_new(g_str_hash, g_str_equal);
	g_assert(hash);
	for (gint i = 0; i < n; ++i) {
		g_hash_table_insert(hash, (gpointer)items[i],
				(gpointer)items[i]);
	}
	gint i = 0;
	for (JoyIterator *iter = joy_iterator_hash_begin(hash); iter;
			iter = joy_iterator_next(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, g_hash_table_lookup(hash, item));
		++i;
	}
	g_assert_cmpint(i, ==, n);
        g_hash_table_destroy(hash);
}

static void
test_003(gpointer *fixture, gconstpointer data)
{
	GPtrArray *array = g_ptr_array_sized_new(n);
	g_assert(array);
	for (gint i = 0; i < n; ++i) {
		g_ptr_array_add(array, (gpointer)items[i]);
	}
	gint i = 0;
	for (JoyIterator *iter = joy_iterator_ptr_array_begin(array); iter;
			iter = joy_iterator_next(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[i++]);
	}
	g_assert_cmpint(i, ==, n);
	i = n - 1;
	for (JoyIterator *iter = joy_iterator_ptr_array_end(array); iter;
			iter = joy_iterator_previous(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[i--]);
	}
	g_assert_cmpint(i, ==, -1);
	g_ptr_array_free(array, TRUE);
}

static void
test_004(gpointer *fixture, gconstpointer data)
{
	gint i = 0;
	for (JoyIterator *iter = joy_iterator_ptr_new(items[0]); iter;
			iter = joy_iterator_next(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[0]);
		++i;
	}
	g_assert_cmpint(i, ==, 1);
	i = 0;
	for (JoyIterator *iter = joy_iterator_ptr_new(items[0]); iter;
			iter = joy_iterator_previous(iter)) {
		const gchar *item = joy_iterator_item(iter);
		g_assert_cmpstr(item, ==, items[0]);
		++i;
	}
	g_assert_cmpint(i, ==, 1);
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
	g_type_init();
	g_test_add(CLASS "/001", gpointer, NULL, NULL, test_001, NULL);
	g_test_add(CLASS "/002", gpointer, NULL, NULL, test_002, NULL);
	g_test_add(CLASS "/003", gpointer, NULL, NULL, test_003, NULL);
	g_test_add(CLASS "/004", gpointer, NULL, NULL, test_004, NULL);
	return g_test_run();
}
