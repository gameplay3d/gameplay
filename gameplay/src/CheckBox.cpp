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
        setDirty(DIRTY_STATE);
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }
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

void CheckBox::updateState(State state)
{
    Label::updateState(state);

    _image = getImage(_checked ? "checked" : "unchecked", state);
}

void CheckBox::updateBounds()
{
    Label::updateBounds();

    Vector2 size;
    if (_checked)
    {
        const Rectangle& selectedRegion = getImageRegion("checked", NORMAL);
        size.set(selectedRegion.width, selectedRegion.height);
    }
    else
    {
        const Rectangle& unselectedRegion = getImageRegion("unchecked", NORMAL);
        size.set(unselectedRegion.width, unselectedRegion.height);
    }

    if (_autoSize & AUTO_SIZE_HEIGHT)
    {
        // Text-only width was already measured in Label::update - append image
        const Theme::Border& border = getBorder(NORMAL);
        const Theme::Border& padding = getPadding();
        setHeightInternal(std::max(_bounds.height, size.y + border.top + border.bottom + padding.top + padding.bottom));
    }

    if (_autoSize & AUTO_SIZE_WIDTH)
    {
        // Text-only width was already measured in Label::update - append image
        setWidthInternal(_bounds.height + 5 + _bounds.width);
    }
}

void CheckBox::updateAbsoluteBounds(const Vector2& offset)
{
    Label::updateAbsoluteBounds(offset);

    _textBounds.x += _bounds.height + 5;
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

    Vector2 pos(_viewportBounds.x, _viewportBounds.y);

    SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
    startBatch(form, batch);
    batch->draw(pos.x, pos.y, _viewportBounds.height, _viewportBounds.height, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
    finishBatch(form, batch);

    return 1;
}

const char* CheckBox::getType() const
{
    return "checkBox";
}

}
