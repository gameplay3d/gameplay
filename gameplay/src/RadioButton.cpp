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
    // Remove this RadioButton from the global list.
    std::vector<RadioButton*>::iterator it = std::find(__radioButtons.begin(), __radioButtons.end(), this);
    if (it != __radioButtons.end())
    {
        __radioButtons.erase(it);
    }
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

void RadioButton::setIconSize(float width, float height)
{
    _iconSize.set(width, height);
}

const Vector2& RadioButton::getIconSize() const
{
    Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
    Theme::Icon* icon = overlay->getCheckBoxIcon();
    if (_iconSize.isZero() && icon)
    {
        return icon->getSize();
    }

    return _iconSize;
}

void RadioButton::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
    {
        assert("TEXT_CHANGED event is not applicable to RadioButton.");
        eventFlags &= ~Listener::TEXT_CHANGED;
    }

    Control::addListener(listener, eventFlags);
}

bool RadioButton::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
                    if (!_selected)
                    {
                        RadioButton::clearSelected(_groupId);
                        _selected = true;
                        notifyListeners(Listener::VALUE_CHANGED);
                    }
                }
            }
        }
        break;
    }

    return Button::touchEvent(evt, x, y, contactIndex);
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
            radioButton->_dirty = true;
        }
    }
}

void RadioButton::drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip)
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

        Vector2 pos(clip.x + _position.x + border.left + padding.left,
            clip.y + _position.y + (_bounds.height - border.bottom - padding.bottom) / 2.0f - size.y / 2.0f);

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

void RadioButton::update(const Rectangle& clip)
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

    _textBounds.x += iconWidth;
    _textBounds.width -= iconWidth;
}

}