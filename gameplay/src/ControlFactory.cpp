#include "Base.h"
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


namespace gameplay 
{

static ControlFactory* __controlFactory = NULL;


ControlFactory::ControlFactory() 
{
	registerStandardControls();
}

ControlFactory::ControlFactory(const ControlFactory& copy)
{
}

ControlFactory::~ControlFactory() 
{
}

ControlFactory* ControlFactory::getInstance() 
{
	if (__controlFactory == NULL)
		__controlFactory = new ControlFactory();
	return __controlFactory;
}

bool ControlFactory::registerCustomControl(const char* name, ControlActivator activator)
{
	if (_registeredControls.find(name) != _registeredControls.end())
		return false;

	_registeredControls[name] = activator;
	return true;
}

void ControlFactory::unregisterCustomControl(const char* controlName)
{
	std::map<std::string, ControlActivator>::iterator it;
	if ((it = _registeredControls.find(controlName)) != _registeredControls.end())
	{
		_registeredControls.erase(it);
	}
}

Control *ControlFactory::createControl(const char* controlName, Theme::Style* style, Properties* properties, Theme* theme)
{
	if (_registeredControls.find(controlName) == _registeredControls.end())
		return NULL;	

	return (*_registeredControls[controlName])(style, properties, theme);
}

void ControlFactory::registerStandardControls() 
{
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
