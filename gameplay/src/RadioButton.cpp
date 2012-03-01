#include "Base.h"
#include "RadioButton.h"

namespace gameplay
{

static std::vector<RadioButton*> __radioButtons;

RadioButton::RadioButton() : _selected(false)
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
    radioButton->init(style, properties);

    properties->getVector2("iconSize", &radioButton->_iconSize);

    if (properties->getBool("selected"))
    {
        RadioButton::clearSelected(radioButton->_groupId);
        radioButton->_selected = true;
    }

    const char* groupId = properties->getString("group");
    if (groupId)
    {
        radioButton->_groupId = groupId;
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

bool RadioButton::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    if (isEnabled())
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
                        if (_callback)
                        {
                            _callback->trigger(this);
                        }
                        RadioButton::clearSelected(_groupId);
                        _selected = true;
                    }
                }
            }
            break;
        }

        return Button::touchEvent(evt, x, y, contactIndex);
    }

    return false;
}

void RadioButton::clearSelected(const std::string& groupId)
{
    std::vector<RadioButton*>::const_iterator it;
    for (it = __radioButtons.begin(); it < __radioButtons.end(); it++)
    {
        RadioButton* radioButton = *it;
        if (groupId == radioButton->_groupId)
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
        Theme::Border border;
        Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
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

        Vector2 pos(position.x + _position.x + border.left + padding.left,
            position.y + _position.y + (_size.y - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

        if (_selected)
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

void RadioButton::update(const Vector2& position)
{
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

    _viewport.set(pos.x, pos.y,
        _size.x - border.left - padding.left - border.right - padding.right - iconWidth,
        _size.y - border.top - padding.top - border.bottom - padding.bottom - overlay->getFontSize());
}

void RadioButton::drawText(const Vector2& position)
{
    // TODO: Batch all labels that use the same font.
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Font* font = overlay->getFont();
    
    // Draw the text.
    font->begin();
    font->drawText(_text.c_str(), _viewport, overlay->getTextColor(), overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft());
    font->end();

    _dirty = false;
}

}