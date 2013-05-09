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

	void PasswordTextBox::insertChar(int index, char character) {
		_text.insert(index, 1, '*');
		_passwordText.insert(index, 1, character);
	}

	void PasswordTextBox::deleteChar(int index) {
		_text.erase(index, 1);
		_passwordText.erase(index, 1);
	}
}