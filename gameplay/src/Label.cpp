#include "Base.h"
#include "Label.h"

namespace gameplay
{
    static std::vector<Label*> __labels;

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
        label->_style = style;
        properties->getVector2("position", &label->_position);
        properties->getVector2("size", &label->_size);

        const char* id = properties->getId();
        if (id)
        {
            label->_id = id;
        }

        const char* text = properties->getString("text");
        if (text)
        {
            label->_text = text;
        }

        __labels.push_back(label);

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

    Label* Label::getLabel(const char* id)
    {
        std::vector<Label*>::const_iterator it;
        for (it = __labels.begin(); it < __labels.end(); it++)
        {
            Label* l = *it;
            if (strcmp(id, l->getID()) == 0)
            {
                return l;
            }
        }

        return NULL;
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

    void Label::update(const Vector2& position)
    {
        Vector2 pos(position.x + _position.x, position.y + _position.y);
        Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
        
        Theme::Border border;
        Theme::ContainerRegion* containerRegion = overlay->getContainerRegion();
        if (containerRegion)
        {
            border = overlay->getContainerRegion()->getBorder();
        }
        Theme::Padding padding = _style->getPadding();

        // Set up the text viewport.
        Font* font = overlay->getFont();
        _viewport.set(pos.x + border.left + padding.left,
                      pos.y + border.top + padding.top,
                      _size.x - border.left - padding.left - border.right - padding.right,
                      _size.y - border.top - padding.top - border.bottom - padding.bottom - overlay->getFontSize());
    }

    void Label::drawText(const Vector2& position)
    {
        if (_text.size() <= 0)
            return;

        // TODO: Batch all labels that use the same font.
        Theme::Style::Overlay* overlay = _style->getOverlay(getOverlayType());
        Font* font = overlay->getFont();

        // Draw the text.
        font->begin();
        font->drawText(_text.c_str(), _viewport, overlay->getTextColor(), overlay->getFontSize(), overlay->getTextAlignment(), true, overlay->getTextRightToLeft());
        font->end();

        _dirty = false;
    }
}