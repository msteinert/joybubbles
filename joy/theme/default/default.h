/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Theme engine interface
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_THEME_DEFAULT_H
#define JOY_THEME_DEFAULT_H

#include <joy/theme.h>

G_BEGIN_DECLS

#define JOY_TYPE_THEME_DEFAULT \
	(joy_theme_default_get_type())

#define JOY_THEME_DEFAULT(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_THEME_DEFAULT, \
		JoyThemeDefault))

#define JOY_IS_THEME_DEFAULT(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_THEME_DEFAULT))

#define JOY_THEME_DEFAULT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_THEME_DEFAULT, \
		JoyThemeDefaultClass))

#define JOY_IS_THEME_DEFAULT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_THEME_DEFAULT))

#define JOY_THEME_DEFAULT_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_THEME_DEFAULT, \
		JoyThemeDefaultClass))

typedef struct JoyThemeDefault_ JoyThemeDefault;

typedef struct JoyThemeDefaultClass_ JoyThemeDefaultClass;

struct JoyThemeDefault_ {
	/*< private >*/
	JoyTheme parent_instance;
};

struct JoyThemeDefaultClass_ {
	/*< private >*/
	JoyThemeClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_theme_default_get_type(void) G_GNUC_CONST;

JoyStyle *
joy_theme_default_new(void);

G_END_DECLS

#endif // JOY_THEME_DEFAULT_H

