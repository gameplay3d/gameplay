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
    button->_id = id ? id : "";
    button->initialize("Button", style, NULL);
    return button;
}

Control* Button::create(Theme::Style* style, Properties* properties)
{
    Button* button = new Button();
	button->initialize("Button", style, properties);
    return button;
}

void Button::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Label::initialize(typeName, style, properties);

    if (properties)
    {
        // Different types of data bindings can be named differently in a button namespace.
        // Gamepad button mappings have the name "mapping" and correspond to Gamepad::ButtonMapping enums.
        const char* mapping = properties->getString("mapping");
        if (mapping)
        {
            _dataBinding = Gamepad::getButtonMappingFromString(mapping);
        }
    }
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
