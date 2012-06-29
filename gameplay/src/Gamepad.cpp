#include "Base.h"
#include "Gamepad.h"
#include "Game.h"

namespace gameplay
{

Gamepad::Gamepad(const char* id)
    : _id(id), _gamepadForm(NULL)
{
}

Gamepad::Gamepad(const char* id, const char* formPath)
    : _id(id), _gamepadForm(NULL)
{
    GP_ASSERT(formPath);

    _gamepadForm = Form::create(formPath);
    GP_ASSERT(_gamepadForm);
    
    _gamepadForm->setConsumeInputEvents(false);

    bindGamepadControls(_gamepadForm);
}

void Gamepad::bindGamepadControls(Container* container)
{
    std::vector<Control*> controls = container->getControls();
    std::vector<Control*>::iterator itr = controls.begin();

    for (; itr != controls.end(); itr++)
    {
        Control* control = *itr;
        GP_ASSERT(control);

        if (control->isContainer())
        {
            bindGamepadControls((Container*) control);
        }
        else if (std::strcmp("joystick", control->getType()) == 0)
        {
            control->addRef();
            _joysticks.push_back((Joystick*) control);
        }
        else if (std::strcmp("button", control->getType()) == 0)
        {
            control->addRef();
            _buttons.push_back((Button*) control);
        }   
    }
}

Gamepad::~Gamepad()
{
    for (std::vector<Joystick*>::iterator itr = _joysticks.begin(); itr != _joysticks.end(); itr++)
    {
        SAFE_RELEASE((*itr));
    }

    for (std::vector<Button*>::iterator itr = _buttons.begin(); itr!= _buttons.end(); itr++)
    {
        SAFE_RELEASE((*itr));
    }

    if (_gamepadForm)
        SAFE_RELEASE(_gamepadForm);
}

const char* Gamepad::getId() const
{
    return _id.c_str();
}

void Gamepad::update()
{
    if (_gamepadForm && _gamepadForm->isEnabled())
    {
        _gamepadForm->update();
    }
}

void Gamepad::draw()
{
    if (_gamepadForm && _gamepadForm->isEnabled())
    {
        _gamepadForm->draw();
    }
}

unsigned int Gamepad::getButtonCount() const
{
    return _buttons.size();
}

Gamepad::ButtonState Gamepad::getButtonState(unsigned int buttonId) const
{
    GP_ASSERT(buttonId < _buttons.size());

    return _buttons[buttonId]->getState() == Control::ACTIVE ? BUTTON_PRESSED : BUTTON_RELEASED;
}

unsigned int Gamepad::getJoystickCount() const
{
    return _joysticks.size();
}

bool Gamepad::isJoystickActive(unsigned int joystickId) const
{
    GP_ASSERT(joystickId < _joysticks.size());

    return !_joysticks[joystickId]->getValue().isZero();
}

const Vector2& Gamepad::getJoystickValue(unsigned int joystickId) const
{
    GP_ASSERT(joystickId < _joysticks.size());

    return _joysticks[joystickId]->getValue();
}

bool Gamepad::isVirtual() const
{
    return true;
}

Form* Gamepad::getForm() const
{
    return _gamepadForm;
}

}
