#include "Base.h"
#include "CheckBox.h"

namespace gameplay
{

static std::vector<CheckBox*> __checkBoxes;

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
    CheckBox* checkbox = new CheckBox();
    checkbox->_style = style;
    checkbox->_id = properties->getId();
    properties->getVector2("position", &checkbox->_position);
    properties->getVector2("size", &checkbox->_size);
    checkbox->_checked = properties->getBool("checked");
    checkbox->_text = properties->getString("text");

    __checkBoxes.push_back(checkbox);

    return checkbox;
}

CheckBox* CheckBox::getCheckBox(const char* id)
{
    std::vector<CheckBox*>::const_iterator it;
    for (it = __checkBoxes.begin(); it < __checkBoxes.end(); it++)
    {
        CheckBox* checkbox = *it;
        if (strcmp(id, checkbox->getID()) == 0)
        {
            return checkbox;
        }
    }

    return NULL;
}

void CheckBox::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    switch (evt)
    {
    case Touch::TOUCH_RELEASE:
        {
            if (_state == Control::STATE_ACTIVE)
            {
                if (x > 0 && x <= _size.x &&
                    y > 0 && y <= _size.y)
                {
                    _checked = !_checked;
                }
            }
        }
        break;
    }

    Button::touchEvent(evt, x, y, contactIndex);
}

void CheckBox::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
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

        const Vector2 size = icon->getSize();
        const Vector4 color = icon->getColor();

        Vector2 pos(position.x + _position.x + border.left + padding.left,
            position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

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

void CheckBox::drawText(const Vector2& position)
{
    // TODO: Batch all labels that use the same font.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getCheckBoxIcon();
    Theme::Border border;
    Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
    if (containerRegion)
    {
        border = overlay->getContainerRegion()->getBorder();
    }
    Theme::Padding padding = _style->getPadding();

    // Set up the text viewport.
    float iconWidth = 0.0f;
    if (icon)
    {
        iconWidth = icon->getSize().x;
    }

    Font* font = overlay->getFont();
    Vector2 pos(position.x + _position.x + border.left + padding.left + iconWidth,
            position.y + _position.y + border.top + padding.top);

    Rectangle viewport(pos.x, pos.y,
        _size.x - border.left - padding.left - border.right - padding.right - iconWidth,
        _size.y - border.top - padding.top - border.bottom - padding.bottom - font->getSize());
    
    // Draw the text.
    font->begin();
    font->drawText(_text.c_str(), viewport, overlay->getTextColor(), overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft());
    font->end();

    _dirty = false;
}

}
