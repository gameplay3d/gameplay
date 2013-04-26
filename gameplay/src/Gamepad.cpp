#include "Base.h"
#include "Gamepad.h"
#include "Game.h"
#include "Button.h"
#include "Platform.h"
#include "Form.h"
#include "Joystick.h"

namespace gameplay
{

static std::vector<Gamepad*> __gamepads;

Gamepad::Gamepad(const char* formPath)
    : _handle((GamepadHandle)INT_MAX), _buttonCount(0), _joystickCount(0), _triggerCount(0), _vendorId(0), _productId(0),
      _form(NULL), _buttons(0)
{
    GP_ASSERT(formPath);
    _form = Form::create(formPath);
    GP_ASSERT(_form);
    _form->setConsumeInputEvents(false);
    _form->_isGamepad = true;
    _vendorString = "None";
    _productString = "Virtual";

    for (int i = 0; i < 2; ++i)
    {
        _uiJoysticks[i] = NULL;
        _triggers[i] = 0.0f;
    }

    for (int i = 0; i < 20; ++i)
    {
        _uiButtons[i] = NULL;
    }

    bindGamepadControls(_form);
}

Gamepad::Gamepad(GamepadHandle handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                 unsigned int vendorId, unsigned int productId, const char* vendorString, const char* productString)
    : _handle(handle), _buttonCount(buttonCount), _joystickCount(joystickCount), _triggerCount(triggerCount),
      _vendorId(vendorId), _productId(productId), _form(NULL), _buttons(0)
{
    if (vendorString)
    {
        _vendorString = vendorString;
    }
    
    if (productString)
    {
        _productString = productString;
    }

    for (int i = 0; i < 2; ++i)
    {
        _triggers[i] = 0.0f;
    }
}

Gamepad::~Gamepad()
{
    if (_form)
    {
        SAFE_RELEASE(_form);
    }
}

Gamepad* Gamepad::add(GamepadHandle handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                      unsigned int vendorId, unsigned int productId, 
                      const char* vendorString, const char* productString)
{
    Gamepad* gamepad = new Gamepad(handle, buttonCount, joystickCount, triggerCount, vendorId, productId, vendorString, productString);

    __gamepads.push_back(gamepad);
    Game::getInstance()->gamepadEvent(CONNECTED_EVENT, gamepad);
    return gamepad;
}

Gamepad* Gamepad::add(const char* formPath)
{
    Gamepad* gamepad = new Gamepad(formPath);

    __gamepads.push_back(gamepad);
    Game::getInstance()->gamepadEvent(CONNECTED_EVENT, gamepad);
    return gamepad;
}

void Gamepad::remove(GamepadHandle handle)
{
    std::vector<Gamepad*>::iterator it = __gamepads.begin();
    do
    {
        Gamepad* gamepad = *it;
        if (gamepad->_handle == handle)
        {
            it = __gamepads.erase(it);
            Game::getInstance()->gamepadEvent(DISCONNECTED_EVENT, gamepad);
            SAFE_DELETE(gamepad);
        }
        else
        {
        	it++;
        }
    } while (it != __gamepads.end());
}

void Gamepad::remove(Gamepad* gamepad)
{
    std::vector<Gamepad*>::iterator it = __gamepads.begin();
    do
    {
        Gamepad* g = *it;
        if (g == gamepad)
        {
            it = __gamepads.erase(it);
            Game::getInstance()->gamepadEvent(DISCONNECTED_EVENT, g);
            SAFE_DELETE(gamepad);
        }
        else
        {
        	it++;
        }
    } while (it != __gamepads.end());
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
            Joystick* joystick = (Joystick*)control;
            joystick->setConsumeInputEvents(true);
            _uiJoysticks[joystick->getIndex()] = joystick;
            _joystickCount++;
        }
        else if (std::strcmp("button", control->getType()) == 0)
        {
            Button* button = (Button*)control;
            button->setConsumeInputEvents(true);
            _uiButtons[button->getDataBinding()] = button;
            _buttonCount++;
        }
    }
}

unsigned int Gamepad::getGamepadCount()
{
    return __gamepads.size();
}

Gamepad* Gamepad::getGamepad(unsigned int index, bool preferPhysical)
{
    unsigned int count = __gamepads.size();
    if (index >= count)
        return NULL;

    if (!preferPhysical)
        return __gamepads[index];

    // Virtual gamepads are guaranteed to come before physical gamepads in the vector.
    Gamepad* backupVirtual = NULL;
    if (index < count && __gamepads[index]->isVirtual())
    {
        backupVirtual = __gamepads[index];
    }

    for (unsigned int i = 0; i < count; ++i)
    {
        if (!__gamepads[i]->isVirtual())
        {
            // __gamepads[i] is the first physical gamepad 
            // and should be returned from getGamepad(0, true).
            if (index + i < count)
            {
                return __gamepads[index + i];
            }
        }
    }

    return backupVirtual;
}

Gamepad* Gamepad::getGamepad(GamepadHandle handle)
{
    unsigned int count = __gamepads.size();
    for (unsigned int i = 0; i < count; ++i)
    {
        if (__gamepads[i]->_handle == handle)
        {
            return __gamepads[i];
        }
    }
    return NULL;
}

