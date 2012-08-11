#include "Base.h"
#include "Gamepad.h"
#include "Game.h"

namespace gameplay
{

Gamepad::Gamepad(unsigned int handle, const char* formPath)
    : _id(""), _handle(handle), _buttonCount(0), _joystickCount(0), _triggerCount(0), _gamepadForm(NULL),
      _uiJoysticks(NULL), _uiButtons(NULL)
{
    GP_ASSERT(formPath);

    _gamepadForm = Form::create(formPath);
    GP_ASSERT(_gamepadForm);

    _id = _gamepadForm->getId();

    bindGamepadControls(_gamepadForm);
}

Gamepad::Gamepad(const char* id, unsigned int handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount)
    : _id(id), _handle(handle), _buttonCount(buttonCount), _joystickCount(joystickCount), _triggerCount(triggerCount),
      _gamepadForm(NULL), _uiJoysticks(NULL), _uiButtons(NULL)
{
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
            if (!_uiJoysticks)
                _uiJoysticks = new std::vector<Joystick*>;

            _uiJoysticks->push_back((Joystick*) control);
            _joystickCount++;
        }
        else if (std::strcmp("button", control->getType()) == 0)
        {
            control->addRef();
            if (!_uiButtons)
                _uiButtons = new std::vector<Button*>;

            _uiButtons->push_back((Button*) control);
            _buttonCount++;
        }   
    }
}

Gamepad::~Gamepad()
{
    if (_gamepadForm)
    {
        if (_uiJoysticks)
        {
            for (std::vector<Joystick*>::iterator itr = _uiJoysticks->begin(); itr != _uiJoysticks->end(); itr++)
            {
                SAFE_RELEASE((*itr));
            }
            _uiJoysticks->clear();
            SAFE_DELETE(_uiJoysticks);
        }

        if (_uiButtons)
        {
            for (std::vector<Button*>::iterator itr = _uiButtons->begin(); itr!= _uiButtons->end(); itr++)
            {
                SAFE_RELEASE((*itr));
            }
            _uiButtons->clear();
            SAFE_DELETE(_uiButtons);
        }
        
        SAFE_RELEASE(_gamepadForm);
    }
}

const char* Gamepad::getId() const
{
    return _id.c_str();
}

void Gamepad::update(float elapsedTime)
{
    if (_gamepadForm && _gamepadForm->isEnabled())
    {
        _gamepadForm->update(elapsedTime);
    }
    else
    {
        isConnected();
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
    return _buttonCount;
}

Gamepad::ButtonState Gamepad::getButtonState(unsigned int buttonId) const
{
    GP_ASSERT(buttonId < _buttonCount);

    if (_gamepadForm)
    {
        if (_uiButtons)
            return _uiButtons->at(buttonId)->getState() == Control::ACTIVE ? BUTTON_PRESSED : BUTTON_RELEASED;
        else
            return BUTTON_RELEASED;
    }
    else
        return Platform::getGamepadButtonState(_handle, buttonId) ? BUTTON_PRESSED : BUTTON_RELEASED;
}

unsigned int Gamepad::getJoystickCount() const
{
    return _joystickCount;
}

bool Gamepad::isJoystickActive(unsigned int joystickId) const
{
    GP_ASSERT(joystickId < _joystickCount);

    if (_gamepadForm)
    {
        if (_uiJoysticks)
            return !_uiJoysticks->at(joystickId)->getValue().isZero();
        else
            return false;
    }
    else
    {
        return Platform::isGamepadJoystickActive(_handle, joystickId);
    }
}

void Gamepad::getJoystickAxisValues(unsigned int joystickId, Vector2* outValue) const
{
    GP_ASSERT(joystickId < _joystickCount);

    if (_gamepadForm)
    {
        if (_uiJoysticks)
        {
            const Vector2& value = _uiJoysticks->at(joystickId)->getValue();
            outValue->set(value.x, value.y);
        }
        else
        {
            outValue->set(0.0f, 0.0f);
        }
    }
    else
    {
        Platform::getGamepadJoystickAxisValues(_handle, joystickId, outValue);
    }
}

float Gamepad::getJoystickAxisX(unsigned int joystickId) const
{
    return Platform::getGamepadJoystickAxisX(_handle, joystickId);
}

float Gamepad::getJoystickAxisY(unsigned int joystickId) const
{
    return Platform::getGamepadJoystickAxisY(_handle, joystickId);
}

bool Gamepad::isVirtual() const
{
    return _gamepadForm;
}

Form* Gamepad::getForm() const
{
    return _gamepadForm;
}

bool Gamepad::isConnected() const
{
    if (_gamepadForm)
    {
        return true;
    }
    else
    {
        return Platform::isGamepadConnected(_handle);
    }
}

}
