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
    friend class ControlFactory;

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
     * @param properties A properties object containing a definition of the button (optional).
     *
     * @return The new button.
     */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

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
