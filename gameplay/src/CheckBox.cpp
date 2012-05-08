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
    GP_ASSERT(properties);

    CheckBox* checkBox = new CheckBox();
    checkBox->initialize(style, properties);
    properties->getVector2("imageSize", &checkBox->_imageSize);
    checkBox->_checked = properties->getBool("checked");

    return checkBox;
}

bool CheckBox::isChecked()
{
    return _checked;
}

void CheckBox::setChecked(bool checked)
{
    if (_checked != checked)
    {
        _checked = checked;
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }
}

void CheckBox::setImageSize(float width, float height)
{
    _imageSize.set(width, height);
}

const Vector2& CheckBox::getImageSize() const
{
    return _imageSize;
}

void CheckBox::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to CheckBox.");
        eventFlags &= ~Control::Listener::TEXT_CHANGED;
    }

    Control::addListener(listener, eventFlags);
}

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
                if (x > 0 && x <= _clipBounds.width &&
                    y > 0 && y <= _clipBounds.height)
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
    Label::update(clip);

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

    _textBounds.x += iconWidth + 5;
    _textBounds.width -= iconWidth + 5;

    if (_checked)
    {
        _image = getImage("checked", _state);
    }
    else
    {
        _image = getImage("unchecked", _state);
    }
}

void CheckBox::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    GP_ASSERT(spriteBatch);
    GP_ASSERT(_image);

    // Left, v-center.
    // TODO: Set an alignment for icons.
    const Theme::Border& border = getBorder(_state);
    const Theme::Padding padding = getPadding();
    
    const Rectangle& region = _image->getRegion();
    const Theme::UVs& uvs = _image->getUVs();
    Vector4 color = _image->getColor();
    color.w *= _opacity;

    Vector2 size;
    if (_imageSize.isZero())
    {
        size.set(region.width, region.height);
    }
    else
    {
        size.set(_imageSize);
    }

    Vector2 pos(clip.x + _bounds.x + border.left + padding.left,
        clip.y + _bounds.y + (_clipBounds.height - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

    spriteBatch->draw(pos.x, pos.y, size.x, size.y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _clip);
}

}
