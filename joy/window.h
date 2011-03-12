/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Top level window abstraction
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_WINDOW_H
#define JOY_WINDOW_H

#include <cairo.h>
#include <joy/container.h>
#include <joy/device/mouse.h>
#include <joy/device/keyboard.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_WINDOW \
	(joy_window_get_type())

#define JOY_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_WINDOW, JoyWindow))

#define JOY_IS_WINDOW(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_WINDOW))

#define JOY_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_WINDOW, JoyWindowClass))

#define JOY_IS_WINDOW_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_WINDOW))

#define JOY_WINDOW_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_WINDOW, \
		JoyWindowClass))

typedef struct JoyWindowClass_ JoyWindowClass;

struct JoyWindow_ {
	/*< private >*/
	JoyContainer parent_instance;
	gpointer priv;
};

/* virtual functions */
typedef cairo_surface_t *
(*JoyWindowCairoSurfaceCreate)(JoyBubble *self);

struct JoyWindowClass_ {
	/*< private >*/
	JoyContainerClass parent_class;
	/*< public >*/
	JoyWindowCairoSurfaceCreate cairo_surface_create;
};

G_GNUC_NO_INSTRUMENT
GType
joy_window_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new window.
 *
 * \param screen [in] The screen upon which to create the new window.
 *
 * \return A new window object.
 */
G_GNUC_WARN_UNUSED_RESULT
JoyBubble *
joy_window_new(JoyScreen *screen);

/**
 * \brief Get a cairo surface for a window object.
 *
 * \param self [in] A window object.
 *
 * \return A cairo surface for \e self. The caller owns the returned value
 *         and must call cairo_surface_destroy() to free memory.
 */
G_GNUC_WARN_UNUSED_RESULT
cairo_surface_t *
joy_window_cairo_surface_create(JoyBubble *self);

G_END_DECLS

#endif // JOY_WINDOW_H
