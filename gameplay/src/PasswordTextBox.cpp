#include "PasswordTextBox.h"

namespace gameplay
{
	PasswordTextBox::PasswordTextBox():_passwordText(""){
	}

	PasswordTextBox::~PasswordTextBox() {
	}

	PasswordTextBox* PasswordTextBox::create(const char* id, Theme::Style* style) {
		GP_ASSERT(style);

		PasswordTextBox* passwordTextBox = new PasswordTextBox();
		if (id)
			passwordTextBox->_id = id;
		passwordTextBox->setStyle(style);

		return passwordTextBox;
	}

	PasswordTextBox* PasswordTextBox::create(Theme::Style* style, Properties* properties) {
		PasswordTextBox* passwordTextBox = new PasswordTextBox();
		passwordTextBox->Control::initialize(style, properties);

		return passwordTextBox;
	}

	const char* PasswordTextBox::getText() {
		return _passwordText.c_str();
	}

	const char* PasswordTextBox::getType() const {
		return "passwordTextBox";
	}

	bool PasswordTextBox::keyEvent(Keyboard::KeyEvent evt, int key) {
		if (_state == FOCUS) {
			switch (evt) {
			case Keyboard::KEY_PRESS:
				{
					switch (key)
					{
					case Keyboard::KEY_HOME:
						{
							// TODO: Move cursor to beginning of line.
							// This only works for left alignment...

							//_caretLocation.x = _viewportClipBounds.x;
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

							int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
								textAlignment, true, rightToLeft);

							_text.erase(textIndex, 1);
							_passwordText.erase(textIndex, 1);

							font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex,
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

							int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
								textAlignment, true, rightToLeft);
							font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex - 1,
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

							int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
								textAlignment, true, rightToLeft);
							font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex + 1,
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
							_prevCaretLocation.set(_caretLocation);
							_caretLocation.y -= fontSize;
							int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
								textAlignment, true, rightToLeft);
							if (textIndex == -1)
							{
								_caretLocation.set(_prevCaretLocation);
							}

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
							_prevCaretLocation.set(_caretLocation);
							_caretLocation.y += fontSize;
							int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
								textAlignment, true, rightToLeft);
							if (textIndex == -1)
							{
								_caretLocation.set(_prevCaretLocation);
							}

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

					int textIndex = font->getIndexAtLocation(_text.c_str(), _textBounds, fontSize, _caretLocation, &_caretLocation,
						textAlignment, true, rightToLeft);
					if (textIndex == -1)
					{
						textIndex = 0;
						font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, 0,
							textAlignment, true, rightToLeft);
					}

					switch (key)
					{
					case Keyboard::KEY_BACKSPACE:
						{
							if (textIndex > 0)
							{
								--textIndex;

								_text.erase(textIndex, 1);
								_passwordText.erase(textIndex, 1);

								font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex,
									textAlignment, true, rightToLeft);

								_dirty = true;
							}
							break;
						}
					case Keyboard::KEY_RETURN:
						// TODO: Handle line-break insertion correctly.
						break;
					case Keyboard::KEY_ESCAPE:
						break;
					case Keyboard::KEY_TAB:
						break;
					default:
						{
							// Insert character into string.
							_text.insert(textIndex, 1, '*');
							_passwordText.insert(textIndex, 1, (char)key);

							// Get new location of caret.
							font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex + 1,
								textAlignment, true, rightToLeft);

							if (key == ' ')
							{
								// If a space was entered, check that caret is still within bounds.
								if (_caretLocation.x >= _textBounds.x + _textBounds.width ||
									_caretLocation.y >= _textBounds.y + _textBounds.height)
								{
									// If not, undo the character insertion.
									_text.erase(textIndex, 1);
									_passwordText.erase(textIndex, 1);

									font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex,
										textAlignment, true, rightToLeft);

									// No need to check again.
									break;
								}
							}

							// Always check that the text still fits within the clip region.
							Rectangle textBounds;
							font->measureText(_text.c_str(), _textBounds, fontSize, &textBounds, textAlignment, true, true);
							if (textBounds.x < _textBounds.x || textBounds.y < _textBounds.y ||
								textBounds.width >= _textBounds.width || textBounds.height >= _textBounds.height)
							{
								// If not, undo the character insertion.
								_text.erase(textIndex, 1);
								_passwordText.erase(textIndex, 1);

								font->getLocationAtIndex(_text.c_str(), _textBounds, fontSize, &_caretLocation, textIndex,
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
					break;
				}
			}
		}

		_lastKeypress = key;

		return _consumeInputEvents;
	} 
}
