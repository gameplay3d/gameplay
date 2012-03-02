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
    slider->init(style, properties);

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
        _dirty = true;
        // Fall through to calculate new value.

    case Touch::TOUCH_MOVE:
    case Touch::TOUCH_RELEASE:
        if (_state == ACTIVE &&
            x > 0 && x <= _bounds.width &&
            y > 0 && y <= _bounds.height)
        {
            // Horizontal case.
            Theme::Border border;
            Theme::ContainerRegion* containerRegion = _style->getOverlay(getOverlayType())->getContainerRegion();
            if (containerRegion)
            {
                border = containerRegion->getBorder();
            }
            Theme::Padding padding = _style->getPadding();

            const Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
            const Theme::SliderIcon* icon = overlay->getSliderIcon();

            const Vector2 minCapSize = icon->getMinCapSize();
            const Vector2 maxCapSize = icon->getMaxCapSize();

            float markerPosition = ((float)x - maxCapSize.x - border.left - padding.left) /
                                    (_bounds.width - border.left - border.right - padding.left - padding.right - minCapSize.x - maxCapSize.x);
            if (markerPosition > 1.0f)
            {
                markerPosition = 1.0f;
            }
            else if (markerPosition < 0.0f)
            {
                markerPosition = 0.0f;
            }

            float oldValue = _value;
            _value = markerPosition * _max;
            if (_step > 0.0f)
            {
                float stepDistance = _step / (_max - _min);
            
                int numSteps = round(_value / _step);
                _value = _step * numSteps;
            }

            // Call the callback if our value changed.
            if (_callback && _value != oldValue)
            {
                _callback->trigger(this);
            }

            _dirty = true;
        }
    }

    return _consumeTouchEvents;
}

void Slider::drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    // TODO: Vertical slider.

    // The slider is drawn in the center of the control (perpendicular to orientation).
    // The track is stretched according to orientation.
    // Caps and marker are not stretched.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::SliderIcon* icon = overlay->getSliderIcon();
    if (icon)
    {
        Theme::Border border;
        Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
        if (containerRegion)
        {
            border = containerRegion->getBorder();
        }
        Theme::Padding padding = _style->getPadding();

        const Vector2 minCapSize = icon->getMinCapSize();
        const Vector2 maxCapSize = icon->getMaxCapSize();
        const Vector2 markerSize = icon->getMarkerSize();
        const Vector2 trackSize = icon->getTrackSize();

        const Theme::UVs minCap = icon->getMinCapUVs();
        const Theme::UVs maxCap = icon->getMaxCapUVs();
        const Theme::UVs marker = icon->getMarkerUVs();
        const Theme::UVs track = icon->getTrackUVs();

        const Vector4 color = icon->getColor();

        // Draw order: track, caps, marker.
        float midY = clip.y + _bounds.y + (_bounds.height - border.bottom - padding.bottom) / 2.0f;
        Vector2 pos(clip.x + _bounds.x + border.left + padding.left, midY - trackSize.y / 2.0f);
        spriteBatch->draw(pos.x, pos.y, _bounds.width, trackSize.y, track.u1, track.v1, track.u2, track.v2, color);

        pos.y = midY - minCapSize.y * 0.5f;
        pos.x -= minCapSize.x * 0.5f;
        spriteBatch->draw(pos.x, pos.y, minCapSize.x, minCapSize.y, minCap.u1, minCap.v1, minCap.u2, minCap.v2, color);
        
        pos.x = clip.x + _bounds.x + _bounds.width - border.right - padding.right - maxCapSize.x * 0.5f;
        spriteBatch->draw(pos.x, pos.y, maxCapSize.x, maxCapSize.y, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, color);

        // Percent across.
        float markerPosition = _value / (_max - _min);
        markerPosition *= _bounds.width - border.left - padding.left - border.right - padding.right - minCapSize.x * 0.5f - maxCapSize.x * 0.5f - markerSize.x;
        pos.x = clip.x + _bounds.x + border.left + padding.left + minCapSize.x * 0.5f + markerPosition;
        pos.y = midY - markerSize.y / 2.0f;
        spriteBatch->draw(pos.x, pos.y, markerSize.x, markerSize.y, marker.u1, marker.v1, marker.u2, marker.v2, color);
    }
}

}
