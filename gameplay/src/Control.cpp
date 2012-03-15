#include "Base.h"
#include "Control.h"

namespace gameplay
{
    Control::Control()
        : _id(""), _state(Control::NORMAL), _position(Vector2::zero()), _size(Vector2::zero()), _bounds(Rectangle::empty()), _clip(Rectangle::empty()),
          _autoWidth(true), _autoHeight(true), _dirty(true), _consumeTouchEvents(true), _listeners(NULL)
    {
    }

    Control::Control(const Control& copy)
    {
    }

    Control::~Control()
    {
        if (_listeners)
        {
            for (ListenerMap::const_iterator itr = _listeners->begin(); itr != _listeners->end(); itr++)
            {
                std::list<Listener*>* list = itr->second;
                SAFE_DELETE(list);
            }
            SAFE_DELETE(_listeners);
        }
    }

    void Control::init(Theme::Style* style, Properties* properties)
    {
        _style = style;

        properties->getVector2("position", &_position);
        properties->getVector2("size", &_size);

        _state = Control::getStateFromString(properties->getString("state"));

        const char* id = properties->getId();
        if (id)
        {
            _id = id;
        }
    }

    const char* Control::getID() const
    {
        return _id.c_str();
    }

    void Control::setPosition(float x, float y)
    {
        _position.set(x, y);
    }

    const Vector2& Control::getPosition() const
    {
        return _position;
    }

    void Control::setSize(float width, float height)
    {
        _size.set(width, height);
    }

    const Vector2& Control::getSize() const
    {
        return _size;
    }

    const Rectangle& Control::getBounds() const
    {
        return _bounds;
    }

    const Rectangle& Control::getClip() const
    {
        return _clip;
    }

    void Control::setAutoSize(bool width, bool height)
    {
        _autoWidth = width;
        _autoHeight = height;
    }

    void Control::setStyle(Theme::Style* style)
    {
        _style = style;
    }

    Theme::Style* Control::getStyle() const
    {
        return _style;
    }

    void Control::setState(State state)
    {
        _state = state;
    }

    Control::State Control::getState()
    {
        return _state;
    }

    void Control::disable()
    {
        _state = DISABLED;
    }

    void Control::enable()
    {
        _state = NORMAL;
    }

    bool Control::isEnabled()
    {
        return _state != DISABLED;
    }

    Theme::Style::OverlayType Control::getOverlayType() const
    {
        switch (_state)
        {
        case Control::NORMAL:
            return Theme::Style::OVERLAY_NORMAL;
        case Control::FOCUS:
            return Theme::Style::OVERLAY_FOCUS;
        case Control::ACTIVE:
            return Theme::Style::OVERLAY_ACTIVE;
        case Control::DISABLED:
            return Theme::Style::OVERLAY_DISABLED;
        default:
            return Theme::Style::OVERLAY_NORMAL;
        }
    }

    void Control::setConsumeTouchEvents(bool consume)
    {
        _consumeTouchEvents = consume;
    }
    
    bool Control::getConsumeTouchEvents()
    {
        return _consumeTouchEvents;
    }

    void Control::addListener(Control::Listener* listener, int eventFlags)
    {
        if ((eventFlags & Listener::PRESS) == Listener::PRESS)
        {
            addSpecificListener(listener, Listener::PRESS);
        }

        if ((eventFlags & Listener::RELEASE) == Listener::RELEASE)
        {
            addSpecificListener(listener, Listener::RELEASE);
        }

        if ((eventFlags & Listener::CLICK) == Listener::CLICK)
        {
            addSpecificListener(listener, Listener::CLICK);
        }

        if ((eventFlags & Listener::VALUE_CHANGED) == Listener::VALUE_CHANGED)
        {
            addSpecificListener(listener, Listener::VALUE_CHANGED);
        }

        if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
        {
            addSpecificListener(listener, Listener::TEXT_CHANGED);
        }
    }

