/* Copyright 2011 EchoStar Corporation
 * 
 * EchoStar Corporation
 * 100 Inverness Terrace East
 * Englewood, CO 80112
 */

/**
 * \file
 * \brief Abstract base class for UI elements, i.e., widgets
 * \author Mike Steinert <michael.steinert@echostar.com>
 */

#ifndef JOY_BUBBLE_H
#define JOY_BUBBLE_H

#include <cairo.h>
#include <glib-object.h>
#include <joy/device/mouse.h>
#include <joy/device/keyboard.h>
#include <joy/types.h>

G_BEGIN_DECLS

#define JOY_TYPE_BUBBLE \
	(joy_bubble_get_type())

#define JOY_BUBBLE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), JOY_TYPE_BUBBLE, JoyBubble))

#define JOY_IS_BUBBLE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), JOY_TYPE_BUBBLE))

#define JOY_BUBBLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), JOY_TYPE_BUBBLE, JoyBubbleClass))

#define JOY_IS_BUBBLE_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), JOY_TYPE_BUBBLE))

#define JOY_BUBBLE_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), JOY_TYPE_BUBBLE, \
		JoyBubbleClass))

typedef struct JoyBubbleClass_ JoyBubbleClass;

struct JoyBubble_ {
	/*< private >*/
	GInitiallyUnowned parent_instance;
	gpointer priv;
};

/* virtual functions */
typedef JoyApplication *
(*JoyBubbleGetApplication)(JoyBubble *self);

typedef JoyScreen *
(*JoyBubbleGetScreen)(JoyBubble *self);

typedef JoyBubble *
(*JoyBubbleGetWindow)(JoyBubble *self);

typedef void
(*JoyBubbleSetBuffered)(JoyBubble *self, gboolean buffered);

typedef gboolean
(*JoyBubbleGetBuffered)(JoyBubble *self);

typedef JoyBubble *
(*JoyBubbleAt)(JoyBubble *self, gint x, gint y);

typedef void
(*JoyBubbleMove)(JoyBubble *self, gint x, gint y);

typedef void
(*JoyBubbleResize)(JoyBubble *self, gint width, gint height);

typedef void
(*JoyBubbleExpose)(JoyBubble *self, const cairo_rectangle_int_t *rect);

typedef void
(*JoyBubbleKey)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym);

typedef void
(*JoyBubbleMotion)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

typedef void
(*JoyBubbleVisibility)(JoyBubble *self);

typedef void
(*JoyBubbleButton)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button);

typedef void
(*JoyBubbleScroll)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyScroll direction);

typedef void
(*JoyBubbleCrossing)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

typedef void
(*JoyBubbleFocus)(JoyBubble *self, JoyDevice *device);

typedef void
(*JoyBubbleDraw)(JoyBubble *self, cairo_t *cr);

struct JoyBubbleClass_ {
	/*< private >*/
	GInitiallyUnownedClass parent_class;
	/*< public >*/
	JoyBubbleGetApplication get_application;
	JoyBubbleGetScreen get_screen;
	JoyBubbleGetWindow get_window;
	JoyBubbleSetBuffered set_buffered;
	JoyBubbleGetBuffered get_buffered;
	JoyBubbleAt at;
	JoyBubbleMove move;
	JoyBubbleResize resize;
	JoyBubbleExpose expose;
	JoyBubbleKey key_down;
	JoyBubbleKey key_up;
	JoyBubbleMotion motion;
	JoyBubbleVisibility show;
	JoyBubbleVisibility hide;
	JoyBubbleButton button_down;
	JoyBubbleButton button_up;
	JoyBubbleScroll scroll;
	JoyBubbleCrossing enter;
	JoyBubbleCrossing leave;
	JoyBubbleFocus focus;
	JoyBubbleFocus unfocus;
	JoyBubbleDraw draw;
};

G_GNUC_NO_INSTRUMENT
GType
joy_bubble_get_type(void) G_GNUC_CONST;

void
joy_bubble_set_name(JoyBubble *self, const gchar *name);

const gchar *
joy_bubble_get_name(JoyBubble *self);

GQuark
joy_bubble_get_quark(JoyBubble *self);

/**
 * \brief Set back-buffering for a widget.
 *
 * When \e buffered is TRUE then a new back-buffer will be allocated for
 * \e self. All drawing will be done via the back-buffer until this
 * function is called again with \e buffered set to FALSE.
 *
 * Back-buffered widgets use more graphics memory, however using the back-
 * buffer may increase rendering speed for widgets that are not resized often.
 *
 * \param self [in] A widget object.
 * \param buffered [in] The new buffer setting.
 */
void
joy_bubble_set_buffered(JoyBubble *self, gboolean buffered);

