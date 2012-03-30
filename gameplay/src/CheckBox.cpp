#include "Base.h"
#include "CheckBox.h"
#include "Game.h"

namespace gameplay
{

CheckBox::CheckBox() : _checked(false)
{
}

CheckBox::CheckBox(const CheckBox& copy)
{
    // Hidden.
}

CheckBox::~CheckBox()
{

}

CheckBox* CheckBox::create(Theme::Style* style, Properties* properties)
{
    CheckBox* checkBox = new CheckBox();
    checkBox->init(style, properties);
    properties->getVector2("iconSize", &checkBox->_imageSize);
    checkBox->_checked = properties->getBool("checked");

    return checkBox;
}

bool CheckBox::isChecked()
{
    return _checked;
}

void CheckBox::setImageSize(float width, float height)
{
    _imageSize.set(width, height);
}

const Vector2& CheckBox::getImageSize() const
{
    return _imageSize;
}
/*
void CheckBox::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        assert("TEXT_CHANGED event is not applicable to CheckBox.");
        eventFlags &= ~Control::Listener::TEXT_CHANGED;
    }

    Control::addListener(listener, eventFlags);
}*/

bool CheckBox::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (!isEnabled())
    {
        return false;
    }

    switch (evt)
    {
    case Touch::TOUCH_RELEASE:
        {
            if (_state == Control::ACTIVE)
            {
                if (x > 0 && x <= _bounds.width &&
                    y > 0 && y <= _bounds.height)
                {
                    _checked = !_checked;
                    notifyListeners(Control::Listener::VALUE_CHANGED);

                    // Animate between icons.  Old fades out, then the new fades in.
                    /*
                    AnimationController* animationController = Game::getInstance()->getAnimationController();
                    float from[1] = { 1.0f };
                    float to[1] = { 0.0f };
                    animationController->createAnimationFromTo("CheckBox::toggle", this, CheckBox::ANIMATE_SPRITE_ALPHA, from, to, Curve::QUADRATIC_IN_OUT, 200L);
                    */
                }
            }
        }
        break;
    }

    return Button::touchEvent(evt, x, y, contactIndex);
}

void CheckBox::update(const Rectangle& clip)
{
    Control::update(clip);

    Vector2 size;
    if (_imageSize.isZero())
    {
        if (_checked)
        {
            const Rectangle& selectedRegion = getImageRegion("checked", _state);
            size.set(selectedRegion.width, selectedRegion.height);
        }
        else
        {
            const Rectangle& unselectedRegion = getImageRegion("unchecked", _state);
            size.set(unselectedRegion.width, unselectedRegion.height);
        }
    }
    else
    {
        size.set(_imageSize);
    }
    float iconWidth = size.x;

    _textBounds.x += iconWidth;
    _textBounds.width -= iconWidth;
}

void CheckBox::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    // Left, v-center.
    // TODO: Set an alignment for icons.
    const Theme::Border border = getBorder(_state);
    const Theme::Padding padding = _style->getPadding();
    float opacity = getOpacity(_state);

    if (_checked)
    {
        const Rectangle& selectedRegion = getImageRegion("checked", _state);
        const Theme::UVs& selected = getImageUVs("checked", _state);
        Vector4 selectedColor = getImageColor("checked", _state);
        selectedColor.w *= opacity;

        Vector2 size;
        if (_imageSize.isZero())
        {
            size.set(selectedRegion.width, selectedRegion.height);
        }
        else
        {
            size.set(_imageSize);
        }

        Vector2 pos(clip.x + _position.x + border.left + padding.left,
            clip.y + _position.y + (_bounds.height - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

        spriteBatch->draw(pos.x, pos.y, size.x, size.y, selected.u1, selected.v1, selected.u2, selected.v2, selectedColor, _clip);
    }
    else
    {
        const Rectangle& unselectedRegion = getImageRegion("unchecked", _state);
        const Theme::UVs& unselected = getImageUVs("unchecked", _state);
        Vector4 unselectedColor = getImageColor("unchecked", _state);
        unselectedColor.w *= opacity;

        Vector2 size;
        if (_imageSize.isZero())
        {
            size.set(unselectedRegion.width, unselectedRegion.height);
        }
        else
        {
            size.set(_imageSize);
        }

        Vector2 pos(clip.x + _position.x + border.left + padding.left,
            clip.y + _position.y + (_bounds.height - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

        spriteBatch->draw(pos.x, pos.y, size.x, size.y, unselected.u1, unselected.v1, unselected.u2, unselected.v2, unselectedColor, _clip);
    }
}

}
