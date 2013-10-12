#include "ControlFactory.h"

#include "Label.h"
#include "Button.h"
#include "CheckBox.h"
#include "RadioButton.h"
#include "Container.h"
#include "Slider.h"
#include "TextBox.h"
#include "Joystick.h"
#include "ImageControl.h"

#include <iostream>

namespace gameplay {

	ControlFactory::ControlFactory() {
		registerStdControls();
	}

	ControlFactory::~ControlFactory() {}

	ControlFactory &ControlFactory::getInstance() {
		static ControlFactory instance;

		return instance;
	}

	bool ControlFactory::registerCustomControl(const std::string &name, ControlCreator controlCreator) {
		if (_registeredControls.find(name) != _registeredControls.end())
			return false;
		_registeredControls[name] = controlCreator;
	}

	void ControlFactory::unregisterCustomControl(const std::string &name) {
		std::map<std::string, ControlCreator>::iterator it;

		if ((it = _registeredControls.find(name)) != _registeredControls.end())
			_registeredControls.erase(it);
	}

	Control *ControlFactory::createControl(const std::string &name, Theme::Style *style, Properties *properties, Theme *theme) {
		if (_registeredControls.find(name) == _registeredControls.end())
			return NULL;	
		return (*_registeredControls[name])(style, properties, theme);
	}

	void ControlFactory::registerStdControls() {
		_registeredControls["LABEL"] = &Label::create;
		_registeredControls["BUTTON"] = &Button::create;
		_registeredControls["CHECKBOX"] = &CheckBox::create;
		_registeredControls["RADIOBUTTON"] = &RadioButton::create;
		_registeredControls["CONTAINER"] = &Container::create;
		_registeredControls["SLIDER"] = &Slider::create;
		_registeredControls["TEXTBOX"] = &TextBox::create;
		_registeredControls["JOYSTICK"] = &Joystick::create;
		_registeredControls["IMAGE"] = &ImageControl::create;
	}

}
