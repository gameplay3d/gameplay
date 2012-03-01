#include "Base.h"
#include "Button.h"

namespace gameplay
{
    Button::Button() : _callback(NULL)
    {
    }

    Button::~Button()
    {
        SAFE_DELETE(_callback);
    }

    Button* Button::create(Theme::Style* style, Properties* properties)
    {
        Button* button = new Button();
        button->init(style, properties);

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
            return _consumeTouchEvents;
        case Touch::TOUCH_RELEASE:
            if (_callback &&
                x > 0 && x <= _bounds.width &&
                y > 0 && y <= _bounds.height)
            {
                // Button-clicked callback.
                _callback->trigger(this);
                setState(Control::NORMAL);
                _dirty = true;
                return _consumeTouchEvents;
            }
            _dirty = true;
            setState(Control::NORMAL);
            break;
        }

        return _consumeTouchEvents;
    }
}