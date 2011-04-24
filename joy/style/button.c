/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "joy/style/button.h"
#include "joy/button.h"
#include "joy/image.h"
#include "joy/label.h"

G_DEFINE_ABSTRACT_TYPE(JoyStyleButton, joy_style_button, JOY_TYPE_STYLE)

static void
joy_style_button_init(JoyStyleButton *self)
{
}

static void
joy_style_button_class_init(JoyStyleButtonClass *klass)
{
}

void
joy_style_button_configure_image(JoyStyle *self, JoyBubble *widget,
		JoyBubble *image)
{
	g_return_if_fail(JOY_IS_STYLE_BUTTON(self));
	g_return_if_fail(JOY_IS_BUTTON(widget));
	g_return_if_fail(JOY_IS_IMAGE(image));
	JoyStyleButtonClass *klass = JOY_STYLE_BUTTON_GET_CLASS(self);
	if (klass->configure_image) {
		klass->configure_image(self, widget, image);
	}
}

void
joy_style_button_draw_image(JoyStyle *self, JoyBubble *widget, cairo_t *cr,
		JoyBubble *image)
{
	g_return_if_fail(JOY_IS_STYLE_BUTTON(self));
	g_return_if_fail(JOY_IS_BUTTON(widget));
	g_return_if_fail(cr);
	g_return_if_fail(JOY_IS_IMAGE(image));
	JoyStyleButtonClass *klass = JOY_STYLE_BUTTON_GET_CLASS(self);
	if (klass->draw_image) {
		klass->draw_image(self, widget, cr, image);
	}
}

void
joy_style_button_configure_label(JoyStyle *self, JoyBubble *widget,
		JoyBubble *label)
{
	g_return_if_fail(JOY_IS_STYLE_BUTTON(self));
	g_return_if_fail(JOY_IS_BUTTON(widget));
	g_return_if_fail(JOY_IS_LABEL(label));
	JoyStyleButtonClass *klass = JOY_STYLE_BUTTON_GET_CLASS(self);
	if (klass->configure_label) {
		klass->configure_label(self, widget, label);
	}
}

void
joy_style_button_draw_label(JoyStyle *self, JoyBubble *widget, cairo_t *cr,
		JoyBubble *label)
{
	g_return_if_fail(JOY_IS_STYLE_BUTTON(self));
	g_return_if_fail(JOY_IS_BUTTON(widget));
	g_return_if_fail(cr);
	g_return_if_fail(JOY_IS_LABEL(label));
	JoyStyleButtonClass *klass = JOY_STYLE_BUTTON_GET_CLASS(self);
	if (klass->draw_label) {
		klass->draw_label(self, widget, cr, label);
	}
}
