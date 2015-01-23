#include "Base.h"
#include "JoystickControl.h"

namespace gameplay
{

JoystickControl::JoystickControl() : _radiusPixels(1.0f), _relative(true), _innerSizePixels(NULL), _outerSizePixels(NULL), _index(0),
    _innerRegionCoord(NULL), _outerRegionCoord(NULL), _innerRegionCoordBoundsBits(0), _outerRegionCoordBoundsBits(0), _radiusCoord(_radiusPixels)
{
}

JoystickControl::~JoystickControl()
{
    if (_innerSizePixels)
        SAFE_DELETE(_innerSizePixels);
    if (_outerSizePixels)
        SAFE_DELETE(_outerSizePixels);
    if(_innerRegionCoord)
        SAFE_DELETE_ARRAY(_innerRegionCoord);
    if(_outerRegionCoord)
        SAFE_DELETE_ARRAY(_outerRegionCoord);
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

const char* JoystickControl::getTypeName() const
{
    return "JoystickControl";
}

const Vector2& JoystickControl::getValue() const
{
    return _value;
}

void JoystickControl::setInnerRegionSize(const Vector2& size, bool isWidthPercentage, bool isHeightPercentage)
{
    if (_innerSizePixels)
    {
        if(!_innerRegionCoord)
        {
            _innerRegionCoord = new Vector2();
        }

        setRegion(size, *_innerRegionCoord, _innerRegionCoordBoundsBits, isWidthPercentage, isHeightPercentage);
        updateAbsoluteSizes();
    }
}

const Vector2& JoystickControl::getInnerRegionSize(bool* isWidthPercentage, bool* isHeightPercentage) const
{
    if(isWidthPercentage)
    {
        *isWidthPercentage = _innerSizePixels && (_innerRegionCoord && _innerRegionCoordBoundsBits & BOUNDS_WIDTH_PERCENTAGE_BIT || Control::isXPercentage());
    }

    if(isHeightPercentage)
    {
        *isHeightPercentage = _innerSizePixels && (_innerRegionCoord && _innerRegionCoordBoundsBits & BOUNDS_HEIGHT_PERCENTAGE_BIT || Control::isYPercentage());
    }

    if (_innerSizePixels)
        return *_innerSizePixels;
    else
        return Vector2::zero();
}

void JoystickControl::setOuterRegionSize(const Vector2& size, bool isWidthPercentage, bool isHeightPercentage)
{
    if (_outerSizePixels)
    {
        if(!_outerRegionCoord)
        {
            _outerRegionCoord = new Vector2();
        }

        setRegion(size, *_outerRegionCoord, _outerRegionCoordBoundsBits, isWidthPercentage, isHeightPercentage);
        updateAbsoluteSizes();
    }
}

const Vector2& JoystickControl::getOuterRegionSize(bool* isWidthPercentage, bool* isHeightPercentage) const
{
    if(isWidthPercentage)
    {
        *isWidthPercentage = _outerSizePixels && (_outerRegionCoord && _outerRegionCoordBoundsBits & BOUNDS_WIDTH_PERCENTAGE_BIT || Control::isXPercentage());
    }

    if(isHeightPercentage)
    {
        *isHeightPercentage = _outerSizePixels && (_outerRegionCoord && _outerRegionCoordBoundsBits & BOUNDS_HEIGHT_PERCENTAGE_BIT || Control::isYPercentage());
    }

    if (_outerSizePixels)
        return *_outerSizePixels;
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

void JoystickControl::setBoundsBit(bool set, int& bitSetOut, int bit)
{
    if(set)
    {
        bitSetOut |= bit;
    }
    else
    {
        bitSetOut &= ~bit;
    }
}

void JoystickControl::setRadius(float radius, bool isPercentage)
{
    _radiusCoord = radius;
    setBoundsBit(isPercentage, _boundsBits, BOUNDS_RADIUS_PERCENTAGE_BIT);
    updateAbsoluteSizes();
}

float JoystickControl::getRadius() const
{
    return _radiusCoord;
}

bool JoystickControl::isRadiusPercentage() const
{
    return _boundsBits & BOUNDS_RADIUS_PERCENTAGE_BIT;
}

void JoystickControl::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Control::initialize(typeName, style, properties);

    if (!properties)
    {
        GP_WARN("JoystickControl creation without properties object is unsupported.");
        return;
    }

    const char* radiusId = "radius";
    if (!properties->exists(radiusId))
    {
        GP_WARN("JoystickControl: required attribute 'radius' is missing.");
    }
    else
    {
        const char* radiusStr = properties->getString(radiusId);
        bool isPercentage = false;
        _radiusCoord = parseCoord(radiusStr, &isPercentage);
        setBoundsBit(isPercentage, _boundsBits, BOUNDS_RADIUS_PERCENTAGE_BIT);
    }

    const char* relativeId = "relative";
    if (properties->exists(relativeId))
    {
        setRelative(properties->getBool(relativeId));
    }
    else
    {
        setRelative(false);
    }

    const char* innerRegionId = "innerRegion";
    if(properties->exists(innerRegionId))
    {
        _innerRegionCoord = new Vector2();
        getRegion(*_innerRegionCoord, _innerRegionCoordBoundsBits, properties->getString(innerRegionId));
    }

    const char* outerRegionId = "outerRegion";
    if(properties->exists(outerRegionId))
    {
        _outerRegionCoord = new Vector2();
        getRegion(*_outerRegionCoord, _outerRegionCoordBoundsBits, properties->getString(outerRegionId));
    }

    _index = properties->getInt("index");
}

void JoystickControl::updateAbsoluteBounds(const Vector2& offset)
{
    Control::updateAbsoluteBounds(offset);
    updateAbsoluteSizes();
}

void JoystickControl::setRegion(const Vector2& regionSizeIn, Vector2& regionSizeOut, int& regionBoundsBitsOut, bool isWidthPercentage, bool isHeightPercentage)
{
    regionSizeOut = regionSizeIn;
    setBoundsBit(isWidthPercentage, regionBoundsBitsOut, BOUNDS_WIDTH_PERCENTAGE_BIT);
    setBoundsBit(isHeightPercentage, regionBoundsBitsOut, BOUNDS_HEIGHT_PERCENTAGE_BIT);
}

void JoystickControl::getRegion(Vector2& regionOut, int& regionBoundsBitsOut, const char* regionPropertyId)
{
    bool isWidthPercent = false;
    bool isHeightPercent = false;
    parseCoordPair(regionPropertyId, &regionOut.x, &regionOut.y, &isWidthPercent, &isHeightPercent);
    setBoundsBit(isWidthPercent, regionBoundsBitsOut, BOUNDS_WIDTH_PERCENTAGE_BIT);
    setBoundsBit(isHeightPercent, regionBoundsBitsOut, BOUNDS_HEIGHT_PERCENTAGE_BIT);
}

Vector2 JoystickControl::getPixelSize(const Vector2& region, const int regionBoundsBits) const
{
    Vector2 size;
    size.x = regionBoundsBits & BOUNDS_WIDTH_PERCENTAGE_BIT ? _absoluteBounds.width * region.x : region.x;
    size.y = regionBoundsBits & BOUNDS_HEIGHT_PERCENTAGE_BIT ? _absoluteBounds.height * region.y: region.y;
    return size;
}

Vector2 JoystickControl::getPixelSize(const Theme::ThemeImage* image) const
{
    Rectangle rect = image->getRegion();
    rect.width = isWidthPercentage() ? (_absoluteBounds.width / rect.width) * rect.width : rect.width;
    rect.height = isHeightPercentage() ? (_absoluteBounds.height / rect.height) * rect.height : rect.height;
    return Vector2(rect.width, rect.height);
}

Theme::ThemeImage * JoystickControl::getNonEmptyImage(const char* id, Control::State state)
{
    Theme::ThemeImage* image = getImage(id, state);
    return strcmp(image->getId(), id) == 0 ? image : NULL;
}

void JoystickControl::updateAbsoluteSizes()
{
    const Control::State state = getState();

    Theme::ThemeImage* innerImage = getNonEmptyImage("inner", state);
    const bool innerRegionDefined = _innerRegionCoord || innerImage;

    if(innerRegionDefined)
    {
        if(!_innerSizePixels)
        {
            _innerSizePixels = new Vector2();
        }

        *_innerSizePixels = _innerRegionCoord ? getPixelSize(*_innerRegionCoord, _innerRegionCoordBoundsBits) : getPixelSize(innerImage);
        _screenRegionPixels.width = _innerSizePixels->x;
        _screenRegionPixels.height = _innerSizePixels->y;
    }

    Theme::ThemeImage* outerImage = getNonEmptyImage("outer", state);
    const bool outerRegionDefined = _outerRegionCoord || outerImage;

    if (outerRegionDefined)
    {
        if(!_outerSizePixels)
        {
            _outerSizePixels = new Vector2();
        }

        *_outerSizePixels = _outerRegionCoord ? getPixelSize(*_outerRegionCoord, _outerRegionCoordBoundsBits) : getPixelSize(outerImage);
        _screenRegionPixels.width = _outerSizePixels->x > _screenRegionPixels.width ? _outerSizePixels->x : _screenRegionPixels.width;
        _screenRegionPixels.height = _outerSizePixels->y > _screenRegionPixels.height ? _outerSizePixels->y : _screenRegionPixels.height;
    }

    _radiusPixels = std::max(1.0f, _boundsBits & BOUNDS_RADIUS_PERCENTAGE_BIT ?
                std::min(_screenRegionPixels.width, _screenRegionPixels.height) * _radiusCoord : _radiusCoord);

    if (!innerRegionDefined && !outerRegionDefined)
    {
        _screenRegionPixels.width = _radiusPixels * 2.0f;
        _screenRegionPixels.height = _screenRegionPixels.width;
    }
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
                    dx = x - _screenRegionPixels.width * 0.5f;
                    dy = _screenRegionPixels.height * 0.5f - y;
                }
                else
                {
                    _screenRegionPixels.x = x + _bounds.x - _screenRegionPixels.width * 0.5f;
                    _screenRegionPixels.y = y + _bounds.y - _screenRegionPixels.height * 0.5f;
                }

                _displacement.set(dx, dy);

                // If the displacement is greater than the radius, then cap the displacement to the
                // radius.

                Vector2 value;
                if ((fabs(_displacement.x) > _radiusPixels) || (fabs(_displacement.y) > _radiusPixels))
                {
                    _displacement.normalize();
                    value.set(_displacement);
                    _displacement.scale(_radiusPixels);
                }
                else
                {
                    value.set(_displacement);
                    GP_ASSERT(_radiusPixels);
                    value.scale(1.0f / _radiusPixels);
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
                float dx = x - ((_relative) ? _screenRegionPixels.x - _bounds.x : 0.0f) - _screenRegionPixels.width * 0.5f;
                float dy = -(y - ((_relative) ? _screenRegionPixels.y - _bounds.y : 0.0f) - _screenRegionPixels.height * 0.5f);

                _displacement.set(dx, dy);

                Vector2 value;
                if ((fabs(_displacement.x) > _radiusPixels) || (fabs(_displacement.y) > _radiusPixels))
                {
                    _displacement.normalize();
                    value.set(_displacement);
                    _displacement.scale(_radiusPixels);
                }
                else
                {
                    value.set(_displacement);
                    GP_ASSERT(_radiusPixels);
                    value.scale(1.0f / _radiusPixels);
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
    const Control::State state = getState();

    unsigned int drawCalls = 0;

    // If the JoystickControl is not absolute, then only draw if it is active.
    if ((_outerSizePixels || _innerSizePixels) && !_relative || (_relative && state == ACTIVE))
    {
        if (!_relative)
        {
            _screenRegionPixels.x = _viewportClipBounds.x + (_viewportClipBounds.width - _screenRegionPixels.width) / 2.0f;
            _screenRegionPixels.y = _viewportClipBounds.y + (_viewportClipBounds.height - _screenRegionPixels.height) / 2.0f;
        }

        SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
        startBatch(form, batch);

        // Draw the outer image.
        if (_outerSizePixels)
        {
            Theme::ThemeImage* outer = getImage("outer", state);
            const Theme::UVs& uvs = outer->getUVs();
            const Vector4& color = outer->getColor();

            Vector2 position(_screenRegionPixels.x, _screenRegionPixels.y);

            if(_outerRegionCoord)
            {
                position.x += (_screenRegionPixels.width / 2) - (_outerSizePixels->x / 2);
                position.y += (_screenRegionPixels.height / 2) - (_outerSizePixels->y / 2);
            }

            if (_relative)
                batch->draw(position.x, position.y, _outerSizePixels->x, _outerSizePixels->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                batch->draw(position.x, position.y, _outerSizePixels->x, _outerSizePixels->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
            ++drawCalls;
        }

        // Draw the inner image.
        if (_innerSizePixels)
        {
            Theme::ThemeImage* inner = getImage("inner", state);
            Vector2 position(_screenRegionPixels.x, _screenRegionPixels.y);

            // Adjust position to reflect displacement.
            position.x += _displacement.x;
            position.y += -_displacement.y;

            if(_innerRegionCoord)
            {
                position.x += (_screenRegionPixels.width / 2) - (_innerSizePixels->x / 2);
                position.y += (_screenRegionPixels.height / 2) - (_innerSizePixels->y / 2);
            }

            // Get the uvs and color and draw.
            const Theme::UVs& uvs = inner->getUVs();
            const Vector4& color = inner->getColor();
            if (_relative)
                batch->draw(position.x, position.y, _innerSizePixels->x, _innerSizePixels->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
            else
                batch->draw(position.x, position.y, _innerSizePixels->x, _innerSizePixels->y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
            ++drawCalls;
        }

        finishBatch(form, batch);
}

    return drawCalls;
}

}
