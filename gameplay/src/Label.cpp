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

        // Draw the text.
        font->begin();
        font->drawText(_text.c_str(), _clip, overlay->getTextColor(), overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft());
        font->end();

        _dirty = false;
    }
}