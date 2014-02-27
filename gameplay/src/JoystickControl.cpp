#include "Base.h"
#include "JoystickControl.h"

namespace gameplay
{

JoystickControl::JoystickControl() : _radius(1.0f), _relative(true), _innerSize(NULL), _outerSize(NULL), _index(0)
{
}

JoystickControl::~JoystickControl()
{
    if (_innerSize)
        SAFE_DELETE(_innerSize);
    if (_outerSize)
        SAFE_DELETE(_outerSize);
}

JoystickControl* JoystickControl::create(const char* id, Theme::Style* style)
{
    JoystickControl* joystickControl = new JoystickControl();
    joystickControl->_id = id ? id : "";
    joystickControl->initialize("Joystick", style, NULL);
    return joystickControl;
}

Control* JoystickControl::create(Theme::Style* style, Properties* properties)
{
    JoystickControl* joystickControl = new JoystickControl();
    joystickControl->initialize("Joystick", style, properties);
    return joystickControl;
}

const Vector2& JoystickControl::getValue() const
{
    return _value;
}

void JoystickControl::setInnerRegionSize(const Vector2& size)
{
    if (_innerSize)
        _innerSize->set(size);
}

const Vector2& JoystickControl::getInnerRegionSize() const
{
    if (_innerSize)
        return *_innerSize;
    else
        return Vector2::zero();
}

void JoystickControl::setOuterRegionSize(const Vector2& size)
{
    if (_outerSize)
        _outerSize->set(size);
}

const Vector2& JoystickControl::getOuterRegionSize() const
{
    if (_outerSize)
        return *_outerSize;
    else
        return Vector2::zero();
}

void JoystickControl::setRelative(bool relative)
{
    _relative = relative;
}

bool JoystickControl::isRelative() const
{
    return _relative;
}

unsigned int JoystickControl::getIndex() const
{
    return _index;
}


void JoystickControl::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Control::initialize(typeName, style, properties);

	if (!properties)
	{
		GP_WARN("JoystickControl creation without properties object is unsupported.");
		return;
	}

	Control::State state = getState();

	if (!properties->exists("radius"))
	{
		GP_WARN("JoystickControl: required attribute 'radius' is missing.");
	}
	else
	{
		_radius = properties->getFloat("radius");
		if (_radius < 1.0f)
			_radius = 1.0f;
	}

	if (properties->exists("relative"))
	{
		setRelative(properties->getBool("relative"));
	}
	else
	{
		setRelative(false);
	}

	Theme::ThemeImage* inner = getImage("inner", state);
	if (inner)
	{
		_innerSize = new Vector2();
		Vector2 innerSize;
		if (properties->getVector2("innerRegion", &innerSize))
		{
			_innerSize->set(innerSize.x, innerSize.y);
		}
		else
		{
			const Rectangle& rect = inner->getRegion();
			_innerSize->set(rect.width, rect.height);
		}
	}

	Theme::ThemeImage* outer = getImage("outer", state);
	if (outer)
	{
		_outerSize = new Vector2();
		Vector2 outerSize;
		if (properties->getVector2("outerRegion", &outerSize))
		{
			_outerSize->set(outerSize.x, outerSize.y);
		}
		else
		{
			const Rectangle& rect = outer->getRegion();
			_outerSize->set(rect.width, rect.height);
		}
		_screenRegion.width = _outerSize->x;
		_screenRegion.height = _outerSize->y;
	}
	else
	{
		if (inner)
		{
			const Rectangle& rect = inner->getRegion();
			_screenRegion.width = rect.width;
			_screenRegion.height = rect.height;
		}
		else
		{
			_screenRegion.width = _radius * 2.0f;
			_screenRegion.height = _screenRegion.width;
		}
	}

	_index = properties->getInt("index");
}

void JoystickControl::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to this control.");
    }

    Control::addListener(listener, eventFlags);
}

