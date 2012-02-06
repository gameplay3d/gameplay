#include "Base.h"
#include "RadioButton.h"

namespace gameplay
{

static std::vector<RadioButton*> __radioButtons;

RadioButton::RadioButton() : _groupId(NULL), _selected(false)
{
}

RadioButton::RadioButton(const RadioButton& copy)
{
    // Hidden.
}

RadioButton::~RadioButton()
{

}

RadioButton* RadioButton::create(Theme::Style* style, Properties* properties)
{
    RadioButton* radioButton = new RadioButton();
    radioButton->_style = style;
    radioButton->_id = properties->getId();
    properties->getVector2("position", &radioButton->_position);
    properties->getVector2("size", &radioButton->_size);
    radioButton->_text = properties->getString("text");
    radioButton->_groupId = properties->getString("group");
    if (properties->getBool("selected"))
    {
        RadioButton::clearSelected(radioButton->_groupId);
        radioButton->_selected = true;
    }

    __radioButtons.push_back(radioButton);

    return radioButton;
}

RadioButton* RadioButton::getRadioButton(const char* id)
{
    std::vector<RadioButton*>::const_iterator it;
    for (it = __radioButtons.begin(); it < __radioButtons.end(); it++)
    {
        RadioButton* radioButton = *it;
        if (strcmp(id, radioButton->getID()) == 0)
        {
            return radioButton;
        }
    }

    return NULL;
}

void RadioButton::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
                if (x > 0 && x <= _size.x &&
                    y > 0 && y <= _size.y)
                {
                    if (_callback)
                    {
                        _callback->trigger(this);
                    }
                    RadioButton::clearSelected(_groupId);
                    _selected = true;
                }
                setState(Control::STATE_NORMAL);
            }
        }
        break;
    }
}

void RadioButton::clearSelected(const char* groupId)
{
    std::vector<RadioButton*>::const_iterator it;
    for (it = __radioButtons.begin(); it < __radioButtons.end(); it++)
    {
        RadioButton* radioButton = *it;
        if (strcmp(groupId, radioButton->_groupId) == 0)
        {
            radioButton->_selected = false;
        }
    }
}

void RadioButton::drawSprites(SpriteBatch* spriteBatch, const Vector2& position)
{
    // Left, v-center.
    // TODO: Set an alignment for icons.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getRadioButtonIcon();
    if (icon)
    {
        Theme::Border border = _style->getBorder();
        Theme::Padding padding = _style->getPadding();

        Vector2 pos(position.x + _position.x + border.left + padding.left,
            position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f - icon->size.y / 2.0f);

        if (_state == STATE_ACTIVE)
        {
            spriteBatch->draw(pos.x, pos.y, icon->size.x, icon->size.y, icon->active.u1, icon->active.v1, icon->active.u2, icon->active.v2, overlay->getBorderColor());
        }
        else if (_selected)
        {
            spriteBatch->draw(pos.x, pos.y, icon->size.x, icon->size.y, icon->on.u1, icon->on.v1, icon->on.u2, icon->on.v2, overlay->getBorderColor());
        }
        else
        {
            spriteBatch->draw(pos.x, pos.y, icon->size.x, icon->size.y, icon->off.u1, icon->off.v1, icon->off.u2, icon->off.v2, overlay->getBorderColor());
        }
    }
}

void RadioButton::drawText(const Vector2& position)
{
    // TODO: Batch all labels that use the same font.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getRadioButtonIcon();
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