#include "Base.h"
#include "Gamepad.h"
#include "Joystick.h"
#include "Game.h"

namespace gameplay
{

Gamepad::Gamepad()
    : _playerIndex(-1), _joystickValues(NULL), _joystickCount(0), _buttonStates(NULL), _buttonCount(0), _gamepadForm(NULL)
{
}

Gamepad::Gamepad(const char* formPath)
    : _playerIndex(-1), _joystickValues(NULL), _joystickCount(0), _buttonStates(NULL), _buttonCount(0), _gamepadForm(NULL)
{
    GP_ASSERT(formPath);

    _gamepadForm = Form::create(formPath);
    GP_ASSERT(_gamepadForm);

    _gamepadForm->setConsumeInputEvents(false);

    getGamepadControls(_gamepadForm);

    if (_joystickCount > 0)
    {
        _joystickValues = new Vector2*[_joystickCount];
        for (unsigned int i = 0; i < _joystickCount; i++)
        {
            _joystickValues[i] = new Vector2();
        }
    }
    if (_buttonCount > 0)
    {
        _buttonStates = new ButtonState*[_buttonCount];
        for (unsigned int i = 0; i < _buttonCount; i++)
        {
            _buttonStates[i] = new ButtonState();
        }
    }
}

void Gamepad::getGamepadControls(Form* form)
{
    std::vector<Control*> controls = form->getControls();
    std::vector<Control*>::iterator itr = controls.begin();

    for (; itr != controls.end(); itr++)
    {
        Control* control = *itr;
        GP_ASSERT(control);

        if (control->isContainer())
        {
            getGamepadControls((Form*) control);
        }
        else if (std::strcmp("joystick", control->getType()) == 0)
        {
            control->addListener(this, Control::Listener::PRESS | Control::Listener::VALUE_CHANGED | Control::Listener::RELEASE);
            control->_data = (void*) new GamepadData(GamepadData::JOYSTICK, _joystickCount);
            _joystickCount++;
        }
        else if (std::strcmp("button", control->getType()) == 0)
        {
            control->addListener(this, Control::Listener::PRESS | Control::Listener::RELEASE);
            control->_data = (void*) new GamepadData(GamepadData::BUTTON, _buttonCount);
            _buttonCount++;
        }   
    }
}

Gamepad::~Gamepad()
{
    if (_joystickValues)
    {
        for (unsigned int i = 0; i < _joystickCount; i++)
            SAFE_DELETE(_joystickValues[i]);
        SAFE_DELETE_ARRAY(_joystickValues);
    }

    if (_buttonStates)
    {
        for (unsigned int i = 0; i < _buttonCount; i++)
           SAFE_DELETE(_buttonStates[i]);
        SAFE_DELETE_ARRAY(_buttonStates);
    }

    if (_gamepadForm)
        SAFE_RELEASE(_gamepadForm);
}

void Gamepad::update()
{
    if (_gamepadForm && _gamepadForm->isEnabled())
    {
        _gamepadForm->update();
    }
}

void Gamepad::render()
{
    if (_gamepadForm && _gamepadForm->isEnabled())
    {
        _gamepadForm->draw();
    }
}

void Gamepad::controlEvent(Control* control, Control::Listener::EventType evt)
{
    if (_gamepadForm && _gamepadForm->isEnabled() && control->_data)
    {
        switch(((GamepadData*)control->_data)->_controlType)
        {
            case GamepadData::JOYSTICK:
            {
                int joystickIndex = ((GamepadData*)control->_data)->_controlIndex;
                switch (evt)
                {
                    case Control::Listener::PRESS:
                    case Control::Listener::VALUE_CHANGED:
                        _joystickValues[joystickIndex]->set(((Joystick*)control)->getValue());
                        break;
                    case Control::Listener::RELEASE:
                        _joystickValues[joystickIndex]->set(0.0f, 0.0f);
                        break;
                }
                Game::getInstance()->gamepadEvent(JOYSTICK_EVENT, this, joystickIndex);
            }
            break;
            case GamepadData::BUTTON:
            {
                int buttonIndex = ((GamepadData*)control->_data)->_controlIndex;
                switch(evt)
                {
                    case Control::Listener::PRESS:
                        *_buttonStates[buttonIndex] = BUTTON_PRESSED;
                        break;
                    case Control::Listener::RELEASE:
                        *_buttonStates[buttonIndex] = BUTTON_RELEASED;
                        break;
                }
                Game::getInstance()->gamepadEvent(BUTTON_EVENT, this, buttonIndex);
            }
            break;
        }
    }
}

Gamepad::ButtonState Gamepad::getButtonState(unsigned int buttonId) const
{
    GP_ASSERT(buttonId < _buttonCount);

    return *_buttonStates[buttonId];
}

bool Gamepad::isJoystickActive(unsigned int joystickId) const
{
    GP_ASSERT(joystickId < _joystickCount);

    return !(_joystickValues[joystickId]->isZero());
}

const Vector2& Gamepad::getJoystickValue(unsigned int joystickId) const
{
    GP_ASSERT(joystickId < _joystickCount);

    return *_joystickValues[joystickId];
}

bool Gamepad::isVirtual() const
{
    return (_gamepadForm && _gamepadForm->isEnabled());
}

Form* Gamepad::getForm() const
{
    return _gamepadForm;
}

}
