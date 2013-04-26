#ifndef BUTTON_H_
#define BUTTON_H_

#include "Container.h"
#include "Label.h"
#include "Touch.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Defines a button UI control. This is essentially a label that can have a callback method set on it.
 *
 * The following properties are available for buttons:

 @verbatim
    button <buttonID>
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
         consumeEvents = <bool>  // Whether the button propagates input events to the Game's input event handler. Default is true.
    }
 @endverbatim
 */
class Button : public Label
{
    friend class Container;
    friend class Gamepad;

public:

    /**
     * Create a new button control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new button.
     * @script{create}
     */
    static Button* create(const char* id, Theme::Style* style);

protected:

    /**
     * Constructor.
     */
    Button();

    /**
     * Destructor.
     */
    virtual ~Button();

    /**
     * Create a button with a given style and properties.
     *
     * @param style The style to apply to this button.
     * @param properties The properties to set on this button.
     *
     * @return The new button.
     */
    static Button* create(Theme::Style* style, Properties* properties);

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
    virtual bool gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Keyboard callback on key events.
     *
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual bool keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Gets the data binding index for this control.
     *
     * @return The data binding index for control. 
     */
    const unsigned int getDataBinding() const;

    /**
     * Sets the data binding provider for this control.
     *
     * @param dataBinding The data binding index for control. 
     */
    void setDataBinding(unsigned int dataBinding);

private:

    /**
     * Constructor.
     */
    Button(const Button& copy);

    unsigned int _dataBinding;

};

}

#endif
