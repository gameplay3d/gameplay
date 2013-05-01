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

 @verbatim
    checkBox <checkBoxID>
    {
         style       = <styleID>
         alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
         position    = <x, y>
         autoWidth   = <bool>
         autoHeight  = <bool>
         size        = <width, height>
         width       = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
         height      = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
         text        = <string>
         checked     = <bool>
         iconSize    = <width, height>   // The size to draw the checkbox icon, if different from its size in the texture.
         consumeEvents = <bool>  // Whether the checkbox propagates input events to the Game's input event handler. Default is true.
    }
 @endverbatim
 */
class CheckBox : public Button
{
    friend class Container;

public:

    /**
     * Create a new check box control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new check box.
     * @script{create}
     */
    static CheckBox* create(const char* id, Theme::Style* style);

    /**
     * Gets whether this checkbox is checked.
     *
     * @return Whether this checkbox is checked.
     */
    bool isChecked();

    /**
     * Sets whether the checkbox is checked.
     *
     * @param checked TRUE if the checkbox is checked; FALSE if the checkbox is not checked.
     */
    void setChecked(bool checked);

    /**
     * Set the size to draw the checkbox icon.
     *
     * @param width The width to draw the checkbox icon.
     * @param height The height to draw the checkbox icon.
     */
    void setImageSize(float width, float height);

    /**
     * Get the size at which the checkbox icon will be drawn.
     *
     * @return The size of the checkbox icon.
     */
    const Vector2& getImageSize() const;

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Add a listener to be notified of specific events affecting
     * this control.  Event types can be OR'ed together.
     * E.g. To listen to touch-press and touch-release events,
     * pass <code>Control::Listener::TOUCH | Control::Listener::RELEASE</code>
     * as the second parameter.
     *
     * @param listener The listener to add.
     * @param eventFlags The events to listen for.
     */
    virtual void addListener(Control::Listener* listener, int eventFlags);

protected:

    /**
     * Constructor.
     */
    CheckBox();

    /**
     * Destructor.
     */
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
     * Gamepad callback on gamepad events.
     *
     * @see Control::gamepadEvent
     */
    bool gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Keyboard callback on key events.
     *
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    bool keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param container This control's parent container.
     * @param offset The position offset.
     */
    void update(const Control* container, const Vector2& offset);

    /**
     * Draw the checkbox icon associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The container position this control is relative to.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Whether this checkbox is currently checked.
     */
    bool _checked;

    /**
     * The size to draw the checkbox icon, if different from its size in the texture.
     */
    Vector2 _imageSize;

    /**
     * The Theme::ThemeImage to display for the checkbox.
     */
    Theme::ThemeImage* _image;

private:

    /*
     * Constructor.
     */
    CheckBox(const CheckBox& copy);
};

}

#endif
