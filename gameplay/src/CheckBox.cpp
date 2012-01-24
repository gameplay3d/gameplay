#include "Base.h"
#include "CheckBox.h"

namespace gameplay
{

static std::vector<CheckBox*> __checkBoxes;

CheckBox::CheckBox()
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
    case Touch::TOUCH_PRESS:
        {
            _state = Control::STATE_ACTIVE;
        }
        break;
    case Touch::TOUCH_RELEASE:
        {
            if (_state == Control::STATE_ACTIVE)
            {
                if (_callback)
                {
                    _callback->trigger(this);
                }
                _checked = !_checked;
                setState(Control::STATE_NORMAL);
            }
        }
        break;
    }
}

void CheckBox::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
{
    // Left, v-center.
    // TODO: Set an alignment for icons.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::CheckBoxIcon* icon = overlay->getCheckBoxIcon();
    if (icon)
    {
        Theme::Border border = _style->getBorder();
        Theme::Padding padding = _style->getPadding();

        Vector2 pos(position.x + _position.x + border.left + padding.left,
            position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f - icon->size.y / 2.0f);

        if (_checked)
        {
            spriteBatch->draw(pos.x, pos.y, icon->size.x, icon->size.y, icon->on.u1, icon->on.v1, icon->on.u2, icon->on.v2, overlay->getBorderColor());
        }
        else
        {
            spriteBatch->draw(pos.x, pos.y, icon->size.x, icon->size.y, icon->off.u1, icon->off.v1, icon->off.u2, icon->off.v2, overlay->getBorderColor());
        }
    }
}

void CheckBox::drawText(const Vector2& position)
{
    // TODO: Batch all labels that use the same font.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::CheckBoxIcon* icon = overlay->getCheckBoxIcon();
    Theme::Border border = _style->getBorder();
    Theme::Padding padding = _style->getPadding();

    // Set up the text viewport.
    float iconWidth = 0.0f;
    if (icon)
    {
        iconWidth = icon->size.x;
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