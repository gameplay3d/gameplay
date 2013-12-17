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
    CheckBox* cb = new CheckBox();
    cb->_id = id ? id : "";
    cb->initialize("CheckBox", style, NULL);
    return cb;
}

Control* CheckBox::create(Theme::Style* style, Properties* properties)
{
    CheckBox* cb = new CheckBox();
    cb->initialize("CheckBox", style, properties);
    return cb;
}

void CheckBox::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Button::initialize(typeName, style, properties);

    if (properties)
    {
        properties->getVector2("imageSize", &_imageSize);
        _checked = properties->getBool("checked");
    }
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
    _dirty = true;
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

bool CheckBox::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (getState() == ACTIVE && evt == Keyboard::KEY_RELEASE && key == Keyboard::KEY_RETURN)
    {
        setChecked( !_checked );
    }

    return Button::keyEvent(evt, key);
}

void CheckBox::controlEvent(Control::Listener::EventType evt)
{
    Button::controlEvent(evt);

    switch (evt)
    {
    case Control::Listener::CLICK:
        setChecked( !_checked );
        break;
    }
}

void CheckBox::update(const Control* container, const Vector2& offset)
{
    Label::update(container, offset);

    Control::State state = getState();

    Vector2 size;
    if (_imageSize.isZero())
    {
        if (_checked)
        {
            const Rectangle& selectedRegion = getImageRegion("checked", state);
            size.set(selectedRegion.width, selectedRegion.height);
        }
        else
        {
            const Rectangle& unselectedRegion = getImageRegion("unchecked", state);
            size.set(unselectedRegion.width, unselectedRegion.height);
        }
    }
    else
    {
        size.set(_imageSize);
    }
    
    if (_autoWidth == Control::AUTO_SIZE_FIT)
    {
        // Text-only width was already measured in Label::update - append image
        setWidth(size.x + _bounds.width + 5);
    }

    if (_autoHeight == Control::AUTO_SIZE_FIT)
    {
        // Text-only width was already measured in Label::update - append image
        setHeight(std::max(getHeight(), size.y));
    }

    _textBounds.x += size.x + 5;
    
    if (_checked)
    {
        _image = getImage("checked", state);
    }
    else
    {
        _image = getImage("unchecked", state);
    }
}

unsigned int CheckBox::drawImages(Form* form, const Rectangle& clip)
{
    if (!_image)
        return 0;

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

    SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
    startBatch(form, batch);
    batch->draw(pos.x, pos.y, size.x, size.y, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
    finishBatch(form, batch);

    return 1;
}

const char* CheckBox::getType() const
{
    return "checkBox";
}

}
