#include "Base.h"
#include "CheckBox.h"
#include "Game.h"

namespace gameplay
{

CheckBox::CheckBox() : _checked(false), _image(NULL)
{
}

CheckBox::~CheckBox()
{

}

CheckBox* CheckBox::create(const char* id, Theme::Style* style)
{
    GP_ASSERT(style);

    CheckBox* checkBox = new CheckBox();
    if (id)
        checkBox->_id = id;
    checkBox->setStyle(style);

    return checkBox;
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
        _dirty = true;
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
    switch (evt)
    {
    case Touch::TOUCH_RELEASE:
        if (_contactIndex == (int) contactIndex && _state == Control::ACTIVE)
        {
            if (!_parent->isScrolling() &&
                x > _clipBounds.x && x <= _clipBounds.x + _clipBounds.width &&
                y > _clipBounds.y && y <= _clipBounds.y + _clipBounds.height)
            {
                _checked = !_checked;
                notifyListeners(Control::Listener::VALUE_CHANGED);
            }
        }
        break;
    }
    return Button::touchEvent(evt, x, y, contactIndex);
}

bool CheckBox::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
    switch (evt)
    {
    case Gamepad::BUTTON_EVENT:
        if (_state == Control::ACTIVE)
        {
            if (!gamepad->isButtonDown(Gamepad::BUTTON_A) &&
                !gamepad->isButtonDown(Gamepad::BUTTON_X))
            {
                _checked = !_checked;
                notifyListeners(Control::Listener::VALUE_CHANGED);   
            }
        }
        break;
    }

    return Button::gamepadEvent(evt, gamepad, analogIndex);
}

bool CheckBox::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (_state == ACTIVE && evt == Keyboard::KEY_RELEASE && key == Keyboard::KEY_RETURN)
    {
        _checked = !_checked;
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }

    return Button::keyEvent(evt, key);
}

void CheckBox::update(const Control* container, const Vector2& offset)
{
    Label::update(container, offset);

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

    Vector2 pos(_viewportBounds.x, _viewportBounds.y + _viewportBounds.height * 0.5f - size.y * 0.5f);

    spriteBatch->draw(pos.x, pos.y, size.x, size.y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
}

const char* CheckBox::getType() const
{
    return "checkBox";
}

}
