#include "Base.h"
#include "Label.h"

namespace gameplay
{

Label::Label() : _text(""), _font(NULL)
{
}

Label::~Label()
{
}

Label* Label::create(const char* id, Theme::Style* style)
{
    GP_ASSERT(style);

    Label* label = new Label();
    if (id)
        label->_id = id;
    label->setStyle(style);

    // Labels don't consume input events by default like other controls.
    label->_consumeInputEvents = false;

    // Ensure that labels cannot receive focus.
    label->_focusIndex = -2;

    return label;
}

Control* Label::create(Theme::Style* style, Properties* properties, Theme *theme)
{
    Label* label = new Label();
    label->initialize(style, properties);

	label->_consumeInputEvents = false;
    label->_focusIndex = -2;

    return label;
}

void Label::initialize(Theme::Style* style, Properties* properties)
{
    GP_ASSERT(properties);

    Control::initialize(style, properties);
    const char* text = properties->getString("text");
    if (text)
    {
        _text = text;
    }
}

void Label::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Control::Listener::TEXT_CHANGED) == Control::Listener::TEXT_CHANGED)
    {
        GP_ERROR("TEXT_CHANGED event is not applicable to this control.");
    }
    if ((eventFlags & Control::Listener::VALUE_CHANGED) == Control::Listener::VALUE_CHANGED)
    {
        GP_ERROR("VALUE_CHANGED event is not applicable to this control.");
    }

    Control::addListener(listener, eventFlags);
}
    
void Label::setText(const char* text)
{
    assert(text);

    if (strcmp(text, _text.c_str()) != 0)
    {
        _text = text;
        _dirty = true;
    }
}

const char* Label::getText()
{
    return _text.c_str();
}

void Label::update(const Control* container, const Vector2& offset)
{
    Control::update(container, offset);

    _textBounds.set((int)_viewportBounds.x, (int)_viewportBounds.y, _viewportBounds.width, _viewportBounds.height);

    _font = getFont(_state);
    _textColor = getTextColor(_state);
    _textColor.w *= _opacity;

    Font* font = getFont(_state);
    if ((_autoWidth == Control::AUTO_SIZE_FIT || _autoHeight == Control::AUTO_SIZE_FIT) && font)
    {
        unsigned int w, h;
        font->measureText(_text.c_str(), getFontSize(_state), &w, &h);
        if (_autoWidth == Control::AUTO_SIZE_FIT)
            setWidth(w + getBorder(_state).left + getBorder(_state).right + getPadding().left + getPadding().right);
        if (_autoHeight == Control::AUTO_SIZE_FIT)
            setHeight(h + getBorder(_state).top + getBorder(_state).bottom + getPadding().top + getPadding().bottom);
    }
}

void Label::drawText(const Rectangle& clip)
{
    if (_text.size() <= 0)
        return;

    // Draw the text.
    if (_font)
    {
        _font->start();
        _font->drawText(_text.c_str(), _textBounds, _textColor, getFontSize(_state), getTextAlignment(_state), true, getTextRightToLeft(_state), &_viewportClipBounds);
        _font->finish();
    }
}

const char* Label::getType() const
{
    return "label";
}

}
