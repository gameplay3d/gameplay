#include "Slider.h"

namespace gameplay
{

// Fraction of slider to scroll when mouse scrollwheel is used.
static const float SCROLL_FRACTION = 0.1f;
// Distance that a slider must be moved before it starts consuming input events,
// e.g. to prevent its parent container from scrolling at the same time.
static const float SLIDER_THRESHOLD = 5.0f;

Slider::Slider() : _min(0.0f), _max(0.0f), _step(0.0f), _value(0.0f), _minImage(NULL),
    _maxImage(NULL), _trackImage(NULL), _markerImage(NULL), _valueTextVisible(false),
    _valueTextAlignment(Font::ALIGN_BOTTOM_HCENTER), _valueTextPrecision(0), _valueText("")
{
}

Slider::~Slider()
{
}

Slider* Slider::create(const char* id, Theme::Style* style)
{
    GP_ASSERT(style);

    Slider* slider = new Slider();
    if (id)
        slider->_id = id;
    slider->setStyle(style);

    return slider;
}

Slider* Slider::create(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);

    Slider* slider = new Slider();
    slider->initialize(style, properties);

    slider->_min = properties->getFloat("min");
    slider->_max = properties->getFloat("max");
    slider->_value = properties->getFloat("value");
    slider->_step = properties->getFloat("step");
    slider->_valueTextVisible = properties->getBool("valueTextVisible");
    slider->_valueTextPrecision = properties->getInt("valueTextPrecision");

    if (properties->exists("valueTextAlignment"))
    {
        slider->_valueTextAlignment = Font::getJustify(properties->getString("valueTextAlignment"));
    }

    return slider;
}

void Slider::setMin(float min)
{
    _min = min;
}

float Slider::getMin() const
{
    return _min;
}

void Slider::setMax(float max)
{
    _max = max;
}

float Slider::getMax() const
{
    return _max;
}

void Slider::setStep(float step)
{
    _step = step;
}

float Slider::getStep() const
{
    return _step;
}

float Slider::getValue() const
{
    return _value;
}

void Slider::setValue(float value)
{
    _value = MATH_CLAMP(value, _min, _max);
}

void Slider::setValueTextVisible(bool valueTextVisible)
{
    _valueTextVisible = valueTextVisible;
}

bool Slider::isValueTextVisible() const
{
    return _valueTextVisible;
}

void Slider::setValueTextAlignment(Font::Justify alignment)
{
    _valueTextAlignment = alignment;
}

Font::Justify Slider::getValueTextAlignment() const
{
    return _valueTextAlignment;
}

void Slider::setValueTextPrecision(unsigned int precision)
{
    _valueTextPrecision = precision;
}

unsigned int Slider::getValueTextPrecision() const
{
    return _valueTextPrecision;
}

void Slider::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to Slider.");
    }

    Control::addListener(listener, eventFlags);
}

bool Slider::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        if (_contactIndex != INVALID_CONTACT_INDEX)
            return false;
        _state = Control::ACTIVE;
        _originalX = x;
        _originalValue = _value;
        _originalConsumeInputEvents = _consumeInputEvents;
        _moveCancelled = false;
        
        // Fall through to calculate new value.
    case Touch::TOUCH_MOVE:
    
        if (evt != Touch::TOUCH_PRESS && _contactIndex != (int)contactIndex)
            return false;

        if (_moveCancelled)
        {
            break;
        }
        else if (abs(x - _originalX) > SLIDER_THRESHOLD)
        {
            // Start consuming input events once we've passed the slider's threshold.
            _consumeInputEvents = true;
        }
        else if (_parent->isScrolling())
        {
            // Cancel the change in slider value if we pass the parent container's scrolling threshold.
            float oldValue = _value;
            _value = _originalValue;
            if (_value != oldValue)
            {
                notifyListeners(Listener::VALUE_CHANGED);
            }

            _dirty = true;
            _moveCancelled = true;
            _state = NORMAL;
            _contactIndex = INVALID_CONTACT_INDEX;
            _consumeInputEvents = _originalConsumeInputEvents;
            break;
        }

        if (_state == ACTIVE &&
            x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
            y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
        {
            // Horizontal case.
            const Theme::Border& border = getBorder(_state);
            const Theme::Padding& padding = getPadding();
            const Rectangle& minCapRegion = _minImage->getRegion();
            const Rectangle& maxCapRegion = _maxImage->getRegion();

            float markerPosition = ((float)x - maxCapRegion.width - border.left - padding.left) /
                (_bounds.width - border.left - border.right - padding.left - padding.right - minCapRegion.width - maxCapRegion.width);
            
            if (markerPosition > 1.0f)
            {
                markerPosition = 1.0f;
            }
            else if (markerPosition < 0.0f)
            {
                markerPosition = 0.0f;
            }

            float oldValue = _value;
            _value = (markerPosition * (_max - _min)) + _min;
            if (_step > 0.0f)
            {
                float stepDistance = _step / (_max - _min);
            
                int numSteps = round(_value / _step);
                _value = _step * numSteps;
            }

            // Call the callback if our value changed.
            if (_value != oldValue)
            {
                notifyListeners(Listener::VALUE_CHANGED);
            }
            _dirty = true;
        }
        break;
    case Touch::TOUCH_RELEASE:
        _consumeInputEvents = _originalConsumeInputEvents;

        if (_contactIndex != (int) contactIndex)
            return false;

        _dirty = true;
        _state = FOCUS;
        break;
    }
    
    if (evt == Touch::TOUCH_MOVE)
        return _consumeInputEvents;
    else
        return Control::touchEvent(evt, x, y, contactIndex);
}

