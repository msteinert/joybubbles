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

#define _GNU_SOURCE

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "joy/macros.h"
#include "joy/sink.h"
#include "joy/source.h"
#include <sys/select.h>
#include <poll.h>

G_DEFINE_TYPE(JoySink, joy_sink, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, JOY_TYPE_SINK, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((JoySink *)instance)->priv)

struct Private {
	GPtrArray *sources;
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	gint nfds;
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
	FD_ZERO(&priv->readfds);
	FD_ZERO(&priv->writefds);
	FD_ZERO(&priv->exceptfds);
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
joy_sink_class_init(JoySinkClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	g_type_class_add_private(klass, sizeof(struct Private));
}

JoySink *
joy_sink_new(void)
{
	return g_object_new(JOY_TYPE_SINK,
			NULL);
}

static void
source_destroy(JoySource *source, JoySink *self)
{
	joy_sink_remove(self, source);
}

inline static void
set_nfds(JoySink *self)
{
	struct Private *priv = GET_PRIVATE(self);
	gint largest = 0;
	for (size_t i = 0; i < priv->sources->len; ++i) {
		JoySource *source = g_ptr_array_index(priv->sources, i);
		gint descriptor = joy_source_get_descriptor(source);
		if (descriptor > largest) {
			largest = descriptor;
		}
	}
	priv->nfds = largest + 1;
}

void
joy_sink_add(JoySink *self, JoySource *source)
{
	g_return_if_fail(JOY_IS_SINK(self));
	g_return_if_fail(JOY_IS_SOURCE(source));
	struct Private *priv = GET_PRIVATE(self);
	gint fd = joy_source_get_descriptor(source);
	GIOCondition events = joy_source_get_condition(source);
	g_ptr_array_add(priv->sources, g_object_ref_sink(source));
	if (G_IO_IN & events) {
		FD_SET(fd, &priv->readfds);
	}
	if (G_IO_OUT & events) {
		FD_SET(fd, &priv->writefds);
	}
	if (G_IO_ERR & events) {
		FD_SET(fd, &priv->exceptfds);
		g_signal_connect(source, "error", G_CALLBACK(source_destroy),
				self);
	}
	set_nfds(self);
}

void
joy_sink_remove(JoySink *self, JoySource *source)
{
	g_return_if_fail(JOY_IS_SINK(self));
	g_return_if_fail(JOY_IS_SOURCE(source));
	struct Private *priv = GET_PRIVATE(self);
	for (gint i = 0; i < (gint)priv->sources->len; ++i) {
		if (priv->sources->pdata[i] == source) {
			g_ptr_array_remove_index(priv->sources, i);
			return;
		}
	}
	gint fd = joy_source_get_descriptor(source);
	FD_CLR(fd, &priv->readfds);
	FD_CLR(fd, &priv->writefds);
	FD_CLR(fd, &priv->exceptfds);
	set_nfds(self);
}

JOY_GNUC_HOT
void
joy_sink_poll(JoySink *self, const struct timespec *timeout)
{
	g_return_val_if_fail(JOY_IS_SINK(self), 0.);
	struct Private *priv = GET_PRIVATE(self);
	gboolean prepared = FALSE;
	for (gint i = 0; i < (gint)priv->sources->len; ++i) {
		if (joy_source_prepare(priv->sources->pdata[i])) {
			joy_source_dispatch(priv->sources->pdata[i], G_IO_IN);
			prepared = TRUE;
		}
	}
	if (prepared) {
		return;
	}
	fd_set readfds = priv->readfds;
	fd_set writefds = priv->writefds;
	fd_set exceptfds = priv->exceptfds;
	gint events = pselect(priv->nfds, &readfds, &writefds, &exceptfds,
			timeout, NULL);
	if (events) {
		if (-1 == events) {
			g_message("pselect: %s", g_strerror(errno));
			return;
		}
		for (gint fd = 0; fd < priv->nfds; ++fd) {
			GIOCondition condition = 0;
			if (FD_ISSET(fd, &priv->readfds)) {
				condition |= G_IO_IN;
			}
			if (FD_ISSET(fd, &priv->writefds)) {
				condition |= G_IO_OUT;
			}
			if (FD_ISSET(fd, &priv->exceptfds)) {
				condition |= G_IO_ERR;
			}
			if (!condition) {
				continue;
			}
			JoySource *source = NULL;
			for (size_t i = 0; i < priv->sources->len; ++i) {
				source = g_ptr_array_index(priv->sources, i);
				if (joy_source_get_descriptor(source) == fd) {
					joy_source_dispatch(source, condition);
				}
			}
		}
	}
}
