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
        if (_autoSize != AUTO_SIZE_NONE)
            setDirty(DIRTY_BOUNDS);
    }
}

const char* Label::getText()
{
    return _text.c_str();
}

void Label::updateBounds(const Vector2& offset)
{
    Control::updateBounds(offset);

    _textBounds.set((int)_viewportBounds.x, (int)_viewportBounds.y, _viewportBounds.width, _viewportBounds.height);

    Control::State state = getState();
    _font = getFont(state);
    _textColor = getTextColor(state);
    _textColor.w *= _opacity;

    Font* font = getFont(state);
    if (_autoSize != AUTO_SIZE_NONE && font)
    {
        unsigned int w, h;
        font->measureText(_text.c_str(), getFontSize(state), &w, &h);
        if (_autoSize & AUTO_SIZE_WIDTH)
        {
            setWidth(w + getBorder(state).left + getBorder(state).right + getPadding().left + getPadding().right);
            _autoSize = (AutoSize)(_autoSize | AUTO_SIZE_WIDTH);
        }
        if (_autoSize & AUTO_SIZE_HEIGHT)
        {
            setHeight(h + getBorder(state).top + getBorder(state).bottom + getPadding().top + getPadding().bottom);
            _autoSize = (AutoSize)(_autoSize | AUTO_SIZE_HEIGHT);
        }
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
