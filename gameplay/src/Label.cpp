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
    Label* label = new Label();
    label->_id = id ? id : "";
    label->initialize("Label", style, NULL);
    return label;
}

Control* Label::create(Theme::Style* style, Properties* properties)
{
    Label* label = new Label();
	label->initialize("Label", style, properties);
    return label;
}

void Label::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Control::initialize(typeName, style, properties);

	if (properties)
	{
		const char* text = properties->getString("text");
		if (text)
		{
			_text = text;
		}
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

    Control::State state = getState();
    _font = getFont(state);
    _textColor = getTextColor(state);
    _textColor.w *= _opacity;

    Font* font = getFont(state);
    if ((_autoWidth == Control::AUTO_SIZE_FIT || _autoHeight == Control::AUTO_SIZE_FIT) && font)
    {
        unsigned int w, h;
        font->measureText(_text.c_str(), getFontSize(state), &w, &h);
        if (_autoWidth == Control::AUTO_SIZE_FIT)
            setWidth(w + getBorder(state).left + getBorder(state).right + getPadding().left + getPadding().right);
        if (_autoHeight == Control::AUTO_SIZE_FIT)
            setHeight(h + getBorder(state).top + getBorder(state).bottom + getPadding().top + getPadding().bottom);
    }
}

unsigned int Label::drawText(Form* form, const Rectangle& clip)
{
    // Draw the text.
    if (_text.size() > 0 && _font)
    {
        Control::State state = getState();
        unsigned int fontSize = getFontSize(state);

        SpriteBatch* batch = _font->getSpriteBatch(fontSize);
        startBatch(form, batch);
        _font->drawText(_text.c_str(), _textBounds, _textColor, fontSize, getTextAlignment(state), true, getTextRightToLeft(state), &_viewportClipBounds);
        finishBatch(form, batch);

        return 1;
    }

    return 0;
}

const char* Label::getType() const
{
    return "label";
}

}