Gamepad::ButtonMapping Gamepad::getButtonMappingFromString(const char* string)
{
    if (strcmp(string, "A") == 0 || strcmp(string, "BUTTON_A") == 0)
        return BUTTON_A;
    else if (strcmp(string, "B") == 0 || strcmp(string, "BUTTON_B") == 0)
        return BUTTON_B;
    else if (strcmp(string, "C") == 0 || strcmp(string, "BUTTON_C") == 0)
        return BUTTON_C;
    else if (strcmp(string, "X") == 0 || strcmp(string, "BUTTON_X") == 0)
        return BUTTON_X;
    else if (strcmp(string, "Y") == 0 || strcmp(string, "BUTTON_Y") == 0)
        return BUTTON_Y;
    else if (strcmp(string, "Z") == 0 || strcmp(string, "BUTTON_Z") == 0)
        return BUTTON_Z;
    else if (strcmp(string, "MENU1") == 0 || strcmp(string, "BUTTON_MENU1") == 0)
        return BUTTON_MENU1;
    else if (strcmp(string, "MENU2") == 0 || strcmp(string, "BUTTON_MENU2") == 0)
        return BUTTON_MENU2;
    else if (strcmp(string, "MENU3") == 0 || strcmp(string, "BUTTON_MENU3") == 0)
        return BUTTON_MENU3;
    else if (strcmp(string, "MENU4") == 0 || strcmp(string, "BUTTON_MENU4") == 0)
        return BUTTON_MENU4;
    else if (strcmp(string, "L1") == 0 || strcmp(string, "BUTTON_L1") == 0)
        return BUTTON_L1;
    else if (strcmp(string, "L2") == 0 || strcmp(string, "BUTTON_L2") == 0)
        return BUTTON_L2;
    else if (strcmp(string, "L3") == 0 || strcmp(string, "BUTTON_L3") == 0)
        return BUTTON_L3;
    else if (strcmp(string, "R1") == 0 || strcmp(string, "BUTTON_R1") == 0)
        return BUTTON_R1;
    else if (strcmp(string, "R2") == 0 || strcmp(string, "BUTTON_R2") == 0)
        return BUTTON_R2;
    else if (strcmp(string, "R3") == 0 || strcmp(string, "BUTTON_R3") == 0)
        return BUTTON_R3;
    else if (strcmp(string, "UP") == 0 || strcmp(string, "BUTTON_UP") == 0)
        return BUTTON_UP;
    else if (strcmp(string, "DOWN") == 0 || strcmp(string, "BUTTON_DOWN") == 0)
        return BUTTON_DOWN;
    else if (strcmp(string, "LEFT") == 0 || strcmp(string, "BUTTON_LEFT") == 0)
        return BUTTON_LEFT;
    else if (strcmp(string, "RIGHT") == 0 || strcmp(string, "BUTTON_RIGHT") == 0)
        return BUTTON_RIGHT;

    GP_WARN("Unknown GamepadButton string.");
    return BUTTON_A;
}

const unsigned int Gamepad::getVendorId() const
{
    return _vendorId;
}

const unsigned int Gamepad::getProductId() const
{
    return _productId;
}

const char* Gamepad::getVendorString() const
{
    return _vendorString.c_str();
}

const char* Gamepad::getProductString() const
{
    return _productString.c_str();
}

void Gamepad::update(float elapsedTime)
{
    if (!_form)
    {
        Platform::pollGamepadState(this);
    }
}

void Gamepad::updateInternal(float elapsedTime)
{
    unsigned int size = __gamepads.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        __gamepads[i]->update(elapsedTime);
    }
}

void Gamepad::draw()
{
    if (_form && _form->isEnabled())
    {
        _form->draw();
    }
}

unsigned int Gamepad::getButtonCount() const
{
    return _buttonCount;
}

bool Gamepad::isButtonDown(ButtonMapping mapping) const
{
    if (_form)
    {
        Button* button = _uiButtons[mapping];
        if (button)
        {
            return (button->getState() == Control::ACTIVE);
        }
        else
        {
            return false;
        }
    }
    else if (_buttons & (1 << mapping))
    {
        return true;
    }

    return false;
}

unsigned int Gamepad::getJoystickCount() const
{
    return _joystickCount;
}

void Gamepad::getJoystickValues(unsigned int joystickId, Vector2* outValue) const
{
    if (joystickId >= _joystickCount)
        return;

    if (_form)
    {
        Joystick* joystick = _uiJoysticks[joystickId];
        if (joystick)
        {
            const Vector2& value = joystick->getValue();
            outValue->set(value.x, value.y);
        }
        else
        {
            outValue->set(0.0f, 0.0f);
        }
    }
    else
    {
        outValue->set(_joysticks[joystickId]);
    }
}

unsigned int Gamepad::getTriggerCount() const
{
    return _triggerCount;
}

float Gamepad::getTriggerValue(unsigned int triggerId) const
{
    if (triggerId >= _triggerCount)
        return 0.0f;

    if (_form)
    {
        // Triggers are currently not available for virtual gamepads.
        return 0.0f;
    }
    else
    {
        return _triggers[triggerId];
    }
}

bool Gamepad::isVirtual() const
{
    return _form;
}

Form* Gamepad::getForm() const
{
    return _form;
}

void Gamepad::setButtons(unsigned int buttons)
{
    if (buttons != _buttons)
    {
        _buttons = buttons;
        Platform::gamepadEventInternal(BUTTON_EVENT, this);
    }
}

void Gamepad::setJoystickValue(unsigned int index, float x, float y)
{
    if (_joysticks[index].x != x || _joysticks[index].y != y)
    {
        _joysticks[index].set(x, y);
        Platform::gamepadEventInternal(JOYSTICK_EVENT, this, index);
    }
}

void Gamepad::setTriggerValue(unsigned int index, float value)
{
    if (_triggers[index] != value)
    {
        _triggers[index] = value;
        Platform::gamepadEventInternal(TRIGGER_EVENT, this, index);
    }
}

}
