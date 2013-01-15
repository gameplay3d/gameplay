#include "Base.h"
#include "GamepadButton.h"

namespace gameplay
{

GamepadButton::GamepadButton()
{
}

GamepadButton::~GamepadButton()
{
}

GamepadButton* GamepadButton::create(const char* id, Theme::Style* style)
{
    GamepadButton* button = new GamepadButton();

    button->_id = id;
    button->_style = style;

    return button;
}

GamepadButton* GamepadButton::create(Theme::Style* style, Properties* properties)
{
    GamepadButton* button = new GamepadButton();
    button->initialize(style, properties);

    button->_mapping = Gamepad::getButtonMappingFromString(properties->getString("mapping"));

    return button;
}

const char* GamepadButton::getType() const
{
    return "gamepadButton";
}

const Gamepad::ButtonMapping GamepadButton::getMapping() const
{
    return _mapping;
}

}