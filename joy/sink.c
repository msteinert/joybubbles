/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "joy/sink.h"
#include "joy/source.h"

G_DEFINE_TYPE(JoySink, joy_sink, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_SINK, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoySink *)instance)->priv)

struct Private {
	GPtrArray *sources;
	GArray *fds;
	GTimer *timer;
};

static void
joy_sink_init(JoySink *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->sources = g_ptr_array_sized_new(1);
	if (G_LIKELY(priv->sources)) {
		g_ptr_array_set_free_func(priv->sources, g_object_unref);
	}
	priv->fds = g_array_sized_new(FALSE, TRUE, sizeof(GPollFD), 1);
	priv->timer = g_timer_new();
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->sources) {
		g_ptr_array_unref(priv->sources);
		priv->sources = NULL;
	}
	G_OBJECT_CLASS(joy_sink_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->fds) {
		g_array_free(priv->fds, TRUE);
	}
	if (priv->timer) {
		g_timer_destroy(priv->timer);
	}
	G_OBJECT_CLASS(joy_sink_parent_class)->finalize(base);
}

static void
joy_sink_class_init(JoySinkClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoySink *
joy_sink_new(void)
{
	return g_object_new(JOY_TYPE_SINK, NULL);
}

static void
source_destroy(JoySource *source, JoySink *self)
{
	struct Private *priv = GET_PRIVATE(self);
	for (gint i = 0; i < priv->sources->len; ++i) {
		if (priv->sources->pdata[i] == source) {
			g_ptr_array_remove_index(priv->sources, i);
			g_array_remove_index(priv->fds, i);
			return;
		}
	}
}

void
joy_sink_add(JoySink *self, JoySource *source)
{
	g_return_if_fail(JOY_IS_SINK(self));
	g_return_if_fail(JOY_IS_SOURCE(source));
	struct Private *priv = GET_PRIVATE(self);
	g_ptr_array_add(priv->sources, g_object_ref_sink(source));
	GPollFD fd = {
		joy_source_get_descriptor(source),
		joy_source_get_condition(source),
		0
	};
	g_array_append_val(priv->fds, fd);
	if (G_LIKELY(G_IO_HUP & fd.events)) {
		g_signal_connect(source, "hangup",
				G_CALLBACK(source_destroy), self);
	}
	if (G_LIKELY(G_IO_ERR & fd.events)) {
		g_signal_connect(source, "error",
				G_CALLBACK(source_destroy), self);
	}
}

static inline gboolean
prepare_sources(GPtrArray *sources)
{
	for (gint i = 0; i < sources->len; ++i) {
		if (joy_source_prepare(sources->pdata[i])) {
			joy_source_input(sources->pdata[i]);
			return TRUE;
		}
	}
	return FALSE;
}

static inline void
poll_sources(GPtrArray *sources, GPollFD *fds, guint nfds, gint timeout)
{
	gint events = g_poll(fds, nfds, timeout);
	if (-1 == events) {
		g_message("%s", g_strerror(errno));
		return;
	}
	for (gint i = 0; i < nfds; ++i) {
		if (fds[i].revents & fds[i].events) {
			joy_source_dispatch(sources->pdata[i],
					fds[i].revents);
			return;
		}
	}
}

gdouble
joy_sink_poll(JoySink *self, gdouble seconds)
{
	g_return_val_if_fail(JOY_IS_SINK(self), 0.);
	struct Private *priv = GET_PRIVATE(self);
	gdouble elapsed = 0.;
	GPollFD *fds = &g_array_index(priv->fds, GPollFD, 0);
	guint nfds = priv->fds->len;
	g_timer_start(priv->timer);
	while (elapsed < seconds) {
		if (!prepare_sources(priv->sources)) {
			gint timeout = (gint)((seconds - elapsed) * 1000.);
			poll_sources(priv->sources, fds, nfds, timeout);
		}
		elapsed = g_timer_elapsed(priv->timer, NULL);
	}
	return elapsed;
}

gdouble
joy_sink_wait(JoySink *self, gdouble seconds)
{
	g_return_val_if_fail(JOY_IS_SINK(self), 0.);
	struct Private *priv = GET_PRIVATE(self);
	gdouble elapsed = 0.;
	GPollFD *fds = &g_array_index(priv->fds, GPollFD, 0);
	guint nfds = priv->fds->len;
	gint timeout = -1;
	g_timer_start(priv->timer);
	while (elapsed < seconds) {
		if (!prepare_sources(priv->sources)) {
			poll_sources(priv->sources, fds, nfds, timeout);
		}
		elapsed = g_timer_elapsed(priv->timer, NULL);
		timeout = (gint)((seconds - elapsed) * 1000.);
	}
	return elapsed;
}
