/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Parallel animation combiner
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_ANIMATION_PARALLEL_H
#define JOY_ANIMATION_PARALLEL_H

#include <joy/animation/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_ANIMATION_PARALLEL \
	(joy_animation_parallel_get_type())

#define JOY_ANIMATION_PARALLEL(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_ANIMATION_PARALLEL, \
		JoyAnimationParallel))

#define JOY_IS_ANIMATION_PARALLEL(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_ANIMATION_PARALLEL))

#define JOY_ANIMATION_PARALLEL_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_ANIMATION_PARALLEL, \
		JoyAnimationParallelClass))

#define JOY_IS_ANIMATION_PARALLEL_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_ANIMATION_PARALLEL))

#define JOY_ANIMATION_PARALLEL_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_ANIMATION_PARALLEL, \
		JoyAnimationParallelClass))

typedef struct JoyAnimationParallelClass_ JoyAnimationParallelClass;

struct JoyAnimationParallel_ {
	/*< private >*/
	JoyAnimation parent_instance;
	gpointer priv;
};

struct JoyAnimationParallelClass_ {
	/*< private >*/
	JoyAnimationClass parent_class;
};

G_GNUC_NO_INSTRUMENT
GType
joy_animation_parallel_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new parallel animation.
 *
 * Parallel animations execute a number of animations simultaneously.
 *
 * \return A new parallel animation object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyAnimation *
joy_animation_parallel_new(void);

/**
 * \brief Add an animation to a parallel animation.
 *
 * \param self [in] A parallel animation object.
 * \param animation [child] The animation to add to \e self.
 */
void
joy_animation_parallel_add(JoyAnimation *self, JoyAnimation *child);

G_END_DECLS

#endif // JOY_ANIMATION_PARALLEL_H
