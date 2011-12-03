/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief X11 screen implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_X11_SCREEN_H
#define JOY_X11_SCREEN_H

#include "joy/platform/x11/types.h"
#include "joy/screen.h"
#include <X11/Xlib.h>

G_BEGIN_DECLS

#define JOY_TYPE_X11_SCREEN \
	(joy_x11_screen_get_type())

#define JOY_X11_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_X11_SCREEN, \
		JoyX11Screen))

#define JOY_IS_X11_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_X11_SCREEN))

#define JOY_X11_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_X11_SCREEN, \
		JoyX11ScreenClass))

#define JOY_IS_X11_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_X11_SCREEN))

#define JOY_X11_SCREEN_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_X11_SCREEN, \
		JoyX11ScreenClass))

typedef struct JoyX11ScreenClass_ JoyX11ScreenClass;

struct JoyX11Screen_ {
	/*< private >*/
	JoyScreen parent_instance;
	gpointer priv;
};

struct JoyX11ScreenClass_ {
	/*< private >*/
	JoyScreenClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_x11_screen_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new screen.
 *
 * \param app [in] The application that is creating the screen.
 * \param id [n] The screen ID.
 *
 * \return A new screen object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyScreen *
joy_x11_screen_new(JoyApplication *app, guint id);

G_GNUC_INTERNAL
void
joy_x11_screen_set_refresh(JoyScreen *self, gdouble refresh);

/**
 * \brief Get the X11 screen.
 *
 * \param self [in] An X11 screen object.
 *
 * \return The X11 Screen for \e self.
 */
G_GNUC_INTERNAL
Screen *
joy_x11_screen_get_screen(JoyScreen *self);

/**
 * \brief Get the X11 display.
 *
 * \param self [in] An X11 screen object.
 *
 * \return The X11 Display for \e self.
 */
G_GNUC_INTERNAL
Display *
joy_x11_screen_get_display(JoyScreen *self);

/**
 * \brief Get the X11 visual.
 *
 * \param self [in] An X11 screen object.
 *
 * \return The best Visual supported by \e self.
 */
G_GNUC_INTERNAL
Visual *
joy_x11_screen_get_visual(JoyScreen *self);

/**
 * \brief Get the X11 colormap.
 *
 * \param self [in] An X11 screen object.
 *
 * \return The best Colormap supported by \e self.
 */
G_GNUC_INTERNAL
Colormap
joy_x11_screen_get_colormap(JoyScreen *self);

/**
 * \brief Get the depth of a screen.
 *
 * \param self [in] An X11 screen object.
 *
 * \return The best bit depth supported by \e self.
 */
G_GNUC_INTERNAL
gint
joy_x11_screen_get_depth(JoyScreen *self);

/**
 * \brief Determine if a screen supports XComposite.
 *
 * \param self [in] An X11 screen object.
 *
 * \return \e TRUE if the server supports XComposite, \e false otherwise.
 */
G_GNUC_INTERNAL
gboolean
joy_x11_screen_has_composite(JoyScreen *self);

/**
 * \brief Create a cairo surface for the given pixmap.
 *
 * The returned surface owns the provided pixmap. It should no longer be
 * referenced by the calling function. The pixmap will be freed when the
 * the cairo surface is destroyed.
 *
 * \param self [in] An X11 screen object.
 * \param display [in] An X11 display.
 * \param pixmap [in] An X11 pixmap.
 * \param width [in] The width of \e drawable.
 * \param height [in] The height of \e drawable.
 *
 * \return A new cairo surface for \e pixmap. If an error occurred, this
 *         function will return NULL.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
cairo_surface_t *
joy_x11_screen_cairo_surface_create(JoyScreen *self, Display *display,
		Pixmap pixmap, gint width, gint height);

G_END_DECLS

#endif // JOY_X11_SCREEN_H
