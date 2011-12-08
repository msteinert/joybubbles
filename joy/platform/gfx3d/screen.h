/* Copyright 2011 EchoStar Corporation
 *
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief GFX3D screen implementation
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_GFX3D_SCREEN_H
#define JOY_GFX3D_SCREEN_H

#include <gfx3d.h>
#include <gfx3d_private.h>
#include "joy/platform/gfx3d/types.h"
#include "joy/screen.h"

G_BEGIN_DECLS

#define JOY_TYPE_GFX3D_SCREEN \
	(joy_gfx3d_screen_get_type())

#define JOY_GFX3D_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_GFX3D_SCREEN, \
		JoyGfx3dScreen))

#define JOY_IS_GFX3D_SCREEN(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_GFX3D_SCREEN))

#define JOY_GFX3D_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_GFX3D_SCREEN, \
		JoyGfx3dScreenClass))

#define JOY_IS_GFX3D_SCREEN_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_GFX3D_SCREEN))

#define JOY_GFX3D_SCREEN_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_GFX3D_SCREEN, \
		JoyGfx3dScreenClass))

typedef struct JoyGfx3dScreenClass_ JoyGfx3dScreenClass;

struct JoyGfx3dScreen_ {
	/*< private >*/
	JoyScreen parent_instance;
	gpointer priv;
};

struct JoyGfx3dScreenClass_ {
	/*< private >*/
	JoyScreenClass parent_class;
};

G_GNUC_INTERNAL
G_GNUC_NO_INSTRUMENT
GType
joy_gfx3d_screen_get_type(void) G_GNUC_CONST;

/**
 * \brief Create a new screen.
 *
 * \param app [in] The application that is creating the screen.
 * \param id [in] The screen ID.
 * \param width [in] The width of the screen.
 * \param height [in] The height of the screen.
 *
 * \return A new screen object.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
JoyScreen *
joy_gfx3d_screen_new(JoyApplication *app, guint id, gint width, gint height);

G_GNUC_INTERNAL
void
joy_gfx3d_screen_set_vsync(JoyScreen *self, gboolean vsync);

/**
 * \brief Get the GFX3D display.
 *
 * \param self [in] A GFX3D screen object.
 *
 * \param The GFX3D display for \e self.
 */
G_GNUC_INTERNAL
GFX3D_Display
joy_gfx3d_screen_get_display(JoyScreen *self);

/**
 * \brief Create a cairo surface for the given GFX3D image.
 *
 * The returned surface owns the provided GFX3D image. It should no longer be
 * referenced by the calling function. The GFX3D image will be freed when
 * cairo surface is destroyed.
 *
 * \param self [in] A GFX3D screen object.
 * \param image [in] A GFX3D image handle.
 */
G_GNUC_INTERNAL
G_GNUC_WARN_UNUSED_RESULT
cairo_surface_t *
joy_gfx3d_screen_cairo_surface_create(JoyScreen *self, GFX3D_Image image);

G_GNUC_INTERNAL
JoyDevice *
joy_gfx3d_screen_get_mouse(JoyScreen *self);

G_GNUC_INTERNAL
JoyDevice *
joy_gfx3d_screen_get_keyboard(JoyScreen *self);

/**
 * \brief Set the cursor image.
 *
 * \param self [in] A GFX3D screen object.
 * \param cursor [in] The new cursor image.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_set_cursor(JoyScreen *self, JoyCursor *cursor);

/**
 * \brief Warp the cursor to absolute coordinates.
 *
 * \param self [in] A GFX3D screen object.
 * \param x [in] The new X-coordinate for the cursor.
 * \param y [in] The new Y-coordinate for the cursor.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_warp_cursor(JoyScreen *self, gint x, gint y);

/**
 * \brief Move the cursor relative to the current position.
 *
 * \param self [in] A GFX3D screen object.
 * \param x [in] The amount to move the cursor in the X direction.
 * \param y [in] The amount to move the cursor in the Y direction.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_move_cursor(JoyScreen *self, gint x, gint y);

/**
 * \brief Get the position of the cursor on a screen.
 *
 * \param self [in] A GFX3D screen object.
 * \param x [out] The current cursor X-coordinate.
 * \param y [out] the current cursor Y-coordinate.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_get_cursor_position(JoyScreen *self, gint *x, gint *y);

/**
 * \brief Show the cursor.
 *
 * \param self [in] A GFX3D screen object.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_show_cursor(JoyScreen *self);

/**
 * \brief Hide the cursor.
 *
 * \param self [in] A GFX3D screen object.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_hide_cursor(JoyScreen *self);

/**
 * \brief Determine if the cursor is visible.
 *
 * \param self [in] A GFX3D screen object.
 *
 * \return TRUE if the cursor is visible, FALSE otherwise.
 */
G_GNUC_INTERNAL
gboolean
joy_gfx3d_screen_cursor_visible(JoyScreen *self);

/**
 * \brief Raise a window to the top of the window stack.
 *
 * \param self [in] A GFX3D screen object.
 * \param window [in] A GFX3D window object.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_raise_window(JoyScreen *self, JoyBubble *window);

/**
 * \brief Lower a window to the bottom of the window stack.
 *
 * \param self [in] A GFX3D screen object.
 * \param window [in] A GFX3D window object.
 */
G_GNUC_INTERNAL
void
joy_gfx3d_screen_lower_window(JoyScreen *self, JoyBubble *window);

G_END_DECLS

#endif // JOY_GFX3D_SCREEN_H
