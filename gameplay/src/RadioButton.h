#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include "Button.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Similar to a checkbox, a radio button can be toggled between two states.
 *
 * However, a radio button can belong to a group, and only one radio button
 * from a group can be selected at one time.
 *
 * The following properties are available for radio buttons:

 @verbatim
    radioButton <RadioButton ID>
    {
         style       = <Style ID>
         alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
         position    = <x, y>
         autoWidth   = <bool>
         autoHeight  = <bool>
         size        = <width, height>
         width       = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
         height      = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
         text        = <string>
         group       = <string>
         iconSize    = <width, height>   // The size to draw the radio button icon, if different from its size in the texture.
         consumeEvents = <bool>          // Whether the radio button propagates input events to the Game's input event handler. Default is true.
    }
 @endverbatim
 */
class RadioButton : public Button
{
    friend class Container;

public:

    /**
     * Create a new radio button control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new radio button.
     * @script{create}
     */
    static RadioButton* create(const char* id, Theme::Style* style);

    /**
     * Get whether this radio button is currently selected.
     *
     * @return Whether this radio button is currently selected.
     */
    bool isSelected() const;

    /**
     * Sets whether this radio button is currently selected.
     */
    void setSelected(bool selected);

    /**
     * Set the size to draw the radio button icon.
     *
     * @param width The width to draw the radio button icon.
     * @param height The height to draw the radio button icon.
     */
    void setImageSize(float width, float height);

    /**
     * Get the size at which the radio button icon will be drawn.
     *
     * @return The size of the radio button icon.
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

    /**
     *
     * @param groupId
     */
    void setGroupId(const char* groupId);

    /**
     * Gets the RadioButton's group ID.
     *
     * @return the RadioButton's group ID.
     */
    const char* getGroupId() const;

protected:

    /**
     * Constructor.
     */
    RadioButton();

    /**
     * Destructor.
     */
    virtual ~RadioButton();

    /**
     * Create a radio button with a given style and properties.
     *
     * @param style The style to apply to this radio button.
     * @param properties The properties to set on this radio button.
     *
     * @return The new radio button.
     */
    static RadioButton* create(Theme::Style* style, Properties* properties);

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
     * @param offset Positioning offset to add to the control's position.
     */
    void update(const Control* container, const Vector2& offset);

    /**
     * Draw the images associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Clear the _selected flag of all radio buttons in the given group.
     *
     * @param groupId The group to clear.
     */
    static void clearSelected(const std::string& groupId);

    /**
     * The RadioButton's group ID.
     */
    std::string _groupId;
    
    /**
     * Whether the RadioButton is currently selected.
     */
    bool _selected;
    
    /**
     * The size at which the RadioButton's icon will be drawn.
     */
    Vector2 _imageSize;

    /**
     * The ThemeImage to use for the RadioButton.
     */ 
    Theme::ThemeImage* _image;

private:

    RadioButton(const RadioButton& copy);
};

}

#endif