/**
 * \brief Determine if a widget is using a back-buffer.
 *
 * \param self [in] A widget object.
 *
 * \return TRUE if \e self is buffered, \e FALSE otherwise.
 */
gboolean
joy_bubble_get_buffered(JoyBubble *self);

void
joy_bubble_set_alpha(JoyBubble *self, gdouble alpha);

gdouble
joy_bubble_get_alpha(JoyBubble *self);

/**
 * \brief Set the parent of a widget.
 *
 * This function is only useful for container implementations. Calling this
 * function from application code may have undesirable results.
 *
 * \param self [in] A widget object.
 * \param parent [in] The new parent of \e self.
 */
void
joy_bubble_set_parent(JoyBubble *self, JoyBubble *parent);

/**
 * \brief Get the parent of a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The parent of \e self.
 */
JoyBubble *
joy_bubble_get_parent(JoyBubble *self);

void
joy_bubble_set_expand(JoyBubble *self, gboolean expand);

void
joy_bubble_set_horizontal_expand(JoyBubble *self, gboolean expand);

gboolean
joy_bubble_get_horizontal_expand(JoyBubble *self);

void
joy_bubble_set_vertical_expand(JoyBubble *self, gboolean expand);

gboolean
joy_bubble_get_vertical_expand(JoyBubble *self);

/**
 * \brief Get the visibility of a widget.
 *
 * \param self [in] A widget object.
 *
 * \return \e TRUE if \e self is visible, \e FALSE otherwise.
 */
gboolean
joy_bubble_get_visible(JoyBubble *self);

/**
 * \brief Get the X-coordinate for a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The X-coordinate of \e self relative to its parent container.
 */
gint
joy_bubble_get_x(JoyBubble *self);

/**
 * \brief Get the Y-coordinate for a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The Y-coordinate of \e self relative to its parent container.
 */
gint
joy_bubble_get_y(JoyBubble *self);

/**
 * \brief Get the width of a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The width of \e self.
 */
gint
joy_bubble_get_width(JoyBubble *self);

/**
 * \brief Get the height of a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The height of \e self.
 */
gint
joy_bubble_get_height(JoyBubble *self);

/**
 * \brief Get the application a widget is running in.
 *
 * \param self [in] A widget object.
 *
 * \return The application \e self is running in.
 */
JoyApplication *
joy_bubble_get_application(JoyBubble *self);

/**
 * \brief Get the screen a widget is on.
 *
 * \param self [in] A widget object.
 *
 * \return The screen \e self is on.
 */
JoyScreen *
joy_bubble_get_screen(JoyBubble *self);

/**
 * \brief Get the window a widget is in.
 *
 * \param self [in] A widget object.
 *
 * \return The window \e self is in.
 */
JoyBubble *
joy_bubble_get_window(JoyBubble *self);

/**
 * \brief Get the widget at the given coordinates.
 *
 * \param self [in] A widget.
 * \param x [in] The X-coordinate relative to \e self.
 * \param y [in] The Y-coordinate relative to \e self.
 *
 * \return The widget at (\e x, \e y) or NULL if the coordinate does not fall
 *         within \e self.
 */
JoyBubble *
joy_bubble_at(JoyBubble *self, gint x, gint y);

JoyBubble *
joy_bubble_at_device(JoyBubble *self, JoyDevice *device);

void
joy_bubble_move(JoyBubble *self, gint x, gint y);

void
joy_bubble_translate(JoyBubble *self, gint x, gint y);

void
joy_bubble_resize(JoyBubble *self, gint width, gint height);

void
joy_bubble_expose(JoyBubble *self, const cairo_rectangle_int_t *rect);

void
joy_bubble_key_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym);

void
joy_bubble_key_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym);

void
joy_bubble_show(JoyBubble *self);

void
joy_bubble_hide(JoyBubble *self);

void
joy_bubble_motion(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

void
joy_bubble_button_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button);

void
joy_bubble_button_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyButton button);

void
joy_bubble_scroll(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyScroll direction);

void
joy_bubble_enter(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

void
joy_bubble_leave(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

void
joy_bubble_draw(JoyBubble *self, cairo_t *cr);

/**
 * \brief Mark an area of a widget for update on the next draw.
 *
 * \param self [in] A widget object.
 * \param x [in] The X-coordinate relative to \e self.
 * \param y [in] The Y-coordinate relative to \e self.
 * \param width [in] The width of the damage area.
 * \param height [in] The height of the damage area.
 */
void
joy_bubble_damage(JoyBubble *self, gint x, gint y, gint width, gint height);

G_END_DECLS

#endif // JOY_BUBBLE_H
