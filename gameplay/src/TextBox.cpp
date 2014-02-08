#include "TextBox.h"
#include "Game.h"

namespace gameplay
{

static bool space(char c)
{
    return isspace(c);
}

TextBox::TextBox() : _caretLocation(0), _lastKeypress(0), _fontSize(0), _caretImage(NULL), _passwordChar('*'), _inputMode(TEXT), _ctrlPressed(false)
{
    _canFocus = true;
}

TextBox::~TextBox()
{
}

TextBox* TextBox::create(const char* id, Theme::Style* style)
{
    TextBox* textBox = new TextBox();
    textBox->_id = id ? id : "";
    textBox->initialize("TextBox", style, NULL);
    return textBox;
}

Control* TextBox::create(Theme::Style* style, Properties* properties)
{
    TextBox* textBox = new TextBox();
    textBox->initialize("TextBox", style, properties);
    return textBox;
}

void TextBox::initialize(const char* typeName, Theme::Style* style, Properties* properties)
{
    Label::initialize(typeName, style, properties);

	if (properties)
	{
		_inputMode = getInputMode(properties->getString("inputMode"));
	}
}

int TextBox::getLastKeypress()
{
    return _lastKeypress;
}

unsigned int TextBox::getCaretLocation() const
{
    return _caretLocation;
}

void TextBox::setCaretLocation(unsigned int index)
{
    _caretLocation = index;
    if (_caretLocation > _text.length())
        _caretLocation = (unsigned int)_text.length();
}

bool TextBox::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
    State state = getState();

    switch (evt)
    {
    case Touch::TOUCH_PRESS: 
        if (state == ACTIVE)
        {
            setCaretLocation(x, y);
        }
        break;
    case Touch::TOUCH_MOVE:
        if (state == ACTIVE)
        {
            setCaretLocation(x, y);
        }
        break;
    }

    return Label::touchEvent(evt, x, y, contactIndex);
}

static bool isWhitespace(char c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return true;

    default:
        return false;
    }
}

static unsigned int findNextWord(const std::string& text, unsigned int from, bool backwards)
{
    int pos = (int)from;
    if (backwards)
    {
        if (pos > 0)
        {
            // Moving backwards: skip all consecutive whitespace characters
            while (pos > 0 && isWhitespace(text.at(pos-1)))
                --pos;
            // Now search back to the first whitespace character
            while (pos > 0 && !isWhitespace(text.at(pos-1)))
                --pos;
        }
    }
    else
    {
        const int len = (const int)text.length();
        if (pos < len)
        {
            // Moving forward: skip all consecutive non-whitespace characters
            ++pos;
            while (pos < len && !isWhitespace(text.at(pos)))
                ++pos;
            // Now search for the first non-whitespace character
            while (pos < len && isWhitespace(text.at(pos)))
                ++pos;
        }
    }

    return (unsigned int)pos;
}

