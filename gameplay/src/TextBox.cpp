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
        GP_ERROR("VALUE_CHANGED event is not applicable to TextBox.");
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
                        GP_ASSERT(font);
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
                        GP_ASSERT(font);
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
                        GP_ASSERT(font);
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
                        GP_ASSERT(font);
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
                        GP_ASSERT(font);
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
                GP_ASSERT(font);
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

                        // Get new location of caret.
                        font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex + 1,
                            textAlignment, true, rightToLeft);

                        if (key == ' ')
                        {
                            // If a space was entered, check that caret is still within bounds.
                            if (_caretLocation.x >= _clip.x + _clip.width ||
                                _caretLocation.y >= _clip.y + _clip.height)
                            {
                                // If not, undo the character insertion.
                                _text.erase(textIndex, 1);
                                font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex,
                                    textAlignment, true, rightToLeft);

                                // No need to check again.
                                break;
                            }
                        }

                        // Always check that the text still fits within the clip region.
                        Rectangle textBounds;
                        font->measureText(_text.c_str(), _clip, fontSize, &textBounds, textAlignment, true, true);
                        if (textBounds.x <= _clip.x || textBounds.y <= _clip.y ||
                            textBounds.width >= _clip.width || textBounds.height >= _clip.height)
                        {
                            // If not, undo the character insertion.
                            _text.erase(textIndex, 1);
                            font->getLocationAtIndex(_text.c_str(), _clip, fontSize, &_caretLocation, textIndex,
                                textAlignment, true, rightToLeft);

                            // TextBox is not dirty.
                            break;
                        }
                
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
    Label::update(clip);

    // Get index into string and cursor location from the last recorded touch location.
    if (_state == FOCUS)
    {
        Font* font = getFont(_state);
        GP_ASSERT(font);
        unsigned int fontSize = getFontSize(_state);
        Font::Justify textAlignment = getTextAlignment(_state);
        bool rightToLeft = getTextRightToLeft(_state);

        font->getIndexAtLocation(_text.c_str(), _clip, fontSize, _caretLocation, &_caretLocation,
            textAlignment, true, rightToLeft);
    }

    _fontSize = getFontSize(_state);
    _caretImage = getImage("textCaret", _state);
}

void TextBox::drawImages(SpriteBatch* spriteBatch, const Rectangle& clip)
{
    if (_state == FOCUS)
    {
        GP_ASSERT(_caretImage);

        // Draw the cursor at its current location.
        const Rectangle& region = _caretImage->getRegion();
        if (!region.isEmpty())
        {
            GP_ASSERT(spriteBatch);
            const Theme::UVs uvs = _caretImage->getUVs();
            Vector4 color = _caretImage->getColor();
            color.w *= _opacity;

            spriteBatch->draw(_caretLocation.x - (region.width / 2.0f), _caretLocation.y, region.width, _fontSize, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color);
        }
    }

    _dirty = false;
}

void TextBox::setCaretLocation(int x, int y)
{
    const Theme::Border& border = getBorder(_state);
    Theme::Padding padding = getPadding();

    _caretLocation.set(x - border.left - padding.left + _clip.x,
                       y - border.top - padding.top + _clip.y);
}

}