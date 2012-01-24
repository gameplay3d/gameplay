#include "Base.h"
#include "Container.h"
#include "Layout.h"
#include "AbsoluteLayout.h"

namespace gameplay
{
    static std::vector<Container*> __containers;

    Container::Container() : _layout(NULL)
    {
    }

    Container::Container(const Container& copy)
    {
    }

    Container::~Container()
    {
    }

    Container* Container::create(const char* id, Layout::Type type)
    {
        Layout* layout;
        switch(type)
        {
        case Layout::LAYOUT_ABSOLUTE:
            layout = AbsoluteLayout::create();
            break;
        case Layout::LAYOUT_FLOW:
            break;
        case Layout::LAYOUT_VERTICAL:
            break;
        }

        Container* container = new Container();
        container->_id = id;
        container->_layout = layout;

        __containers.push_back(container);

        return container;
    }

    Container* Container::getContainer(const char* id)
    {
        std::vector<Container*>::const_iterator it;
        for (it = __containers.begin(); it < __containers.end(); it++)
        {
            Container* c = *it;
            if (strcmp(id, c->getID()) == 0)
            {
                return c;
            }
        }

        return NULL;
    }

    Layout* Container::getLayout()
    {
        return _layout;
    }

    unsigned int Container::addControl(Control* control)
    {
        _controls.push_back(control);
        control->addRef();

        return _controls.size() - 1;
    }

    void Container::insertControl(Control* control, unsigned int index)
    {
        std::vector<Control*>::iterator it = _controls.begin() + index;
        _controls.insert(it, control);
    }

    void Container::removeControl(unsigned int index)
    {
        std::vector<Control*>::iterator it = _controls.begin() + index;
        _controls.erase(it);
    }

    void Container::removeControl(const char* id)
    {
        std::vector<Control*>::iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* c = *it;
            if (strcmp(id, c->getID()) == 0)
            {
                _controls.erase(it);
            }
        }
    }

    void Container::removeControl(Control* control)
    {
        std::vector<Control*>::iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            if (*it == control)
            {
                _controls.erase(it);
            }
        }
    }

    Control* Container::getControl(unsigned int index) const
    {
        std::vector<Control*>::const_iterator it = _controls.begin() + index;
        return *it;
    }

    Control* Container::getControl(const char* id) const
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* c = *it;
            if (strcmp(id, c->getID()) == 0)
            {
                return c;
            }
        }

        return NULL;
    }

    void Container::update()
    {
        // Should probably have sizeChanged() for this.
        //if (isDirty())
        {
            _layout->update(_controls, _size);
        }
    }

    void Container::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            control->drawSprites(spriteBatch, position);
        }

        _dirty = false;
    }

    void Container::drawText(const Vector2& position)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            control->drawText(position);
        }

        _dirty = false;
    }

    bool Container::isDirty()
    {
        if (_dirty)
        {
            return true;
        }
        else
        {
            std::vector<Control*>::const_iterator it;
            for (it = _controls.begin(); it < _controls.end(); it++)
            {
                if ((*it)->isDirty())
                {
                    return true;
                }
            }
        }

        return false;
    }

    void Container::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            const Vector2& size = control->getSize();
            const Vector2& position = control->getPosition();
            
            if (control->getState() == Control::STATE_ACTIVE ||
                (x >= position.x &&
                 x <= position.x + size.x &&
                 y >= position.y &&
                 y <= position.y + size.y))
            {
                // Pass on the event's position relative to the control.
                control->touchEvent(evt, x - position.x, y - position.y, contactIndex);
            }
        }

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            setState(Control::STATE_ACTIVE);
            break;
        case Touch::TOUCH_RELEASE:
            setState(Control::STATE_NORMAL);
            break;
        }
    }
}
