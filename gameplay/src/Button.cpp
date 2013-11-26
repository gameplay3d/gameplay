#include "Base.h"
#include "Button.h"
#include "Gamepad.h"

namespace gameplay
{

Button::Button() : _dataBinding(0)
{
    _canFocus = true;
}

Button::~Button()
{
}

Button* Button::create(const char* id, Theme::Style* style)
{
    Button* button = new Button();

    button->_id = id;
    button->_style = style;

    return button;
}

Control* Button::create(Theme::Style* style, Properties* properties)
{
    Button* button = new Button();
    button->initialize(style, properties);

    // Different types of data bindings can be named differently in a button namespace.
    // Gamepad button mappings have the name "mapping" and correspond to Gamepad::ButtonMapping enums.
    const char* mapping = properties->getString("mapping");
    if (mapping)
    {
        button->_dataBinding = Gamepad::getButtonMappingFromString(mapping);
    }

    return button;
}

const char* Button::getType() const
{
    return "button";
}

const unsigned int Button::getDataBinding() const
{
    return _dataBinding;
}

void Button::setDataBinding(unsigned int dataBinding)
{
    _dataBinding = dataBinding;
}

}
