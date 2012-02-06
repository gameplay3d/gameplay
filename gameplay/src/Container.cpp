#include "Base.h"
#include "Container.h"
#include "Layout.h"
#include "AbsoluteLayout.h"
#include "VerticalLayout.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Slider.h"

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
            layout = VerticalLayout::create();
            break;
        }

        Container* container = new Container();
        container->_id = id;
        container->_layout = layout;

        __containers.push_back(container);

        return container;
    }

    Container* Container::create(Theme::Style* style, Properties* properties, Theme* theme)
    {
        const char* id = properties->getId();
        const char* layoutString = properties->getString("layout");
        Container* container = Container::create(id, getLayoutType(layoutString));

        container->_style = style;
        properties->getVector2("position", &container->_position);
        properties->getVector2("size", &container->_size);

        container->addControls(theme, properties);

        return container;
    }

    void Container::addControls(Theme* theme, Properties* properties)
    {
        // Add all the controls to this container.
        Properties* controlSpace = properties->getNextNamespace();
        while (controlSpace != NULL)
        {
            Control* control = NULL;

            const char* controlStyleName = controlSpace->getString("style");
            Theme::Style* controlStyle = NULL;
            if (controlStyleName)
            {
                 controlStyle = theme->getStyle(controlStyleName);
            }

            std::string controlName(controlSpace->getNamespace());
            std::transform(controlName.begin(), controlName.end(), controlName.begin(), (int(*)(int))toupper);
            if (controlName == "LABEL")
            {
                control = Label::create(controlStyle, controlSpace);
            }
            else if (controlName == "BUTTON")
            {
                control = Button::create(controlStyle, controlSpace);
            }
            else if (controlName == "CHECKBOX")
            {
                control = CheckBox::create(controlStyle, controlSpace);
            }
            else if (controlName == "RADIOBUTTON")
            {
                control = RadioButton::create(controlStyle, controlSpace);
            }
            else if (controlName == "CONTAINER")
            {
                control = Container::create(controlStyle, controlSpace, theme);
            }
            else if (controlName == "SLIDER")
            {
                control = Slider::create(controlStyle, controlSpace);
            }

            // Add the new control to the form.
            if (control)
            {
                addControl(control);
            }

            // Get the next control.
            controlSpace = properties->getNextNamespace();
        }
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
            // Call update() on nested Containers.
            std::vector<Control*>::const_iterator it;
            for (it = _controls.begin(); it < _controls.end(); it++)
            {
                // Can't do this without enabling run-time type information!
                //Container* container = dynamic_cast<Container*>(*it);

                Control* control = *it;
                if (control->isContainer())
                {
                    Container* container = static_cast<Container*>(control);
                    container->update();
                }
            }
            
            _layout->update(_controls, _size, _style);
        }
    }

    void Container::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
    {
        Vector2 pos(position.x + _position.x, position.y + _position.y);
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            control->drawSprites(spriteBatch, pos);
        }

        _dirty = false;
    }

    void Container::drawText(const Vector2& position)
    {
        Vector2 pos(position.x + _position.x, position.y + _position.y);
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            control->drawText(pos);
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

    Layout::Type Container::getLayoutType(const char* layoutString)
    {
        std::string layoutName(layoutString);
        std::transform(layoutName.begin(), layoutName.end(), layoutName.begin(), (int(*)(int))toupper);
        if (layoutName == "LAYOUT_ABSOLUTE")
        {
            return Layout::LAYOUT_ABSOLUTE;
        }
        else if (layoutName == "LAYOUT_VERTICAL")
        {
            return Layout::LAYOUT_VERTICAL;
        }
        else if (layoutName == "LAYOUT_FLOW")
        {
            return Layout::LAYOUT_FLOW;
        }
        else
        {
            // Default.
            return Layout::LAYOUT_ABSOLUTE;
        }
    }

    bool Container::isContainer()
    {
        return true;
    }
}
