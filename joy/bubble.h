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
#include <pango/pango.h>

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

typedef enum  {
	JOY_BUBBLE_STATE_DEFAULT,
	JOY_BUBBLE_STATE_FOCUSED,
	JOY_BUBBLE_STATE_ACTIVE,
	JOY_BUBBLE_STATE_DISABLED
} JoyBubbleState;

typedef struct JoyBubbleClass_ JoyBubbleClass;

struct JoyBubble_ {
	/*< private >*/
	GInitiallyUnowned parent_instance;
	gpointer priv;
};

/* virtual functions */
typedef void
(*JoyBubbleSetStyle)(JoyBubble *self, JoyTheme *theme);

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
		gint x, gint y, JoyMouseButton button);

typedef void
(*JoyBubbleScroll)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseScroll direction);

typedef void
(*JoyBubbleCrossing)(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

typedef gboolean
(*JoyBubbleDraw)(JoyBubble *self, cairo_t *cr);

struct JoyBubbleClass_ {
	/*< private >*/
	GInitiallyUnownedClass parent_class;
	/*< public >*/
	JoyBubbleSetStyle set_style;
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
	JoyBubbleDraw draw;
};

G_GNUC_NO_INSTRUMENT
GType
joy_bubble_get_type(void) G_GNUC_CONST;

/**
 * \brief Set the name of a widget.
 *
 * This function is generally only useful for changing the style applied to
 * a widget. Calling this function may invalidate the widget style and
 * render the widget unable to draw itself.
 *
 * \param self [in] A widget object.
 * \param name [in] The new name of \e self.
 */
void
joy_bubble_set_name(JoyBubble *self, const gchar *name);

/**
 * \brief Get the name of a widget.
 *
 * If no name was previously set with joy_bubble_set_name() then the class
 * type name is returned. Theme engines should use this value to determine
 * what style to apply to a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The name of \e self (cannot be NULL).
 */
const gchar *
joy_bubble_get_name(JoyBubble *self);

/**
 * \brief Set the widget style.
 *
 * The function is generally only useful for screen & container
 * implementations. Calling this function from application code may have
 * undesirable results.
 *
 * \param self [in] A widget object.
 * \param theme [in] A theme containing the new style for \e self.
 */
void
joy_bubble_set_style(JoyBubble *self, JoyTheme *theme);

/**
 * \brief Get the style applied a widget.
 *
 * \param self [in] A widget object.
 *
 * \return The currently applied style for \e self.
 */
JoyStyle *
joy_bubble_get_style(JoyBubble *self);

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

/**
 * \brief Set the widget alpha value.
 *
 * The alpha value is applied globally to the entire widget after it has been
 * drawn, i.e., alpha values drawn by the widget will be composited with the
 * global alpha. If a widget is not back-buffered then setting alpha values
 * other than 0.0 (hides a visible widget) or 1.0 (shows a hidden widget) have
 * no effect.
 *
 * \param self [in] A widget object.
 * \param alpha [in] The new alpha value for \e self.
 */
void
joy_bubble_set_alpha(JoyBubble *self, gdouble alpha);

/**
 * \brief Get the widget alpha value.
 *
 * \brief self [in] A widget object.
 *
 * \return The alpha value for \e self.
 */
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
joy_bubble_set_default(JoyBubble *self);

void
joy_bubble_set_focused(JoyBubble *self);

void
joy_bubble_set_active(JoyBubble *self);

void
joy_bubble_set_disabled(JoyBubble *self);

void
joy_bubble_set_state(JoyBubble *self, JoyBubbleState state);

JoyBubbleState
joy_bubble_get_state(JoyBubble *self);

/**
 * \brief Simultaneously set the horizontal & vertical expand.
 *
 * \param self [in] A widget object.
 * \param expand [in] The new expand value.
 */
void
joy_bubble_set_expand(JoyBubble *self, gboolean expand);

/**
 * \brief Set a widget's horizontal expand.
 *
 * If \e expand is TRUE then \e self will expand horizontally to fill the
 * container it is in.
 *
 * \param self [in] A widget object.
 * \param expand [in] Indicates if \e self should expand or not.
 */
void
joy_bubble_set_horizontal_expand(JoyBubble *self, gboolean expand);

/**
 * \brief Determine if a widget should expand horizontally.
 *
 * \param self [in] A widget object.
 *
 * \return TRUE if \e self should expand, FALSE otherwise.
 */
gboolean
joy_bubble_get_horizontal_expand(JoyBubble *self);

/**
 * \brief Set a widget's vertical expand.
 *
 * If \e expand is TRUE then \e self will expand vertically to fill the
 * container it is in.
 *
 * \param self [in] A widget object.
 * \param expand [in] Indicates if \e self should expand or not.
 */
void
joy_bubble_set_vertical_expand(JoyBubble *self, gboolean expand);

/**
 * \brief Determine if a widget should expand vertically.
 *
 * \param self [in] A widget object.
 *
 * \return TRUE if \e self should expand, FALSE otherwise.
 */
gboolean
joy_bubble_get_vertical_expand(JoyBubble *self);

/**
 * \brief Get the visibility of a widget.
 *
 * \param self [in] A widget object.
 *
 * \return TRUE if \e self is visible, FALSE otherwise.
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
 * \brief Create a Pango layout using the current style.
 *
 * This function will connect the appropriate signal handlers to update
 * the layout if the Pango context changes or the font description for the
 * associated style changes.
 *
 * Widgets that plan to keep the created layout should connect to the
 * notify:style signal and destroy the layout when it is emitted.
 *
 * \param self [in] A Widget object.
 *
 * \return A new Pango layout for use with \e self.
 */
G_GNUC_WARN_UNUSED_RESULT
PangoLayout *
joy_bubble_pango_layout_create(JoyBubble *self);

/**
 * \brief Set the default Cairo source for drawing fonts.
 *
 * \param self [in] A Widget object.
 * \param cr [in] A Cairo context.
 */
void
joy_bubble_cairo_set_font_source(JoyBubble *self, cairo_t *cr);

/**
 * \brief Get the widget at the given coordinates.
 *
 * \param self [in] A widget object.
 * \param x [in] The X-coordinate relative to \e self.
 * \param y [in] The Y-coordinate relative to \e self.
 *
 * \return The widget at (\e x, \e y) or NULL if the coordinate does not fall
 *         within \e self.
 */
JoyBubble *
joy_bubble_at(JoyBubble *self, gint x, gint y);

/**
 * \brief Get the widget under the specified device.
 *
 * \param self [in] A widget object.
 * \param device [in] A device object.
 *
 * \return The widget under \e device (may be NULL).
 */
JoyBubble *
joy_bubble_at_device(JoyBubble *self, JoyDevice *device);

/**
 * \brief Move a widget to a new location within its parent container.
 *
 * \param self [in] A widget object.
 * \param x [in] The new X-coordinate (relative to the parent container).
 * \param y [in] The new Y-coordinate (relative to the parent container).
 */
void
joy_bubble_move(JoyBubble *self, gint x, gint y);

/**
 * \brief Translate a widget to a new location within its parent container.
 *
 * \param self [in] A widget object.
 * \param x [in] The amount to move on the X-axis.
 * \param y [in] The amount to move on the Y-axis.
 */
void
joy_bubble_translate(JoyBubble *self, gint x, gint y);

/**
 * \brief Resize a widget.
 *
 * \param self [in] A widget object.
 * \param width [in] The width of \e self.
 * \param height [in] The new height of \e self.
 */
void
joy_bubble_resize(JoyBubble *self, gint width, gint height);

/**
 * \brief Send an expose event to a widget.
 *
 * \param self [in] A widget object.
 * \param rect [in] The exposed area of \e self.
 */
void
joy_bubble_expose(JoyBubble *self, const cairo_rectangle_int_t *rect);

/**
 * \brief Send a key-down event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The keyboard device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 * \param sym [in] The sym for the key that was pressed.
 */
void
joy_bubble_key_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym);

