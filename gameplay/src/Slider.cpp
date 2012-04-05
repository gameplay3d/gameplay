#include "Slider.h"

namespace gameplay
{

Slider::Slider()
{
}

Slider::~Slider()
{
}

Slider* Slider::create(Theme::Style* style, Properties* properties)
{
    Slider* slider = new Slider();
    slider->initialize(style, properties);

    slider->_min = properties->getFloat("min");
    slider->_max = properties->getFloat("max");
    slider->_value = properties->getFloat("value");
    slider->_step = properties->getFloat("step");

    return slider;
}

void Slider::setMin(float min)
{
    _min = min;
}

float Slider::getMin()
{
    return _min;
}

void Slider::setMax(float max)
{
    _max = max;
}

float Slider::getMax()
{
    return _max;
}

void Slider::setStep(float step)
{
    _step = step;
}

float Slider::getStep()
{
    return _step;
}

float Slider::getValue()
{
    return _value;
}

void Slider::setValue(float value)
{
    _value = MATH_CLAMP(value, _min, _max);
}

void Slider::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
    {
        assert("TEXT_CHANGED event is not applicable to Slider.");
        eventFlags &= ~Listener::TEXT_CHANGED;
    }

    Control::addListener(listener, eventFlags);
}

bool Slider::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!isEnabled())
    {
        return false;
    }

    switch (evt)
    {
    case Touch::TOUCH_PRESS:
        _state = Control::ACTIVE;
        // Fall through to calculate new value.

    case Touch::TOUCH_MOVE:
    case Touch::TOUCH_RELEASE:
        if (_state == ACTIVE &&
            x > 0 && x <= _clipBounds.width &&
            y > 0 && y <= _clipBounds.height)
        {
            // Horizontal case.
            const Theme::Border& border = getBorder(_state);
            const Theme::Padding& padding = getPadding();
            const Rectangle& minCapRegion = getImageRegion("minCap", _state);
            const Rectangle& maxCapRegion = getImageRegion("maxCap", _state);

            float markerPosition = ((float)x - maxCapRegion.width - border.left - padding.left) /
                (_clipBounds.width - border.left - border.right - padding.left - padding.right - minCapRegion.width - maxCapRegion.width);
            
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
    }

    return Control::touchEvent(evt, x, y, contactIndex);
}

void Slider::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    // TODO: Vertical slider.

    // The slider is drawn in the center of the control (perpendicular to orientation).
    // The track is stretched according to orientation.
    // Caps and marker are not stretched.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding& padding = getPadding();

    const Rectangle& minCapRegion = getImageRegion("minCap", _state);
    const Rectangle& maxCapRegion = getImageRegion("maxCap", _state);
    const Rectangle& markerRegion = getImageRegion("marker", _state);
    const Rectangle& trackRegion = getImageRegion("track", _state);

    const Theme::UVs minCap = getImageUVs("minCap", _state);
    const Theme::UVs maxCap = getImageUVs("maxCap", _state);
    const Theme::UVs marker = getImageUVs("marker", _state);
    const Theme::UVs track = getImageUVs("track", _state);

    Vector4 minCapColor = getImageColor("minCap", _state);
    Vector4 maxCapColor = getImageColor("maxCap", _state);
    Vector4 markerColor = getImageColor("marker", _state);
    Vector4 trackColor = getImageColor("track", _state);

    float opacity = getOpacity(_state);
    minCapColor.w *= opacity;
    maxCapColor.w *= opacity;
    markerColor.w *= opacity;
    trackColor.w *= opacity;

    // Draw order: track, caps, marker.
    float midY = clip.y + _clipBounds.y + (_clipBounds.height - border.bottom - padding.bottom) / 2.0f;
    Vector2 pos(clip.x + _clipBounds.x + border.left + padding.left, midY - trackRegion.height / 2.0f);
    spriteBatch->draw(pos.x, pos.y, _clipBounds.width, trackRegion.height, track.u1, track.v1, track.u2, track.v2, trackColor, _clip);

    pos.y = midY - minCapRegion.height * 0.5f;
    pos.x -= minCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, minCapRegion.width, minCapRegion.height, minCap.u1, minCap.v1, minCap.u2, minCap.v2, minCapColor, _clip);
        
    pos.x = clip.x + _clipBounds.x + _clipBounds.width - border.right - padding.right - maxCapRegion.width * 0.5f;
    spriteBatch->draw(pos.x, pos.y, maxCapRegion.width, maxCapRegion.height, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, maxCapColor, _clip);

    // Percent across.
    float markerPosition = (_value - _min) / (_max - _min);
    markerPosition *= _clipBounds.width - border.left - padding.left - border.right - padding.right - minCapRegion.width * 0.5f - maxCapRegion.width * 0.5f - markerRegion.width;
    pos.x = clip.x + _clipBounds.x + border.left + padding.left + minCapRegion.width * 0.5f + markerPosition;
    pos.y = midY - markerRegion.height / 2.0f;
    spriteBatch->draw(pos.x, pos.y, markerRegion.width, markerRegion.height, marker.u1, marker.v1, marker.u2, marker.v2, markerColor, _clip);
}

}
