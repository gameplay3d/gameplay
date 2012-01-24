#include "Base.h"
#include "Button.h"

namespace gameplay
{
    static std::vector<Button*> __buttons;

    Button::Button() : _callback(NULL)
    {
    }

    Button::~Button()
    {
    }

    Button* Button::create(Theme::Style* style, Properties* properties)
    {
        Button* button = new Button();
        button->_style = style;
        button->_id = properties->getId();
        properties->getVector2("position", &button->_position);
        properties->getVector2("size", &button->_size);
        button->_text = properties->getString("text");

        __buttons.push_back(button);

        return button;
    }

    Button* Button::create(const char* id, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        Button* button = new Button();
        button->_id = id;
        button->_position.set(x, y);
        button->_size.set(width, height);

        __buttons.push_back(button);

        return button;
    }

    Button* Button::getButton(const char* id)
    {
        std::vector<Button*>::const_iterator it;
        for (it = __buttons.begin(); it < __buttons.end(); it++)
        {
            Button* b = *it;
            if (strcmp(id, b->getID()) == 0)
            {
                return b;
            }
        }

        return NULL;
    }

    void Button::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            _state = Control::STATE_ACTIVE;
            break;
        case Touch::TOUCH_RELEASE:
            if (_callback &&
                x > 0 && x <= _size.x &&
                y > 0 && y <= _size.y)
            {
                _callback->trigger(this);
            }
            setState(Control::STATE_NORMAL);
            break;
        }
    }
}