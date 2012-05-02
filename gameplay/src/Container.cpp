#include "Base.h"
#include "Container.h"
#include "Layout.h"
#include "AbsoluteLayout.h"
#include "FlowLayout.h"
#include "VerticalLayout.h"
#include "ScrollLayout.h"
#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Slider.h"
#include "TextBox.h"
#include "Game.h"

namespace gameplay
{
    Container::Container() : _layout(NULL)
    {
    }

    Container::Container(const Container& copy)
    {
    }

    Container::~Container()
    {
        std::vector<Control*>::iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            SAFE_RELEASE((*it));
        }

        SAFE_RELEASE(_layout);
    }

    Container* Container::create(Layout::Type type)
    {
        Layout* layout = NULL;
        switch (type)
        {
        case Layout::LAYOUT_ABSOLUTE:
            layout = AbsoluteLayout::create();
            break;
        case Layout::LAYOUT_FLOW:
            layout = FlowLayout::create();
            break;
        case Layout::LAYOUT_VERTICAL:
            layout = VerticalLayout::create();
            break;
        case Layout::LAYOUT_SCROLL:
            layout = ScrollLayout::create();
            break;
        }

        Container* container = new Container();
        container->_layout = layout;

        return container;
    }

    Container* Container::create(Theme::Style* style, Properties* properties, Theme* theme)
    {
        const char* layoutString = properties->getString("layout");
        Container* container = Container::create(getLayoutType(layoutString));
        container->initialize(style, properties);
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
            assert(controlStyle);

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
            else if (controlName == "TEXTBOX")
            {
                control = TextBox::create(controlStyle, controlSpace);
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

    Layout* Container::getLayout()
    {
        return _layout;
    }

    unsigned int Container::addControl(Control* control)
    {
        _controls.push_back(control);

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
            else if (c->isContainer())
            {
                Control* cc = ((Container*)c)->getControl(id);
                if (cc)
                {
                    return cc;
                }
            }
        }

        return NULL;
    }

    std::vector<Control*> Container::getControls() const
    {
        return _controls;
    }

    Animation* Container::getAnimation(const char* id) const
    {
        std::vector<Control*>::const_iterator itr = _controls.begin();
        std::vector<Control*>::const_iterator end = _controls.end();
        
        Control* control = NULL;
        for (; itr != end; itr++)
        {
            control = *itr;
            Animation* animation = control->getAnimation(id);
            if (animation)
                return animation;

            if (control->isContainer())
            {
                animation = ((Container*)control)->getAnimation(id);
                if (animation)
                    return animation;
            }
        }

        return NULL;
    }

    void Container::update(const Rectangle& clip, const Vector2& offset)
    {
        // Update this container's viewport.
        Control::update(clip, offset);

        _layout->update(this);
    }
    /*
    void Container::drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip, const Vector2& offset)
    {
        // First draw our own border.
        Control::drawBorder(spriteBatch, clip);

        // Now call drawBorder on all controls within this container.
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            if (control->isDirty())
            {
                control->drawBorder(spriteBatch, _clip);
            }
        }
    }

    void Container::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            if (control->isDirty())
                control->drawImages(spriteBatch, _clip);
        }

        _dirty = false;
    }

    void Container::drawText(const Rectangle& clip)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            if (control->isDirty())
                control->drawText(_clip);
        }

        _dirty = false;
    }*/

    void Container::draw(SpriteBatch* spriteBatch, const Rectangle& clip)
    {
        GL_ASSERT( glEnable(GL_SCISSOR_TEST) );
        GL_ASSERT( glClearColor(0, 0, 0, 1) );
        GL_ASSERT( glScissor(_absoluteBounds.x, Game::getInstance()->getHeight() - _absoluteBounds.y - _absoluteBounds.height,
            _absoluteBounds.width, _absoluteBounds.height) );
        GL_ASSERT( glClear(GL_COLOR_BUFFER_BIT) );
        GL_ASSERT( glDisable(GL_SCISSOR_TEST) );

        Control::drawBorder(spriteBatch, clip);

        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            //if (control->isDirty())
            {
                control->draw(spriteBatch, _clip);
            }
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

    bool Container::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        if (!isEnabled())
        {
            return false;
        }

        bool eventConsumed = false;

        const Theme::Border& border = getBorder(_state);
        const Theme::Padding& padding = getPadding();
        float xPos = border.left + padding.left;
        float yPos = border.top + padding.top;

        Vector2* offset = NULL;
        if (_layout->getType() == Layout::LAYOUT_SCROLL)
        {
            offset = &((ScrollLayout*)_layout)->_scrollPosition;
        }

        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            if (!control->isEnabled())
            {
                continue;
            }

            const Rectangle& bounds = control->getBounds();
            float boundsX = bounds.x;
            float boundsY = bounds.y;
            if (offset)
            {
                boundsX += offset->x;
                boundsY += offset->y;
            }

            if (control->getState() != Control::NORMAL ||
                (evt == Touch::TOUCH_PRESS &&
                 x >= xPos + boundsX &&
                 x <= xPos + boundsX + bounds.width &&
                 y >= yPos + boundsY &&
                 y <= yPos + boundsY + bounds.height))
            {
                // Pass on the event's clip relative to the control.
                eventConsumed |= control->touchEvent(evt, x - xPos - boundsX, y - yPos - boundsY, contactIndex);
            }
        }

        if (!isEnabled())
        {
            return (_consumeTouchEvents | eventConsumed);
        }

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            setState(Control::FOCUS);
            break;
        case Touch::TOUCH_RELEASE:
            setState(Control::NORMAL);
            break;
        }

        if (!eventConsumed)
        {
            // Pass the event on to the layout.
            if (_layout->touchEvent(evt, x - xPos, y - yPos, contactIndex))
            {
                _dirty = true;
            }
        }

        return (_consumeTouchEvents | eventConsumed);
    }

    void Container::keyEvent(Keyboard::KeyEvent evt, int key)
    {
        std::vector<Control*>::const_iterator it;
        for (it = _controls.begin(); it < _controls.end(); it++)
        {
            Control* control = *it;
            if (!control->isEnabled())
            {
                continue;
            }

            if (control->isContainer() || control->getState() == Control::FOCUS)
            {
                control->keyEvent(evt, key);
            }
        }
    }

    bool Container::isContainer()
    {
        return true;
    }

    Layout::Type Container::getLayoutType(const char* layoutString)
    {
        if (!layoutString)
        {
            return Layout::LAYOUT_ABSOLUTE;
        }

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
        else if (layoutName == "LAYOUT_SCROLL")
        {
            return Layout::LAYOUT_SCROLL;
        }
        else
        {
            // Default.
            return Layout::LAYOUT_ABSOLUTE;
        }
    }
}
