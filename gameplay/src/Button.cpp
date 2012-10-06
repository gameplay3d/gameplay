#include "Base.h"
#include "Button.h"

namespace gameplay
{

Button::Button()
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

    return button;
}

bool Button::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!isEnabled())
        return false;

    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (_contactIndex == INVALID_CONTACT_INDEX)
        {
            if (x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                _contactIndex = (int) contactIndex;

                setState(Control::ACTIVE);

                notifyListeners(Listener::PRESS);

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
            notifyListeners(Listener::RELEASE);
            if (!_parent->isScrolling() &&
                x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                setState(Control::FOCUS);

                notifyListeners(Listener::CLICK);
            }
            else
            {
                setState(Control::NORMAL);
            }
            return _consumeInputEvents;
        }
        break;
    case Touch::TOUCH_MOVE:
        if (_contactIndex == (int) contactIndex)
            return _consumeInputEvents;
        break;
    }

    return false;
}

const char* Button::getType() const
{
    return "button";
}

}
