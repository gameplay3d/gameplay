#include "Base.h"
#include "Joystick.h"

#define INVALID_CONTACT_INDEX ((unsigned int)-1)

namespace gameplay
{

Joystick::Joystick() : _contactIndex(INVALID_CONTACT_INDEX), _absolute(true)
{
}

Joystick::Joystick(const Joystick& copy)
{
}

Joystick::~Joystick()
{
}

Joystick* Joystick::create(Theme::Style* style, Properties* properties)
{
    Joystick* joystick = new Joystick();
    joystick->initialize(style, properties);
    joystick->_consumeTouchEvents = false;

    return joystick;
}

void Joystick::initialize(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);

    Control::initialize(style, properties);

    if (!properties->exists("radius"))
    {
        GP_ERROR("Failed to load joystick; required attribute 'radius' is missing.");
        return;
    }
    _radius = properties->getFloat("radius");

    Vector4 v;
    if (properties->getVector4("region", &v))
    {
        _absolute = false;
        _region = _bounds;
        _bounds.x = v.x;
        _bounds.y = v.y;
        _bounds.width = v.z;
        _bounds.height = v.w;
    }
}

void Joystick::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to this control.");
    }

    _consumeTouchEvents = true;

    Control::addListener(listener, eventFlags);
}

bool Joystick::touchEvent(Touch::TouchEvent touchEvent, int x, int y, unsigned int contactIndex)
{
    switch (touchEvent)
    {
        case Touch::TOUCH_PRESS:
        {
            float dx = 0.0f;
            float dy = 0.0f;

            if (_absolute)
            {
                dx = x - _bounds.width * 0.5f;
                dy = _bounds.height * 0.5f - y;
            }
            else
            {
                _region.x = x + _bounds.x - _region.width * 0.5f;
                _region.y = y + _bounds.y - _region.height * 0.5f;
            }

            if ((dx >= -_radius && dx <= _radius) && (dy >= -_radius && dy <= _radius) && 
                _contactIndex == INVALID_CONTACT_INDEX)
            {
                _contactIndex = contactIndex;
                _displacement.set(0.0f, 0.0f);
                
                Vector2 value(0.0f, 0.0f);
                if (_value != value)
                {
                    _value.set(value);
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                _state = ACTIVE;
            }
        }
        case Touch::TOUCH_MOVE:
        {
            if (_contactIndex == contactIndex)
            {
                float dx = x - ((!_absolute) ? _region.x - _bounds.x : 0.0f) - _region.width * 0.5f;
                float dy = -(y - ((!_absolute) ? _region.y - _bounds.y : 0.0f) - _region.height * 0.5f);
                if (((dx * dx) + (dy * dy)) <= (_radius * _radius))
                {
                    GP_ASSERT(_radius);
                    Vector2 value(dx, dy);
                    value.scale(1.0f / _radius);
                    if (_value != value)
                    {
                        _value.set(value);
                        notifyListeners(Control::Listener::VALUE_CHANGED);
                    }
                }
                else
                {
                    Vector2 value(dx, dy);
                    value.normalize();
                    value.scale(_radius);
                    value.normalize();
                    if (_value != value)
                    {
                        _value.set(value);
                        notifyListeners(Control::Listener::VALUE_CHANGED);
                    }
                }

                _displacement.set(dx, dy);
            }
        }
        break;
        case Touch::TOUCH_RELEASE:
        {
            if (_contactIndex == contactIndex)
            {
                // Reset displacement and direction vectors.
                _contactIndex = INVALID_CONTACT_INDEX;
                _displacement.set(0.0f, 0.0f);
                
                Vector2 value(0.0f, 0.0f);
                if (_value != value)
                {
                    _value.set(value);
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                _state = NORMAL;
            }
        }
        break;
    }

    return Control::touchEvent(touchEvent, x, y, contactIndex);
}

void Joystick::update(const Rectangle& clip, const Vector2& offset)
{
    Control::update(clip, offset);
}

void Joystick::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    GP_ASSERT(spriteBatch);
    spriteBatch->begin();

    // If the joystick is not absolute, then only draw if it is active.
    if (_absolute || (!_absolute && _state == ACTIVE))
    {
        if (_absolute)
            _region = _bounds;

        // Draw the outer image.
        Theme::ThemeImage* outer = getImage("outer", _state);
        if (outer)
        {
            // Get the uvs and color and draw.
            const Theme::UVs& uvs = outer->getUVs();
            const Vector4& color = outer->getColor();
            spriteBatch->draw(_region.x, _region.y, _region.width, _region.height, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
        }

        // Draw the inner image.
        Theme::ThemeImage* inner = getImage("inner", _state);
        if (inner)
        {
            Rectangle region = _region;

            // Adjust position to reflect displacement.
            if (((_displacement.x * _displacement.x) + (_displacement.y * _displacement.y)) <= (_radius * _radius))
            {
                region.x += _displacement.x;
                region.y += -_displacement.y;
            }
            else
            {
                // Find the point on the joystick's circular bound where the
                // vector intersects. This is the position of the inner image.
                Vector2 delta = Vector2(_displacement.x, -_displacement.y);
                delta.normalize();
                delta.scale(_radius);
                region.x += delta.x;
                region.y += delta.y;
            }
        
            // Get the uvs and color and draw.
            const Theme::UVs& uvs = inner->getUVs();
            const Vector4& color = inner->getColor();
            spriteBatch->draw(region.x, region.y, _region.width, _region.height, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
        }
    }
    spriteBatch->end();
}

}