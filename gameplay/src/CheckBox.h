#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "Theme.h"
#include "Properties.h"
#include "Touch.h"
#include "Button.h"

namespace gameplay
{

/**
 * Defines a checkbox UI control.  This is a button that toggles between two icons when clicked.
 *
 * The following properties are available for checkboxes:
 *
 * checkBox <CheckBox ID>
 * {
 *      style       = <Style ID>
 *      position    = <x, y>
 *      size        = <width, height>
 *      text        = <string>
 *      checked     = <bool>
 *      iconSize    = <width, height>   // The size to draw the checkbox icon, if different from its size in the texture.
 * }
 */
class CheckBox : public Button
{
    friend class Container;

public:
    /**
     * Gets whether this checkbox is checked.
     *
     * @return Whether this checkbox is checked.
     */
    bool isChecked();

    /**
     * Set the size to draw the checkbox icon.
     *
     * @param width The width to draw the checkbox icon.
     * @param height The height to draw the checkbox icon.
     */
    void setIconSize(float width, float height);

    /**
     * Get the size at which the checkbox icon will be drawn.
     *
     * @return The size of the checkbox icon.
     */
    const Vector2& getIconSize() const;

protected:
    CheckBox();
    ~CheckBox();

    /**
     * Create a checkbox with a given style and properties.
     *
     * @param style The style to apply to this checkbox.
     * @param properties The properties to set on this checkbox.
     *
     * @return The new checkbox.
     */
    static CheckBox* create(Theme::Style* style, Properties* properties);

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by the control.
     *
     * @see Touch::TouchEvent
     */
    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param position The control's position within its container.
     */
    void update(const Rectangle& clip);

    /**
     * Draw the checkbox icon associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param position The container position this control is relative to.
     */
    void drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw this control's text.
     *
     * @param position The container position this control is relative to.
     */
    void drawText(const Rectangle& clip);

    bool _checked;      // Whether this checkbox is currently checked.
    Vector2 _iconSize;  // The size to draw the checkbox icon, if different from its size in the texture.

private:
    CheckBox(const CheckBox& copy);
};

}

#endif