    void Control::addSpecificListener(Control::Listener* listener, Listener::EventType eventType)
    {
        if (!_listeners)
        {
            _listeners = new std::map<Listener::EventType, std::list<Listener*>*>();
        }

        ListenerMap::const_iterator itr = _listeners->find(eventType);
        if (itr == _listeners->end())
        {
            _listeners->insert(std::make_pair(eventType, new std::list<Listener*>()));
            itr = _listeners->find(eventType);
        }

        std::list<Listener*>* listenerList = itr->second;
        listenerList->push_back(listener);
    }

    bool Control::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
    {
        if (!isEnabled())
        {
            return false;
        }

        switch (evt)
        {
        case Touch::TOUCH_PRESS:
            alertListeners(Listener::PRESS);
            break;
            
        case Touch::TOUCH_RELEASE:
            // Always trigger Listener::RELEASE
            alertListeners(Listener::RELEASE);

            // Only trigger Listener::CLICK if both PRESS and RELEASE took place within the control's bounds.
            if (x > 0 && x <= _bounds.width &&
                y > 0 && y <= _bounds.height)
            {
                alertListeners(Listener::CLICK);
            }
            break;
        }

        return _consumeTouchEvents;
    }

    void Control::keyEvent(Keyboard::KeyEvent evt, int key)
    {
    }

    void Control::alertListeners(Listener::EventType eventType)
    {
        if (_listeners)
        {
            ListenerMap::const_iterator itr = _listeners->find(eventType);
            if (itr != _listeners->end())
            {
                std::list<Listener*>* listenerList = itr->second;
                for (std::list<Listener*>::iterator listenerItr = listenerList->begin(); listenerItr != listenerList->end(); listenerItr++)
                {
                    (*listenerItr)->controlEvent(this, eventType);
                }
            }
        }
    }

    void Control::update(const Rectangle& clip)
    {
        // Calculate the bounds.
        float x = clip.x + _position.x;
        float y = clip.y + _position.y;
        float width = _size.x;
        float height = _size.y;

        float clipX2 = clip.x + clip.width;
        float x2 = x + width;
        if (x2 > clipX2)
        {
            width = clipX2 - x;
        }

        float clipY2 = clip.y + clip.height;
        float y2 = y + height;
        if (y2 > clipY2)
        {
            height = clipY2 - y;
        }

        _bounds.set(_position.x, _position.y, width, height);

        // Calculate the clipping viewport.
        Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
        Theme::Border border;
        Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
        if (containerRegion)
        {
            border = overlay->getContainerRegion()->getBorder();
        }
        Theme::Padding padding = _style->getPadding();

        x +=  border.left + padding.left;
        y +=  border.top + padding.top;
        width = _size.x - border.left - padding.left - border.right - padding.right;
        height = _size.y - border.top - padding.top - border.bottom - padding.bottom;

        _textBounds.set(x, y, width, height);

        clipX2 = clip.x + clip.width;
        x2 = x + width;
        if (x2 > clipX2)
        {
            width = clipX2 - x;
        }

        clipY2 = clip.y + clip.height;
        y2 = y + height;
        if (y2 > clipY2)
        {
            height = clipY2 - y;
        }

        if (x < clip.x)
        {
            x = clip.x;
        }

        if (y < clip.y)
        {
            y = clip.y;
        }

        _clip.set(x, y, width, height);
    }

