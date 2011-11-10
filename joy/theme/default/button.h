/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Default theme button style_default
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_STYLE_DEFAULT_BUTTON_H
#define JOY_STYLE_DEFAULT_BUTTON_H

#include <joy/style.h>

G_BEGIN_DECLS

#define JOY_TYPE_STYLE_DEFAULT_BUTTON \
	(joy_style_default_button_get_type())

#define JOY_STYLE_DEFAULT_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), \
		JOY_TYPE_STYLE_DEFAULT_BUTTON, JoyStyleDefaultButton))

#define JOY_IS_STYLE_DEFAULT_BUTTON(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_STYLE_DEFAULT_BUTTON))

#define JOY_STYLE_DEFAULT_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_STYLE_DEFAULT_BUTTON, \
		JoyStyleDefaultButtonClass))

#define JOY_IS_STYLE_DEFAULT_BUTTON_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_STYLE_DEFAULT_BUTTON))

#define JOY_STYLE_DEFAULT_BUTTON_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_STYLE_DEFAULT_BUTTON, \
		JoyStyleDefaultButtonClass))

typedef struct JoyStyleDefaultButton_ JoyStyleDefaultButton;

typedef struct JoyStyleDefaultButtonClass_ JoyStyleDefaultButtonClass;

struct JoyStyleDefaultButton_ {
	/*< private >*/
	JoyStyle parent_instance;
};

struct JoyStyleDefaultButtonClass_ {
	/*< private >*/
	JoyStyleClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_style_default_button_get_type(void) G_GNUC_CONST;

JoyStyle *
joy_style_default_button_new(JoyBubble *button);

G_END_DECLS

#endif // JOY_STYLE_DEFAULT_BUTTON_H

