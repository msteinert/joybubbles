/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Series animation combiner
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_SERIES_H
#define JOY_ANIMATION_SERIES_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_SERIES \
	(joy_animation_series_get_type())

#define JOY_ANIMATION_SERIES(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_SERIES, \
		JoyAnimationSeries))

#define JOY_IS_ANIMATION_SERIES(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_SERIES))

#define JOY_ANIMATION_SERIES_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_SERIES, \
		JoyAnimationSeriesClass))

#define JOY_IS_ANIMATION_SERIES_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_SERIES))

#define JOY_ANIMATION_SERIES_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_SERIES, \
		JoyAnimationSeriesClass))

typedef struct JoyAnimationSeriesClass_ JoyAnimationSeriesClass;

struct JoyAnimationSeries_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationSeriesClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_series_get_type(void) G_GNUC_CONST;

JoyAnimation *
joy_animation_series_new(void);

void
joy_animation_series_add(JoyAnimation *self, JoyAnimation *child);

G_END_DECLS

#endif // JOY_ANIMATION_SERIES_H