/**
 * \brief Send a key-up even to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The keyboard device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 * \param sym [in] The sym for the key that was pressed.
 */
void
joy_bubble_key_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyKeySym sym);

/**
 * \brief Show a widget.
 *
 * \param self [in] A widget object.
 */
void
joy_bubble_show(JoyBubble *self);

/**
 * \brief Hide a widget.
 *
 * \param self [in] A widget object.
 */
void
joy_bubble_hide(JoyBubble *self);

/**
 * \brief Send a mouse motion event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 */
void
joy_bubble_motion(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

/**
 * \brief Send a mouse button-down event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 * \param button [in] The button that was pressed.
 */
void
joy_bubble_button_down(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button);

/**
 * \brief Send a mouse button-up event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 * \param button [in] The button that was released.
 */
void
joy_bubble_button_up(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseButton button);

/**
 * \brief Send a mouse scroll-wheel event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 * \param direction [in] The direction of the scroll-wheel.
 */
void
joy_bubble_scroll(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y, JoyMouseScroll direction);

/**
 * \brief Send an enter crossing event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 */
void
joy_bubble_enter(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

/**
 * \brief Send an exit crossing event to a widget.
 *
 * \param self [in] A widget object.
 * \param device [in] The mouse device that generated the event.
 * \param timestamp [in] The event timestamp.
 * \param x [in] The X-coordinate of the cursor (relative to \e self).
 * \param y [in] The Y-coordinate of the cursor (relative to \e self).
 */
void
joy_bubble_leave(JoyBubble *self, JoyDevice *device, gulong timestamp,
		gint x, gint y);

/**
 * \brief Draw a widget to the provided cairo context.
 *
 * This function is generally only useful to the toolkit implementation.
 * Calling this function from application code may have undesirable results.
 *
 * \param self [in] A widget object.
 * \param cr [in] The cairo context \e self should draw to.
 */
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
