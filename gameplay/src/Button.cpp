#include "Base.h"
#include "Button.h"
#include "Gamepad.h"

namespace gameplay
{

Button::Button() : _dataBinding(0)
{
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

Button* Button::create(Theme::Style* style, Properties* properties)
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

bool Button::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (_contactIndex == INVALID_CONTACT_INDEX)
        {
            if (x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                _contactIndex = (int) contactIndex;
                notifyListeners(Control::Listener::PRESS);
                setState(Control::ACTIVE);
                return _consumeInputEvents;
            }
            else
            {
                setState(Control::NORMAL);
            }
        }
        break;

    case Touch::TOUCH_RELEASE:
        if (_contactIndex == (int) contactIndex)
        {
            _contactIndex = INVALID_CONTACT_INDEX;
            notifyListeners(Control::Listener::RELEASE);
            if (!_parent->isScrolling() &&
                x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                notifyListeners(Control::Listener::CLICK);
                setState(Control::FOCUS);
            }
            else
            {
                setState(Control::NORMAL);
            }
            return _consumeInputEvents;
        }
        break;
    case Touch::TOUCH_MOVE:
        return Control::touchEvent(evt, x, y, contactIndex);
    }

    return false;
}

bool Button::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    switch (evt)
    {
    case Gamepad::BUTTON_EVENT:
        if (_state == Control::FOCUS)
        {
            if (gamepad->isButtonDown(Gamepad::BUTTON_A) ||
                gamepad->isButtonDown(Gamepad::BUTTON_X))
            {
                notifyListeners(Control::Listener::PRESS);
                setState(Control::ACTIVE);
                return _consumeInputEvents;
            }
        }
        else if (_state == Control::ACTIVE)
        {
            if (!gamepad->isButtonDown(Gamepad::BUTTON_A) &&
                !gamepad->isButtonDown(Gamepad::BUTTON_X))
            {
                notifyListeners(Control::Listener::RELEASE);
                notifyListeners(Control::Listener::CLICK);
                setState(Control::FOCUS);
                return _consumeInputEvents;
            }
        }
        break;
    default:
        break;
    }

    return false;
}

bool Button::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS && key == Keyboard::KEY_RETURN)
    {
        notifyListeners(Control::Listener::PRESS);
        setState(Control::ACTIVE);
        return _consumeInputEvents;
    }
    else if (_state == ACTIVE && evt == Keyboard::KEY_RELEASE && key == Keyboard::KEY_RETURN)
    {
        notifyListeners(Control::Listener::RELEASE);
        notifyListeners(Control::Listener::CLICK);
        setState(Control::FOCUS);
        return _consumeInputEvents;
    }

    return false;
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
