#include "Base.h"
#include "Input.h"

namespace gameplay
{

static std::string __keyNames[] =
{
    "None",
    "Esc",
    "Return",
    "Tab",
    "Space",
    "Backspace",
    "Up",
    "Down",
    "Left",
    "Right",
    "Insert",
    "Delete",
    "Home",
    "End",
    "PageUp",
    "PageDown",
    "Print",
    "Plus",
    "Minus",
    "LeftBracket",
    "RightBracket",
    "Semicolon",
    "Quote",
    "Comma",
    "Period",
    "Slash",
    "Backslash",
    "Tilde",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "NumPad0",
    "NumPad1",
    "NumPad2",
    "NumPad3",
    "NumPad4",
    "NumPad5",
    "NumPad6",
    "NumPad7",
    "NumPad8",
    "NumPad9",
    "Key0",
    "Key1",
    "Key2",
    "Key3",
    "Key4",
    "Key5",
    "Key6",
    "Key7",
    "Key8",
    "Key9",
    "KeyA",
    "KeyB",
    "KeyC",
    "KeyD",
    "KeyE",
    "KeyF",
    "KeyG",
    "KeyH",
    "KeyI",
    "KeyJ",
    "KeyK",
    "KeyL",
    "KeyM",
    "KeyN",
    "KeyO",
    "KeyP",
    "KeyQ",
    "KeyR",
    "KeyS",
    "KeyT",
    "KeyU",
    "KeyV",
    "KeyW",
    "KeyX",
    "KeyY",
    "KeyZ",
    "GamepadA",
    "GamepadB",
    "GamepadX",
    "GamepadY",
    "GamepadThumbL",
    "GamepadThumbR",
    "GamepadShoulderL",
    "GamepadShoulderR",
    "GamepadUp",
    "GamepadDown",
    "GamepadLeft",
    "GamepadRight",
    "GamepadBack",
    "GamepadStart",
    "GamepadGuide",
};

std::string Input::getKeyName(Input::Key key)
{
    GP_ASSERT(key < Input::Key::KEY_COUNT);
    return __keyNames[key];
}

char Input::getKeyCode(Input::Key key, uint8_t keyModifiers)
{
    const bool isAscii = (Key::KEY_0 <= key && key <= Key::KEY_Z) || (Key::KEY_ESC  <= key && key <= Key::KEY_MINUS);
    if (!isAscii)
    {
        return '\0';
    }

    const bool isNumber = (Key::KEY_0 <= key && key <= Key::KEY_9);
    if (isNumber)
    {
        return '0' + char(key - Key::KEY_0);
    }

    const bool isChar = (Key::KEY_A <= key && key <= Key::KEY_Z);
    if (isChar)
    {
        enum { ShiftMask = Input::KeyModifier::KEY_MODIFIER_LEFT_SHIFT | KeyModifier::KEY_MODIFIER_RIGHT_SHIFT };

        const bool shift = !!(keyModifiers & ShiftMask);
        return (shift ? 'A' : 'a') + char(key - Key::KEY_A);
    }

    switch (key)
    {
    case Key::KEY_ESC:       
        return 0x1b;
    case Key::KEY_RETURN:    
        return '\n';
    case Key::KEY_TAB:       
        return '\t';
    case Key::KEY_SPACE:     
        return ' ';
    case Key::KEY_BACKSPACE: 
        return 0x08;
    case Key::KEY_PLUS:      
        return '+';
    case Key::KEY_MINUS:     
        return '-';
    default:             
        break;
    }
    return '\0';
}

bool Input::isKeyHeld(Input::Key key)
{
    return false;
}

bool Input::isKeyDown(Input::Key key)
{
    return false;
}

bool Input::isKeyUp(Input::Key key)
{
    return false;
}

bool Input::isButtonHeld(std::string buttonName)
{
    return false;
}

bool Input::isButtonDown(std::string buttonName)
{
    return false;
}

bool Input::isButtonUp(std::string buttonName)
{
    return false;
}

bool Input::getAxis(std::string axisName)
{
    return false;
}

bool Input::isMouseButtonHeld(MouseButton button)
{
    return false;
}

bool Input::isMouseButtonDown(MouseButton button)
{
    return false;
}

bool Input::isMouseButtonUp(MouseButton button)
{
    return false;
}

float Input::getMouseScroll()
{
    return 0.0f;
}

bool Input::isMouseLocked()
{
    return false;
}

void Input::setMouseLocked(bool lock)
{
}

Input::Input()
{
}

Input::~Input()
{
}

Input* Input::getInput()
{
    static Input input;
    return &input;
}

void Input::initialize()
{
}

void Input::update()
{
}

void Input::postGamepadConnectionEvent(uint32_t controllerIndex, bool connected)
{
}

void Input::postGamepadAxisEvent(uint32_t controllerIndex, Input::GamepadAxis axis, int value)
{
    print("%d %d", axis, value);
}

void Input::postMouseMotionEvent(int mx, int my, int mz)
{
}

void Input::postMousePressEvent(int mx, int my, int mz, Input::MouseButton button, bool down)
{
}

void Input::postKeyCharEvent(char chr)
{
}
    
void Input::postKeyPressEvent(Input::Key key, uint8_t keyModifiers, bool down)
{
}

}
