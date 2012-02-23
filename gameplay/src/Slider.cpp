#include "Slider.h"

namespace gameplay
{

static std::vector<Slider*> __sliders;

Slider::Slider()
{
}

Slider::~Slider()
{
}

Slider* Slider::create(Theme::Style* style, Properties* properties)
{
    Slider* slider = new Slider();

    slider->_style = style;
    properties->getVector2("position", &slider->_position);
    properties->getVector2("size", &slider->_size);
    slider->_min = properties->getFloat("min");
    slider->_max = properties->getFloat("max");
    slider->_value = properties->getFloat("value");
    slider->_step = properties->getFloat("step");

    const char* id = properties->getId();
    if (id)
    {
        slider->_id = id;
    }

    const char* text = properties->getString("text");
    if (text)
    {
        slider->_text = text;
    }

    __sliders.push_back(slider);

    return slider;
}

Slider* Slider::create(const char* id, float min, float max, float defaultPosition, float step)
{
    Slider* slider = new Slider();

    return slider;
}

Slider* Slider::getSlider(const char* id)
{
    std::vector<Slider*>::const_iterator it;
    for (it = __sliders.begin(); it < __sliders.end(); it++)
    {
        Slider* slider = *it;
        if (strcmp(id, slider->getID()) == 0)
        {
            return slider;
        }
    }

    return NULL;
}

void Slider::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    Button::touchEvent(evt, x, y, contactIndex);

    if (_state == STATE_ACTIVE &&
        x > 0 && x <= _size.x &&
        y > 0 && y <= _size.y)
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
                               (_size.x - border.left - border.right - padding.left - padding.right - minCapSize.x - maxCapSize.x);
        if (markerPosition > 1.0f)
        {
            markerPosition = 1.0f;
        }
        else if (markerPosition < 0.0f)
        {
            markerPosition = 0.0f;
        }

        _value = markerPosition * _max;
        if (_step > 0.0f)
        {
            float stepDistance = _step / (_max - _min);
            
            int numSteps = round(_value / _step);
            _value = _step * numSteps;
        }
    }
}

void Slider::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
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
        float midY = position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f;
        Vector2 pos(position.x + _position.x + border.left + padding.left, midY - trackSize.y / 2.0f);
        spriteBatch->draw(pos.x, pos.y, _size.x, trackSize.y, track.u1, track.v1, track.u2, track.v2, color);

        pos.y = midY - minCapSize.y * 0.5f;
        pos.x -= minCapSize.x * 0.5f;
        spriteBatch->draw(pos.x, pos.y, minCapSize.x, minCapSize.y, minCap.u1, minCap.v1, minCap.u2, minCap.v2, color);
        
        pos.x = position.x + _position.x + _size.x - border.right - padding.right - maxCapSize.x * 0.5f;
        spriteBatch->draw(pos.x, pos.y, maxCapSize.x, maxCapSize.y, maxCap.u1, maxCap.v1, maxCap.u2, maxCap.v2, color);

        // Percent across.
        float markerPosition = _value / (_max - _min);
        markerPosition *= _size.x - border.left - padding.left - border.right - padding.right - minCapSize.x * 0.5f - maxCapSize.x * 0.5f - markerSize.x;
        pos.x = position.x + _position.x + border.left + padding.left + minCapSize.x * 0.5f + markerPosition;
        pos.y = midY - markerSize.y / 2.0f;
        spriteBatch->draw(pos.x, pos.y, markerSize.x, markerSize.y, marker.u1, marker.v1, marker.u2, marker.v2, color);
    }
}

}
