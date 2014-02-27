#include "Base.h"
#include "RadioButton.h"

namespace gameplay
{
static std::vector<RadioButton*> __radioButtons;

RadioButton::RadioButton() : _selected(false), _image(NULL)
{
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

RadioButton* RadioButton::create(const char* id, Theme::Style* style)
{
    RadioButton* rb = new RadioButton();
    rb->_id = id ? id : "";
    rb->initialize("RadioButton", style, NULL);

    __radioButtons.push_back(rb);

    return rb;
}

Control* RadioButton::create(Theme::Style* style, Properties* properties)
{
    RadioButton* rb = new RadioButton();
    rb->initialize("RadioButton", style, properties);

    __radioButtons.push_back(rb);

    return rb;
}

void RadioButton::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Button::initialize(typeName, style, properties);

    if (properties)
    {
        if (properties->getBool("selected"))
        {
            RadioButton::clearSelected(_groupId);
            _selected = true;
        }

        const char* groupId = properties->getString("group");
        if (groupId)
        {
            _groupId = groupId;
        }
    }
}

bool RadioButton::isSelected() const
{
    return _selected;
}

void RadioButton::setSelected(bool selected)
{
    if (selected)
        RadioButton::clearSelected(_groupId);

    if (selected != _selected)
    {
        _selected = selected;
        setDirty(DIRTY_STATE);
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }
}

void RadioButton::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to RadioButton.");
    }

    Control::addListener(listener, eventFlags);
}

void RadioButton::clearSelected(const std::string& groupId)
{
    std::vector<RadioButton*>::const_iterator it;
    for (it = __radioButtons.begin(); it < __radioButtons.end(); ++it)
    {
        RadioButton* radioButton = *it;
        GP_ASSERT(radioButton);
        if (groupId == radioButton->_groupId)
        {
            radioButton->setSelected(false);
        }
    }
}

bool RadioButton::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (getState() == ACTIVE && evt == Keyboard::KEY_RELEASE && key == Keyboard::KEY_RETURN && !_selected)
    {
        RadioButton::clearSelected(_groupId);
        _selected = true;
        notifyListeners(Control::Listener::VALUE_CHANGED);
    }

    return Button::keyEvent(evt, key);
}

void RadioButton::controlEvent(Control::Listener::EventType evt)
{
    Button::controlEvent(evt);

    switch (evt)
    {
    case Control::Listener::CLICK:
        if (!_selected)
        {
            RadioButton::clearSelected(_groupId);
            _selected = true;
            notifyListeners(Control::Listener::VALUE_CHANGED);
        }
        break;
    }
}

void RadioButton::updateState(State state)
{
    Label::updateState(state);

    _image = getImage(_selected ? "selected" : "unselected", state);
}

void RadioButton::updateBounds()
{
    Label::updateBounds();

    Vector2 size;
    if (_selected)
    {
        const Rectangle& selectedRegion = getImageRegion("selected", NORMAL);
        size.set(selectedRegion.width, selectedRegion.height);
    }
    else
    {
        const Rectangle& unselectedRegion = getImageRegion("unselected", NORMAL);
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

void RadioButton::updateAbsoluteBounds(const Vector2& offset)
{
    Label::updateAbsoluteBounds(offset);

    _textBounds.x += _bounds.height + 5;
}

unsigned int RadioButton::drawImages(Form* form, const Rectangle& clip)
{
    if (!_image)
        return 0;

    // Left, v-center.
    // TODO: Set an alignment for radio button images.   
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

const char* RadioButton::getType() const
{
    return "radioButton";
}

void RadioButton::setGroupId(const char* groupId)
{
    _groupId = groupId;
}

const char* RadioButton::getGroupId() const
{
    return _groupId.c_str();
}

}