bool JoystickControl::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
        case Touch::TOUCH_PRESS:
        {
            if (_contactIndex == INVALID_CONTACT_INDEX)
            {
                float dx = 0.0f;
                float dy = 0.0f;

                _contactIndex = (int)contactIndex;

                // Get the displacement of the touch from the centre.
                if (!_relative)
                {
                    dx = x - _screenRegion.width * 0.5f;
                    dy = _screenRegion.height * 0.5f - y;
                }
                else
                {
                    _screenRegion.x = x + _bounds.x - _screenRegion.width * 0.5f;
                    _screenRegion.y = y + _bounds.y - _screenRegion.height * 0.5f;
                }

                _displacement.set(dx, dy);

                // If the displacement is greater than the radius, then cap the displacement to the
                // radius.

                Vector2 value;
                if ((fabs(_displacement.x) > _radius) || (fabs(_displacement.y) > _radius))
                {
                    _displacement.normalize();
                    value.set(_displacement);
                    _displacement.scale(_radius);
                }
                else
                {
                    value.set(_displacement);
                    GP_ASSERT(_radius);
                    value.scale(1.0f / _radius);
                }

                // Check if the value has changed. Won't this always be the case?
                if (_value != value)
                {
                    _value.set(value);
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                return true;
            }
            break;
        }

        case Touch::TOUCH_MOVE:
        {
            if (_contactIndex == (int) contactIndex)
            {
                float dx = x - ((_relative) ? _screenRegion.x - _bounds.x : 0.0f) - _screenRegion.width * 0.5f;
                float dy = -(y - ((_relative) ? _screenRegion.y - _bounds.y : 0.0f) - _screenRegion.height * 0.5f);

                _displacement.set(dx, dy);

                Vector2 value;
                if ((fabs(_displacement.x) > _radius) || (fabs(_displacement.y) > _radius))
                {
                    _displacement.normalize();
                    value.set(_displacement);
                    _displacement.scale(_radius);
                }
                else
                {
                    value.set(_displacement);
                    GP_ASSERT(_radius);
                    value.scale(1.0f / _radius);
                }

                if (_value != value)
                {
                    _value.set(value);
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                return true;
            }
            break;
        }

        case Touch::TOUCH_RELEASE:
        {
            if (_contactIndex == (int) contactIndex)
            {
                _contactIndex = INVALID_CONTACT_INDEX;

                // Reset displacement and direction vectors.
                _displacement.set(0.0f, 0.0f);
                Vector2 value(_displacement);
                if (_value != value)
                {
                    _value.set(value);
                    notifyListeners(Control::Listener::VALUE_CHANGED);
                }

                return true;
            }
            break;
        }
    }

    return Control::touchEvent(evt, x, y, contactIndex);
}

unsigned int JoystickControl::drawImages(Form* form, const Rectangle& clip)
{
    Control::State state = getState();

    unsigned int drawCalls = 0;

    // If the JoystickControl is not absolute, then only draw if it is active.
    if (!_relative || (_relative && state == ACTIVE))
    {
        if (!_relative)
        {
            _screenRegion.x = _viewportClipBounds.x + (_viewportClipBounds.width - _screenRegion.width) / 2.0f;
            _screenRegion.y = _viewportClipBounds.y + (_viewportClipBounds.height - _screenRegion.height) / 2.0f;
        }

        SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
        startBatch(form, batch);

        // Draw the outer image.
        Theme::ThemeImage* outer = getImage("outer", state);
        if (outer)
        {
            const Theme::UVs& uvs = outer->getUVs();
            const Vector4& color = outer->getColor();
            if (_relative)
                batch->draw(_screenRegion.x, _screenRegion.y, _outerSize->x, _outerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                batch->draw(_screenRegion.x, _screenRegion.y, _outerSize->x, _outerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
            ++drawCalls;
        }

        // Draw the inner image.
        Theme::ThemeImage* inner = getImage("inner", state);
        if (inner)
        {
            Vector2 position(_screenRegion.x, _screenRegion.y);

            // Adjust position to reflect displacement.
            position.x += _displacement.x;
            position.y += -_displacement.y;

            // Get the uvs and color and draw.
            const Theme::UVs& uvs = inner->getUVs();
            const Vector4& color = inner->getColor();
            if (_relative)
                batch->draw(position.x, position.y, _innerSize->x, _innerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                batch->draw(position.x, position.y, _innerSize->x, _innerSize->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
            ++drawCalls;
        }

        finishBatch(form, batch);
    }

    return drawCalls;
}

const char* JoystickControl::getType() const
{
    return "joystick";
}

}
