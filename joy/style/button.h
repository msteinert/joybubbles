/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Button style
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_STYLE_BUTTON_H
#define JOY_STYLE_BUTTON_H

#include <joy/style/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_STYLE_BUTTON \
	(joy_style_button_get_type())

#define JOY_STYLE_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_STYLE_BUTTON, \
		JoyStyleButton))

#define JOY_IS_STYLE_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_STYLE_BUTTON))

#define JOY_STYLE_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_STYLE_BUTTON, \
		JoyStyleButtonClass))

#define JOY_IS_STYLE_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_STYLE_BUTTON))

#define JOY_STYLE_BUTTON_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_STYLE_BUTTON, \
		JoyStyleButtonClass))

typedef struct JoyStyleButtonClass_ JoyStyleButtonClass;

struct JoyStyleButton_ {
	/*< private >*/
	JoyStyle parent_instance;
};

typedef void
(*JoyStyleConfigure)(JoyStyle *self, JoyBubble *widget, JoyBubble *configure);

typedef void
(*JoyStyleButtonDraw)(JoyStyle *self, JoyBubble *widget, cairo_t *cr,
		JoyBubble *draw);

struct JoyStyleButtonClass_ {
	/*< private >*/
	JoyStyleClass parent_class;
	/*< public >*/
	JoyStyleConfigure configure_image;
	JoyStyleButtonDraw draw_image;
	JoyStyleConfigure configure_label;
	JoyStyleButtonDraw draw_label;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_button_get_type(void) G_GNUC_CONST;

void
joy_style_button_configure_image(JoyStyle *self, JoyBubble *widget,
		JoyBubble *image);

void
joy_style_button_draw_image(JoyStyle *self, JoyBubble *widget, cairo_t *cr,
		JoyBubble *image);

void
joy_style_button_configure_label(JoyStyle *self, JoyBubble *widget,
		JoyBubble *label);

void
joy_style_button_draw_label(JoyStyle *self, JoyBubble *widget, cairo_t *cr,
		JoyBubble *label);

G_END_DECLS

#endif // JOY_STYLE_BUTTON_H
