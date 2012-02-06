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
    slider->_id = properties->getId();
    properties->getVector2("position", &slider->_position);
    properties->getVector2("size", &slider->_size);
    slider->_min = properties->getFloat("min");
    slider->_max = properties->getFloat("max");
    slider->_value = properties->getFloat("value");
    slider->_step = properties->getFloat("step");
    slider->_text = properties->getString("text");

    __sliders.push_back(slider);

    return slider;
}

Slider* Slider::create(const char* id, float min, float max, float default, float step)
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
        Theme::Border border = _style->getBorder();
        Theme::Padding padding = _style->getPadding();

        Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
        Theme::SliderIcon* icon = overlay->getSliderIcon();

        float markerPosition = ((float)x - icon->rightCapSize.x - border.left - padding.left) /
                               (_size.x - border.left - border.right - padding.left - padding.right - icon->leftCapSize.x - icon->rightCapSize.x);
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
        Theme::Border border = _style->getBorder();
        Theme::Padding padding = _style->getPadding();

        // Draw order: track, caps, marker.
        float midY = position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f;
        Vector2 pos(position.x + _position.x + border.left + padding.left, midY - icon->trackSize.y / 2.0f);
        spriteBatch->draw(pos.x, pos.y, _size.x, icon->trackSize.y, icon->track.u1, icon->track.v1, icon->track.u2, icon->track.v2, overlay->getBorderColor());

        pos.y = midY - icon->leftCapSize.y / 2.0f;
        spriteBatch->draw(pos.x, pos.y, icon->leftCapSize.x, icon->leftCapSize.y, icon->leftCap.u1, icon->leftCap.v1, icon->leftCap.u2, icon->leftCap.v2, overlay->getBorderColor());
        
        pos.x = position.x + _position.x + _size.x - border.left - padding.left - icon->rightCapSize.x;
        spriteBatch->draw(pos.x, pos.y, icon->rightCapSize.x, icon->rightCapSize.y, icon->rightCap.u1, icon->rightCap.v1, icon->rightCap.u2, icon->rightCap.v2, overlay->getBorderColor());

        // Percent across.
        float markerPosition = _value / (_max - _min);
        markerPosition *= _size.x - border.left - padding.left - border.right - padding.right - icon->leftCapSize.x - icon->rightCapSize.x - icon->markerSize.x;
        pos.x = position.x + _position.x + border.left + padding.left + icon->leftCapSize.x + markerPosition;
        pos.y = midY - icon->markerSize.y / 2.0f;
        spriteBatch->draw(pos.x, pos.y, icon->markerSize.x, icon->markerSize.y, icon->marker.u1, icon->marker.v1, icon->marker.u2, icon->marker.v2, overlay->getBorderColor());
    }
}

void Slider::drawText(const Vector2& position)
{

}

}