bool TextBox::keyEvent(Keyboard::KeyEvent evt, int key)
{
    switch (evt)
    {
        case Keyboard::KEY_PRESS:
        {
            switch (key)
            {
                case Keyboard::KEY_CTRL:
                {
                    _ctrlPressed = true;
                    break;
                }
                case Keyboard::KEY_HOME:
                {
                    _caretLocation = 0;
                    break;
                }
                case Keyboard::KEY_END:
                {
                    _caretLocation = _text.length();
                    break;
                }
                case Keyboard::KEY_DELETE:
                {
                    if (_caretLocation < _text.length())
                    {
                        int newCaretLocation;
                        if (_ctrlPressed)
                        {
                            newCaretLocation = findNextWord(getDisplayedText(), _caretLocation, false);
                        }
                        else
                        {
                            newCaretLocation = _caretLocation + 1;
                        }
                        _text.erase(_caretLocation, newCaretLocation - _caretLocation);
                        notifyListeners(Control::Listener::TEXT_CHANGED);
                    }
                    break;
                }
                case Keyboard::KEY_TAB:
                {
                    // Allow tab to move the focus forward.
                    return false;
                }
                case Keyboard::KEY_LEFT_ARROW:
                {
                    if (_caretLocation > 0)
                    {
                        if (_ctrlPressed)
                        {
                            _caretLocation = findNextWord(getDisplayedText(), _caretLocation, true);
                        }
                        else
                        {
                            --_caretLocation;
                        }
                    }
                    break;
                }
                case Keyboard::KEY_RIGHT_ARROW:
                {
                    if (_caretLocation < _text.length())
                    {
                        if (_ctrlPressed)
                        {
                            _caretLocation = findNextWord(getDisplayedText(), _caretLocation, false);
                        }
                        else
                        {
                            ++_caretLocation;
                        }
                    }
                    break;
                }
                case Keyboard::KEY_UP_ARROW:
                {
                    // TODO: Support multiline
                    break;
                }
                case Keyboard::KEY_DOWN_ARROW:
                {
                    // TODO: Support multiline
                    break;
                }
                case Keyboard::KEY_BACKSPACE:
                {
                    if (_caretLocation > 0)
                    {
                        int newCaretLocation;
                        if (_ctrlPressed)
                        {
                            newCaretLocation = findNextWord(getDisplayedText(), _caretLocation, true);
                        }
                        else
                        {
                            newCaretLocation = _caretLocation - 1;
                        }
                        _text.erase(newCaretLocation, _caretLocation - newCaretLocation);
                        _caretLocation = newCaretLocation;
                        notifyListeners(Control::Listener::TEXT_CHANGED);
                    }
                    break;
                }
            }
            break;
        }

        case Keyboard::KEY_CHAR:
        {
            switch (key)
            {
                case Keyboard::KEY_RETURN:
                    // TODO: Support multi-line
                    break;
                case Keyboard::KEY_ESCAPE:
                    break;
                case Keyboard::KEY_BACKSPACE:
                    break;
                case Keyboard::KEY_TAB:
                    // Allow tab to move the focus forward.
                    return false;
                default:
                {
                    // Insert character into string, only if our font supports this character
                    if (_font && _font->isCharacterSupported(key))
                    {
                        if (_caretLocation <= _text.length())
                        {
                            _text.insert(_caretLocation, 1, (char)key);
                            ++_caretLocation;
                        }

                        notifyListeners(Control::Listener::TEXT_CHANGED);
                    }
                    break;
                }
            
                break;
            }
            break;
        }
        case Keyboard::KEY_RELEASE:
            switch (key)
            {
                case Keyboard::KEY_CTRL:
                {
                    _ctrlPressed = false;
                    break;
                }
            }
    }

    _lastKeypress = key;

    return Label::keyEvent(evt, key);
}

void TextBox::controlEvent(Control::Listener::EventType evt)
{
    Label::controlEvent(evt);

    switch (evt)
    {
    case Control::Listener::FOCUS_GAINED:
        Game::getInstance()->displayKeyboard(true);
        break;

    case Control::Listener::FOCUS_LOST:
        Game::getInstance()->displayKeyboard(false);
        break;
    }
}

void TextBox::updateState(State state)
{
    Label::updateState(state);

    _fontSize = getFontSize(state);
    _caretImage = getImage("textCaret", state);
}

unsigned int TextBox::drawImages(Form* form, const Rectangle& clip)
{
    Control::State state = getState();

    if (_caretImage && (state == ACTIVE || hasFocus()))
    {
        // Draw the cursor at its current location.
        const Rectangle& region = _caretImage->getRegion();
        if (!region.isEmpty())
        {
            const Theme::UVs& uvs = _caretImage->getUVs();
            Vector4 color = _caretImage->getColor();
            color.w *= _opacity;

            float caretWidth = region.width * _fontSize / region.height;

            Font* font = getFont(state);
            unsigned int fontSize = getFontSize(state);
            Vector2 point;
            font->getLocationAtIndex(getDisplayedText().c_str(), _textBounds, fontSize, &point, _caretLocation, 
                 getTextAlignment(state), true, getTextRightToLeft(state));

            SpriteBatch* batch = _style->getTheme()->getSpriteBatch();
            startBatch(form, batch);
            batch->draw(point.x - caretWidth * 0.5f, point.y, caretWidth, fontSize, uvs.u1, uvs.v1, uvs.u2, uvs.v2, color, _viewportClipBounds);
            finishBatch(form, batch);

            return 1;
        }
    }

    return 0;
}

