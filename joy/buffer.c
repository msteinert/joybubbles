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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/bubble.h"
#include "joy/buffer.h"
#include "joy/screen.h"

struct JoyBuffer_ {
	cairo_surface_t *surface;
	cairo_region_t *damage;
	gint width;
	gint height;
	gboolean paint_with_alpha;
	gdouble alpha;
};

JoyBuffer *
joy_buffer_new(void)
{
	JoyBuffer *self = g_slice_new0(struct JoyBuffer_);
	if (G_UNLIKELY(!self)) {
		goto error;
	}
	self->damage = cairo_region_create();
	if (G_UNLIKELY(!self->damage)) {
		goto error;
	}
exit:
	return self;
error:
	joy_buffer_destroy(self);
	goto exit;
}

void
joy_buffer_destroy(JoyBuffer *self)
{
	if (!self) {
		return;
	}
	if (self->damage) {
		cairo_region_destroy(self->damage);
	}
	if (self->surface) {
		cairo_surface_destroy(self->surface);
	}
	g_slice_free(struct JoyBuffer_, self);
}

void
joy_buffer_damage(JoyBuffer *self, const cairo_region_t *damage)
{
	g_return_if_fail(self);
	cairo_region_union(self->damage, damage);
}

void
joy_buffer_set_alpha(JoyBuffer *self, gdouble alpha)
{
	g_return_if_fail(self);
	alpha = CLAMP(alpha, 0., 1.);
	if (1. == alpha) {
		self->paint_with_alpha = FALSE;
	} else {
		self->paint_with_alpha = TRUE;
		self->alpha = alpha;
	}
}

gdouble
joy_buffer_get_alpha(JoyBuffer *self)
{
	g_return_val_if_fail(self, 0.);
	return self->paint_with_alpha ? self->alpha : 1.;
}

void
joy_buffer_draw(JoyBuffer *self, JoyBubble *widget, guint id, cairo_t *cr,
		gboolean *status)
{
	g_return_if_fail(self);
	g_return_if_fail(JOY_IS_BUBBLE(widget));
	g_return_if_fail(cr);
	gint width = joy_bubble_get_width(widget);
	gint height = joy_bubble_get_height(widget);
	if ((width != self->width) || (height != self->height)) {
		if (self->surface) {
			cairo_surface_destroy(self->surface);
			self->surface = NULL;
		}
		self->width = width;
		self->height = height;
	}
	if (G_UNLIKELY(!self->width || !self->height)) {
		return;
	}
	if (G_UNLIKELY(!self->surface)) {
		JoyScreen *screen = joy_bubble_get_screen(widget);
		if (G_UNLIKELY(!screen)) {
			return;
		}
		self->surface = joy_screen_cairo_surface_create(screen,
				self->width, self->height);
		if (G_UNLIKELY(!self->surface)) {
			return;
		}
		cairo_rectangle_int_t rect = {
			0, 0, self->width, self->height
		};
		cairo_region_union_rectangle(self->damage, &rect);
	}
	if (G_UNLIKELY(!cairo_region_is_empty(self->damage))) {
		cairo_t *cr = cairo_create(self->surface);
		gint n = cairo_region_num_rectangles(self->damage);
		for (gint i = 0; i < n; ++i) {
			cairo_rectangle_int_t rect;
			cairo_region_get_rectangle(self->damage, i, &rect);
			cairo_rectangle(cr, rect.x, rect.y,
					rect.width, rect.height);
		}
		cairo_clip(cr);
		cairo_save(cr);
		cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
		cairo_paint(cr);
		cairo_restore(cr);
		g_signal_emit(widget, id, 0, cr, status);
		cairo_destroy(cr);
		cairo_region_subtract(self->damage, self->damage);
	}
	cairo_set_source_surface(cr, self->surface, 0., 0.);
	if (!self->paint_with_alpha) {
		cairo_paint(cr);
	} else {
		cairo_paint_with_alpha(cr, self->alpha);
	}
}
