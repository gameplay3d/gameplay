#include "Base.h"
#include "Label.h"

namespace gameplay
{
    Label::Label() : _text(""), _font(NULL)
    {
    }

    Label::Label(const Label& copy)
    {
    }

    Label::~Label()
    {
    }

    Label* Label::create(Theme::Style* style, Properties* properties)
    {
        Label* label = new Label();
        label->initialize(style, properties);

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
        if ((eventFlags & Listener::TEXT_CHANGED) == Listener::TEXT_CHANGED)
        {
            GP_ERROR("TEXT_CHANGED event is not applicable to this control.");
        }
        if ((eventFlags & Listener::VALUE_CHANGED) == Listener::VALUE_CHANGED)
        {
            GP_ERROR("VALUE_CHANGED event is not applicable to this control.");
        }

        Control::addListener(listener, eventFlags);
    }
    
    void Label::setText(const char* text)
    {
        if (text)
        {
            _text = text;
        }
    }

    const char* Label::getText()
    {
        return _text.c_str();
    }

    void Label::update(const Rectangle& clip)
    {
        Control::update(clip);

        _font = getFont(_state);
        _textColor = getTextColor(_state);
        _textColor.w *= getOpacity(_state);
    }

    void Label::drawText(const Rectangle& clip)
    {
        if (_text.size() <= 0)
            return;

        // Draw the text.
        if (_font)
        {
            _font->drawText(_text.c_str(), _textBounds, _textColor, getFontSize(_state), getTextAlignment(_state), true, getTextRightToLeft(_state), &_clip);
        }

        _dirty = false;
    }
}