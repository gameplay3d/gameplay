#include "TextBox.h"
#include "Game.h"

namespace gameplay
{

TextBox::TextBox() : _lastKeypress(0)
{
}

TextBox::TextBox(const TextBox& copy)
{
}

TextBox::~TextBox()
{
}

TextBox* TextBox::create(Theme::Style* style, Properties* properties)
{
    TextBox* textBox = new TextBox();
    textBox->initialize(style, properties);

    return textBox;
}

int TextBox::getLastKeypress()
{
    return _lastKeypress;
}

void TextBox::addListener(Control::Listener* listener, int eventFlags)
{
    if ((eventFlags & Listener::VALUE_CHANGED) == Listener::VALUE_CHANGED)
    {
        assert("VALUE_CHANGED event is not applicable to TextBox.");
        eventFlags &= ~Listener::VALUE_CHANGED;
    }

    Control::addListener(listener, eventFlags);
}

bool TextBox::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{   
    if (!isEnabled())
    {
        return false;
    }

    switch (evt)
    {
    case Touch::TOUCH_PRESS: 
        if (_state == NORMAL)
        {
            _state = ACTIVE;
            Game::getInstance()->displayKeyboard(true);
            _dirty = true;
            return _consumeTouchEvents;
        }
        else if (!(x > 0 && x <= _clipBounds.width &&
                    y > 0 && y <= _clipBounds.height))
        {
            _state = NORMAL;
            Game::getInstance()->displayKeyboard(false);
            _dirty = true;
            return _consumeTouchEvents;
        }
        break;
    case Touch::TOUCH_MOVE:
        if (_state == FOCUS &&
            x > 0 && x <= _clipBounds.width &&
            y > 0 && y <= _clipBounds.height)
        {
            setCaretLocation(x, y);
            _dirty = true;
            return _consumeTouchEvents;
        }
        break;
    case Touch::TOUCH_RELEASE:
        if (x > 0 && x <= _clipBounds.width &&
            y > 0 && y <= _clipBounds.height)
        {
            setCaretLocation(x, y);
            _state = FOCUS;
            _dirty = true;
            return _consumeTouchEvents;
        }
        break;
    }

    return _consumeTouchEvents;
}

void TextBox::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (_state == FOCUS)
    {
        switch (evt)
        {
            case Keyboard::KEY_PRESS:
            {
                switch (key)
                {
                    case Keyboard::KEY_HOME:
                    {
                        // TODO: Move cursor to beginning of line.
                        // This only works for left alignment...
                        
                        //_caretLocation.x = _clip.x;
                        //_dirty = true;
                        break;
                    }
                    case Keyboard::KEY_END:
                    {
                        // TODO: Move cursor to end of line.
                        break;
                    }
                    case Keyboard::KEY_DELETE:
                    {
                        Font* font = getFont(_state);
                        unsigned int fontSize = getFontSize(_state);
                        Font::Justify textAlignment = getTextAlignment(_state);
                        bool rightToLeft = getTextRightToLeft(_state);

                        unsigned int textIndex = font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                            textAlignment, true, rightToLeft);
                        _text.erase(textIndex, 1);
                        font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex,
                            textAlignment, true, rightToLeft);
                        _dirty = true;
                        notifyListeners(Listener::TEXT_CHANGED);
                        break;
                    }
                    case Keyboard::KEY_LEFT_ARROW:
                    {
                        Font* font = getFont(_state);
                        unsigned int fontSize = getFontSize(_state);
                        Font::Justify textAlignment = getTextAlignment(_state);
                        bool rightToLeft = getTextRightToLeft(_state);

                        unsigned int textIndex = font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                            textAlignment, true, rightToLeft);
                        font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex - 1,
                            textAlignment, true, rightToLeft);
                        _dirty = true;
                        break;
                    }
                    case Keyboard::KEY_RIGHT_ARROW:
                    {
                        Font* font = getFont(_state);
                        unsigned int fontSize = getFontSize(_state);
                        Font::Justify textAlignment = getTextAlignment(_state);
                        bool rightToLeft = getTextRightToLeft(_state);

                        unsigned int textIndex = font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                            textAlignment, true, rightToLeft);
                        font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex + 1,
                            textAlignment, true, rightToLeft);
                        _dirty = true;
                        break;
                    }
                    case Keyboard::KEY_UP_ARROW:
                    {
                        Font* font = getFont(_state);
                        unsigned int fontSize = getFontSize(_state);
                        Font::Justify textAlignment = getTextAlignment(_state);
                        bool rightToLeft = getTextRightToLeft(_state);

                        _caretLocation.y -= fontSize;
                        font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                            textAlignment, true, rightToLeft);
                        _dirty = true;
                        break;
                    }
                    case Keyboard::KEY_DOWN_ARROW:
                    {
                        Font* font = getFont(_state);
                        unsigned int fontSize = getFontSize(_state);
                        Font::Justify textAlignment = getTextAlignment(_state);
                        bool rightToLeft = getTextRightToLeft(_state);

                        _caretLocation.y += fontSize;
                        font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                            textAlignment, true, rightToLeft);
                        _dirty = true;
                        break;
                    }
                }
                break;
            }

            case Keyboard::KEY_CHAR:
            {
                Font* font = getFont(_state);
                unsigned int fontSize = getFontSize(_state);
                Font::Justify textAlignment = getTextAlignment(_state);
                bool rightToLeft = getTextRightToLeft(_state);

                unsigned int textIndex = font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
                    textAlignment, true, rightToLeft);

                switch (key)
                {
                    case Keyboard::KEY_BACKSPACE:
                    {
                        if (textIndex > 0)
                        {
                            --textIndex;
                            _text.erase(textIndex, 1);
                            font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex,
                                textAlignment, true, rightToLeft);

                            _dirty = true;
                        }
                        break;
                    }
                    case Keyboard::KEY_RETURN:
                        // TODO: Handle line-break insertion correctly.
                        break;
                    default:
                    {
                        // Insert character into string.
                        _text.insert(textIndex, 1, (char)key);

                        // Get new location of cursor.
                        font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex + 1,
                            textAlignment, true, rightToLeft);
                
                        _dirty = true;
                        break;
                    }
            
                    break;
                }

                notifyListeners(Listener::TEXT_CHANGED);
            }
        }
    }

    _lastKeypress = key;
}

void TextBox::update(const Rectangle& clip)
{
    Control::update(clip);

    // Get index into string and cursor location from the last recorded touch location.
    if (_state == FOCUS)
    {
        Font* font = getFont(_state);
        unsigned int fontSize = getFontSize(_state);
        Font::Justify textAlignment = getTextAlignment(_state);
        bool rightToLeft = getTextRightToLeft(_state);

        font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
            textAlignment, true, rightToLeft);
    }
}

void TextBox::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    if (_state == FOCUS)
    {
        // Draw the cursor at its current location.
        const Rectangle& region = getImageRegion("textCaret", _state);
        if (!region.isEmpty())
        {
            const Vector4& color = getImageColor("textCaret", _state);
            const Theme::UVs uvs = getImageUVs("textCaret", _state);
            unsigned int fontSize = getFontSize(_state);

            spriteBatch->draw(_caretLocation.x - (region.width / 2.0f), _caretLocation.y, region.width, fontSize, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
        }
    }

    _dirty = false;
}

void TextBox::setCaretLocation(int x, int y)
{
    Theme::Border border = getBorder(_state);
    Theme::Padding padding = getPadding();

    _caretLocation.set(x - border.left - padding.left + _clip.x,
                       y - border.top - padding.top + _clip.y);
}

}