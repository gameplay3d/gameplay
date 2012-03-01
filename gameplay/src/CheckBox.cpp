#include "Base.h"
#include "CheckBox.h"

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
    properties->getVector2("iconSize", &checkBox->_iconSize);
    checkBox->_checked = properties->getBool("checked");

    return checkBox;
}

bool CheckBox::isChecked()
{
    return _checked;
}

void CheckBox::setIconSize(float width, float height)
{
    _iconSize.set(width, height);
}

const Vector2& CheckBox::getIconSize() const
{
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getCheckBoxIcon();
    if (_iconSize.isZero() && icon)
    {
        return icon->getSize();
    }

    return _iconSize;
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
                if (x > 0 && x <= _bounds.width &&
                    y > 0 && y <= _bounds.height)
                {
                    _checked = !_checked;
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

    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getCheckBoxIcon();
    Vector2& size = _iconSize;
    if (_iconSize.isZero() && icon)
    {
        size = icon->getSize();
    }
    float iconWidth = size.x;

    _clip.x += iconWidth;
    _clip.width -= iconWidth;
}

void CheckBox::drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    // Left, v-center.
    // TODO: Set an alignment for icons.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getCheckBoxIcon();
    if (icon)
    {
        const Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
        Theme::Border border;
        if (containerRegion)
        {
                border = containerRegion->getBorder();
        }
        const Theme::Padding padding = _style->getPadding();

        Vector2& size = _iconSize;
        if (_iconSize.isZero())
        {
            size = icon->getSize();
        }

        const Vector4 color = icon->getColor();

        Vector2 pos(clip.x + _position.x + border.left + padding.left,
            clip.y + _position.y + (_bounds.height - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

        if (_checked)
        {
            const Theme::UVs on = icon->getOnUVs();
            spriteBatch->draw(pos.x, pos.y, size.x, size.y, on.u1, on.v1, on.u2, on.v2, color);
        }
        else
        {
            const Theme::UVs off = icon->getOffUVs();
            spriteBatch->draw(pos.x, pos.y, size.x, size.y, off.u1, off.v1, off.u2, off.v2, color);
        }
    }
}

void CheckBox::drawText(const Rectangle& clip)
{
    // TODO: Batch all labels that use the same font.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Font* font = overlay->getFont();
    
    // Draw the text.
    font->begin();
    font->drawText(_text.c_str(), _clip, overlay->getTextColor(), overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft());
    font->end();

    _dirty = false;
}

}