    void Control::drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip)
    {
        Vector2 pos(clip.x + _position.x, clip.y + _position.y);

        // Get the border and background images for this control's current state.
        Theme::ContainerRegion* containerRegion = _style->getOverlay(getOverlayType())->getContainerRegion();
        if (containerRegion)
        {
            // Get the UVs.
            Theme::UVs topLeft, top, topRight, left, center, right, bottomLeft, bottom, bottomRight;
            topLeft = containerRegion->getUVs(Theme::ContainerRegion::TOP_LEFT);
            top = containerRegion->getUVs(Theme::ContainerRegion::TOP);
            topRight = containerRegion->getUVs(Theme::ContainerRegion::TOP_RIGHT);
            left = containerRegion->getUVs(Theme::ContainerRegion::LEFT);
            center = containerRegion->getUVs(Theme::ContainerRegion::CENTER);
            right = containerRegion->getUVs(Theme::ContainerRegion::RIGHT);
            bottomLeft = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM_LEFT);
            bottom = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM);
            bottomRight = containerRegion->getUVs(Theme::ContainerRegion::BOTTOM_RIGHT);

            // Calculate screen-space positions.
            Theme::Border border = containerRegion->getBorder();
            Theme::Padding padding = _style->getPadding();
            Vector4 borderColor = containerRegion->getColor();

            float midWidth = _size.x - border.left - border.right;
            float midHeight = _size.y - border.top - border.bottom;
            float midX = pos.x + border.left;
            float midY = pos.y + border.top;
            float rightX = pos.x + _size.x - border.right;
            float bottomY = pos.y + _size.y - border.bottom;

            // Draw themed border sprites.
            if (!border.left && !border.right && !border.top && !border.bottom)
            {
                // No border, just draw the image.
                spriteBatch->draw(pos.x, pos.y, _size.x, _size.y, center.u1, center.v1, center.u2, center.v2, borderColor, clip);
            }
            else
            {
                if (border.left && border.top)
                    spriteBatch->draw(pos.x, pos.y, border.left, border.top, topLeft.u1, topLeft.v1, topLeft.u2, topLeft.v2, borderColor, clip);
                if (border.top)
                    spriteBatch->draw(pos.x + border.left, pos.y, midWidth, border.top, top.u1, top.v1, top.u2, top.v2, borderColor, clip);
                if (border.right && border.top)
                    spriteBatch->draw(rightX, pos.y, border.right, border.top, topRight.u1, topRight.v1, topRight.u2, topRight.v2, borderColor, clip);
                if (border.left)
                    spriteBatch->draw(pos.x, midY, border.left, midHeight, left.u1, left.v1, left.u2, left.v2, borderColor, clip);
                if (border.left && border.right && border.top && border.bottom)
                    spriteBatch->draw(pos.x + border.left, pos.y + border.top, _size.x - border.left - border.right, _size.y - border.top - border.bottom,
                        center.u1, center.v1, center.u2, center.v2, borderColor, clip);
                if (border.right)
                    spriteBatch->draw(rightX, midY, border.right, midHeight, right.u1, right.v1, right.u2, right.v2, borderColor, clip);
                if (border.bottom && border.left)
                    spriteBatch->draw(pos.x, bottomY, border.left, border.bottom, bottomLeft.u1, bottomLeft.v1, bottomLeft.u2, bottomLeft.v2, borderColor, clip);
                if (border.bottom)
                    spriteBatch->draw(midX, bottomY, midWidth, border.bottom, bottom.u1, bottom.v1, bottom.u2, bottom.v2, borderColor, clip);
                if (border.bottom && border.right)
                    spriteBatch->draw(rightX, bottomY, border.right, border.bottom, bottomRight.u1, bottomRight.v1, bottomRight.u2, bottomRight.v2, borderColor, clip);
            }
        }
    }

    void Control::drawSprites(SpriteBatch* spriteBatch, const Rectangle& position)
    {
    }

    void Control::drawText(const Rectangle& position)
    {
    }

    bool Control::isDirty()
    {
        return _dirty;
    }

    bool Control::isContainer()
    {
        return false;
    }

    Control::State Control::getStateFromString(const char* state)
    {
        if (!state)
        {
            return NORMAL;
        }

        if (strcmp(state, "NORMAL") == 0)
        {
            return NORMAL;
        }
        else if (strcmp(state, "ACTIVE") == 0)
        {
            return ACTIVE;
        }
        else if (strcmp(state, "FOCUS") == 0)
        {
            return FOCUS;
        }
        else if (strcmp(state, "DISABLED") == 0)
        {
            return DISABLED;
        }

        return NORMAL;
    }
}
