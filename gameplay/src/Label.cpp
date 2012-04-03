#include "Base.h"
#include "Label.h"

namespace gameplay
{
    Label::Label() : _text("")
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
        label->init(style, properties);

        return label;
    }

    void Label::init(Theme::Style* style, Properties* properties)
    {
        Control::init(style, properties);

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
            assert("TEXT_CHANGED event is not applicable to this control.");
            eventFlags &= ~Listener::TEXT_CHANGED;
        }

        if ((eventFlags & Listener::VALUE_CHANGED) == Listener::VALUE_CHANGED)
        {
            assert("VALUE_CHANGED event is not applicable to this control.");
            eventFlags &= ~Listener::VALUE_CHANGED;
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

    void Label::drawText(const Rectangle& clip)
    {
        if (_text.size() <= 0)
            return;

        // TODO: Batch all labels that use the same font.
        Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
        Font* font = overlay->getFont();

        Vector4 textColor = overlay->getTextColor();
        textColor.w *= overlay->getOpacity();

        // Draw the text.
        font->begin();
        font->drawText(_text.c_str(), _textBounds, textColor, overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft(), &_clip);
        font->end();

        _dirty = false;
    }
}