unsigned int TextBox::drawText(Form* form, const Rectangle& clip)
{
    if (_text.size() <= 0)
        return 0;

    // Draw the text.
    if (_font)
    {
        Control::State state = getState();
        const std::string displayedText = getDisplayedText();
        unsigned int fontSize = getFontSize(state);

        SpriteBatch* batch = _font->getSpriteBatch(fontSize);
        startBatch(form, batch);
        _font->drawText(displayedText.c_str(), _textBounds, _textColor, fontSize, getTextAlignment(state), true, getTextRightToLeft(state), &_viewportClipBounds);
        finishBatch(form, batch);

        return 1;
    }

    return 0;
}

void TextBox::setCaretLocation(int x, int y)
{
    Control::State state = getState();

    Vector2 point(x + _absoluteBounds.x, y + _absoluteBounds.y);

    // Get index into string and cursor location from the latest touch location.
    Font* font = getFont(state);
    unsigned int fontSize = getFontSize(state);
    Font::Justify textAlignment = getTextAlignment(state);
    bool rightToLeft = getTextRightToLeft(state);
    const std::string displayedText = getDisplayedText();

    int index = font->getIndexAtLocation(displayedText.c_str(), _textBounds, fontSize, point, &point,
            textAlignment, true, rightToLeft);

    if (index == -1)
    {
        // Attempt to find the nearest valid caret location.
        Rectangle textBounds;
        font->measureText(displayedText.c_str(), _textBounds, fontSize, &textBounds, textAlignment, true, true);

        if (point.x > textBounds.x + textBounds.width &&
            point.y > textBounds.y + textBounds.height)
        {
            font->getLocationAtIndex(displayedText.c_str(), _textBounds, fontSize, &point, (unsigned int)_text.length(),
                textAlignment, true, rightToLeft);
            return;
        }

        if (point.x < textBounds.x)
        {
            point.x = textBounds.x;
        }
        else if (point.x > textBounds.x + textBounds.width)
        {
            point.x = textBounds.x + textBounds.width;
        }

        if (point.y < textBounds.y)
        {
            point.y = textBounds.y;
        }
        else if (point.y > textBounds.y + textBounds.height)
        {
            Font* font = getFont(state);
            GP_ASSERT(font);
            unsigned int fontSize = getFontSize(state);
            point.y = textBounds.y + textBounds.height - fontSize;
        }

        index = font->getIndexAtLocation(displayedText.c_str(), _textBounds, fontSize, point, &point,
            textAlignment, true, rightToLeft);
    }

    if (index != -1)
        _caretLocation = index;
}

void TextBox::getCaretLocation(Vector2* p)
{
    GP_ASSERT(p);

    State state = getState();
    getFont(state)->getLocationAtIndex(getDisplayedText().c_str(), _textBounds, getFontSize(state), p, _caretLocation, getTextAlignment(state), true, getTextRightToLeft(state));
}

const char* TextBox::getType() const
{
    return "textBox";
}

void TextBox::setPasswordChar(char character)
{
    _passwordChar = character;
}

char TextBox::getPasswordChar() const
{
    return _passwordChar;
}

void TextBox::setInputMode(InputMode inputMode)
{
    _inputMode = inputMode;
}

TextBox::InputMode TextBox::getInputMode() const
{
    return _inputMode;
}

TextBox::InputMode TextBox::getInputMode(const char* inputMode)
{
    if (!inputMode)
    {
        return TextBox::TEXT;
    }

    if (strcmp(inputMode, "TEXT") == 0)
    {
        return TextBox::TEXT;
    }
    else if (strcmp(inputMode, "PASSWORD") == 0)
    {
        return TextBox::PASSWORD;
    }
    else
    {
        GP_ERROR("Failed to get corresponding textbox inputmode for unsupported value '%s'.", inputMode);
    }

    // Default.
    return TextBox::TEXT;
}

std::string TextBox::getDisplayedText() const
{
    std::string displayedText;
    switch (_inputMode) {
        case PASSWORD:
            displayedText.insert((size_t)0, _text.length(), _passwordChar);
            break;

        case TEXT:
        default:
            displayedText = _text;
            break;
    }

    return displayedText;
}

}
