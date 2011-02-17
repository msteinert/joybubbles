/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Screen abstraction
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_SCREEN_H
#define JOY_SCREEN_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_SCREEN \
	(joy_screen_get_type())

#define JOY_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_SCREEN, JoyScreen))

#define JOY_IS_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_SCREEN))

#define JOY_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_SCREEN, JoyScreenClass))

#define JOY_IS_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_SCREEN))

#define JOY_SCREEN_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_SCREEN, \
		JoyScreenClass))

typedef struct JoyScreenClass_ JoyScreenClass;

struct JoyScreen_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

typedef JoyBubble *
(*JoyScreenWindowCreate)(JoyScreen *self);

typedef cairo_surface_t *
(*JoyScreenCairoSurfaceCreate)(JoyScreen *self, gint width, gint height);

typedef JoyIterator *
(*JoyScreenBegin)(JoyScreen *self);

typedef JoyIterator *
(*JoyScreenEnd)(JoyScreen *self);

typedef void
(*JoyScreenSubmit)(JoyScreen *self);

struct JoyScreenClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	JoyScreenWindowCreate window_create;
	JoyScreenCairoSurfaceCreate cairo_surface_create;
	JoyScreenBegin begin;
	JoyScreenEnd end;
	JoyScreenSubmit submit;
};

G_GNUC_NO_INSTRUMENT
GType
joy_screen_get_type(void) G_GNUC_CONST;

/**
 * \brief Get the display associated with this screen.
 *
 * \param self [in] A screen object.
 *
 * \return The display associated with \e self.
 */
JoyApplication *
joy_screen_get_application(JoyScreen *self);

/**
 * \brief Set the theme for the screen.
 *
 * \param self [in] A screen object.
 * \param theme [in] The new theme for \e self.
 */
void
joy_screen_set_theme(JoyScreen *self, JoyTheme *theme);

/**
 * \brief Get the screen ID.
 *
 * \param self [in] A screen object.
 *
 * \return The screen ID for \e self.
 */
gint
joy_screen_get_id(JoyScreen *self);

/**
 * \brief Get the width of a screen.
 *
 * \param self [in] A screen object.
 *
 * \return The width of \e self.
 */
gint
joy_screen_get_width(JoyScreen *self);

/**
 * \brief Get the height of a screen.
 *
 * \param self [in] A screen object.
 *
 * \return The height of \e self.
 */
gint
joy_screen_get_height(JoyScreen *self);

/**
 * \brief Get the window at the given coordinates.
 *
 * \param self [in] A screen object.
 * \param x [in] The X-coordinate relative to \e self.
 * \param y [in] The Y-coordinate relative to \e self.
 *
 * \return The window at (\e x, \e y) or NULL if the coordinate does not fall
 *         within \e self.
 */
JoyBubble *
joy_screen_window_at(JoyScreen *self, gint x, gint y);

/**
 * \brief Get an iterator to the first window of a screen.
 *
 * \param self [in] A screen object.
 *
 * \return An iterator to the first window of \e self.
 */
JoyIterator *
joy_screen_begin(JoyScreen *self);

/**
 * \brief Get an iterator to the last window of a screen.
 *
 * \param self [in] A screen object.
 *
 * \return An iterator to the last window of \e self.
 */
JoyIterator *
joy_screen_end(JoyScreen *self);

/**
 * \brief Create a new window on the given screen.
 *
 * \param self [in] A screen object.
 *
 * \return A new window object.
 */
JoyBubble *
joy_screen_window_create(JoyScreen *self);

/**
 * \brief Create an empty off-screen surface.
 *
 * Sub-classes must provide an implementation of this function.
 *
 * \param self [in] A screen object.
 * \param width [in] The width of the surface.
 * \param height [in] The height of the surface.
 *
 * \return An empty Cairo surface. The caller owns the returned value and
 *         should call cairo_surface_destroy() to free memory.
 */
G_GNUC_WARN_UNUSED_RESULT
cairo_surface_t *
joy_screen_cairo_surface_create(JoyScreen *self, gint width, gint height);

/**
 * \brief Get the widget under a specified device.
 *
 * \param self [in] A screen object.
 * \param device [in] A device object.
 *
 * \return The bubble object under \e device.
 */
JoyBubble *
joy_screen_get_at_device(JoyScreen *self, JoyDevice *device);

/**
 * \brief Set the widget under a specified device.
 *
 * \param self [in] A screen object.
 * \param device [in] A device object.
 * \param at [in] A bubble object.
 */ 
void
joy_screen_set_at_device(JoyScreen *self, JoyDevice *device, JoyBubble *at);

void
joy_screen_add_animation(JoyScreen *self, JoyAnimation *animation);

void
joy_screen_remove_animation(JoyScreen *self, JoyAnimation *animation);

gboolean
joy_screen_in_animation(JoyScreen *self);

/**
 * \brief Animate all windows on a given screen.
 *
 * \param self [in] A screen object.
 */
void
joy_screen_animate(JoyScreen *self);

/**
 * \brief Draw all windows on a given screen.
 *
 * \param self [in] A screen object.
 */
void
joy_screen_draw(JoyScreen *self);

/**
 * \brief Submit a screen for updates.
 *
 * All damaged windows should be updated on the screen after calling this
 * function.
 *
 * \param self [in] A screen object.
 */
void
joy_screen_submit(JoyScreen *self);

G_END_DECLS

#endif // JOY_SCREEN_H