bool Slider::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    switch (evt)
    {
        case Mouse::MOUSE_PRESS_LEFT_BUTTON:
            return touchEvent(Touch::TOUCH_PRESS, x, y, 0);

        case Mouse::MOUSE_MOVE:
            return touchEvent(Touch::TOUCH_MOVE, x, y, 0);

        case Mouse::MOUSE_RELEASE_LEFT_BUTTON:
            return touchEvent(Touch::TOUCH_RELEASE, x, y, 0);

        case Mouse::MOUSE_WHEEL:
        {
            if (_state == FOCUS || _state == ACTIVE)
            {
                float total = _max - _min;
                float oldValue = _value;
                _value += (total * SCROLL_FRACTION) * wheelDelta;
            
                if (_value > _max)
                    _value = _max;
                else if (_value < _min)
                    _value = _min;

                if (_value != oldValue)
                {
                    notifyListeners(Listener::VALUE_CHANGED);
                }

                _dirty = true;
                return _consumeInputEvents;
            }
            break;
        }

        default:
            break;
    }

    return false;
}

void Slider::update(const Control* container, const Vector2& offset)
{
    Label::update(container, offset);

    _minImage = getImage("minCap", _state);
    _maxImage = getImage("maxCap", _state);
    _markerImage = getImage("marker", _state);
    _trackImage = getImage("track", _state);

    char s[32];
    sprintf(s, "%.*f", _valueTextPrecision, _value);
    _valueText = s;
}

void Slider::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    GP_ASSERT(spriteBatch);
    GP_ASSERT(_minImage);
    GP_ASSERT(_maxImage);
    GP_ASSERT(_markerImage);
    GP_ASSERT(_trackImage);

    // TODO: Vertical slider.

    // The slider is drawn in the center of the control (perpendicular to orientation).
    // The track is stretched according to orientation.
    // Caps and marker are not stretched.
    const Rectangle& minCapRegion = _minImage->getRegion();
    const Rectangle& maxCapRegion = _maxImage->getRegion();
    const Rectangle& markerRegion = _markerImage->getRegion();
    const Rectangle& trackRegion = _trackImage->getRegion();

    const Theme::UVs& minCap = _minImage->getUVs();
    const Theme::UVs& maxCap = _maxImage->getUVs();
    const Theme::UVs& marker = _markerImage->getUVs();
    const Theme::UVs& track = _trackImage->getUVs();

    Vector4 minCapColor = _minImage->getColor();
    Vector4 maxCapColor = _maxImage->getColor();
    Vector4 markerColor = _markerImage->getColor();
    Vector4 trackColor = _trackImage->getColor();

    minCapColor.w *= _opacity;
    maxCapColor.w *= _opacity;
    markerColor.w *= _opacity;
    trackColor.w *= _opacity;

    // Draw order: track, caps, marker.
    float midY = _viewportBounds.y + (_viewportBounds.height) * 0.5f;
    Vector2 pos(_viewportBounds.x, midY - trackRegion.height * 0.5f);
    spriteBatch->draw(pos.x, pos.y, _viewportBounds.width, trackRegion.height, track.u1, track.v1, track.u2, track.v2, trackColor, _viewportClipBounds);

    pos.y = midY - minCapRegion.height * 0.5f;
    pos.x -= minCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, minCapRegion.width, minCapRegion.height, minCap.u1, minCap.v1, minCap.u2, minCap.v2, minCapColor, _viewportClipBounds);
        
    pos.x = _viewportBounds.x + _viewportBounds.width - maxCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, maxCapRegion.width, maxCapRegion.height, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, maxCapColor, _viewportClipBounds);

    // Percent across.
    float markerPosition = (_value - _min) / (_max - _min);
    markerPosition *= _viewportBounds.width - minCapRegion.width * 0.5f - maxCapRegion.width * 0.5f - markerRegion.width;
    pos.x = _viewportBounds.x + minCapRegion.width * 0.5f + markerPosition;
    pos.y = midY - markerRegion.height / 2.0f;
    spriteBatch->draw(pos.x, pos.y, markerRegion.width, markerRegion.height, marker.u1, marker.v1, marker.u2, marker.v2, markerColor, _viewportClipBounds);
}

void Slider::drawText(const Rectangle& clip)
{
    Label::drawText(clip);

    if (_valueTextVisible && _font)
    {
        _font->start();
        _font->drawText(_valueText.c_str(), _textBounds, _textColor, getFontSize(_state), _valueTextAlignment, true, getTextRightToLeft(_state), &_viewportClipBounds);
        _font->finish();
    }
}

const char* Slider::getType() const
{
    return "slider";
}

}
