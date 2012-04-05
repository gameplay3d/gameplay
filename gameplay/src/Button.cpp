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

    Button* Button::create(Theme::Style* style, Properties* properties)
    {
        Button* button = new Button();
        button->initialize(style, properties);

        return button;
    }

    bool Button::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        if (!isEnabled())
        {
            return false;
        }

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            _state = Control::ACTIVE;
            _dirty = true;
            break;
        case Touch::TOUCH_RELEASE:
            _dirty = true;
            setState(Control::NORMAL);
            break;
        }

        return Control::touchEvent(evt, x, y, contactIndex);
    }
}