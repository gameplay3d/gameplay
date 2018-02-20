#include "Base.h"
#include "Input.h"
#include "Command.h"
#include "Platform.h"

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

std::unordered_map<std::string, const Input::Binding*> Input::inputBindings;
Input::Keyboard Input::keyboard;
Input::Mouse Input::mouse;
Input::Gamepad Input::gamepads[GP_GAMEPADS_MAX];

bool Input::isGamepadValid(Input::GamepadHandle gamepad)
{ 
	return UINT16_MAX != gamepad.idx; 
}

std::string Input::getKeyName(Input::Key key)
{
	GP_ASSERT(key < Input::Key::KEY_COUNT);
	return __keyNames[key];
}


char Input::keyToAscii(Input::Key key, uint8_t keyModifiers)
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

Input::Keyboard::Keyboard() : 
	ring(BX_COUNTOF(chrs) - 4)
{
}

void Input::Keyboard::reset()
{
	bx::memSet(keys, 0, sizeof(keys) );
	bx::memSet(once, 0xff, sizeof(once) );
}

uint32_t Input::Keyboard::encodeKeyState(uint8_t modifiers, bool down)
{
	uint32_t state = 0;
	state |= uint32_t(down ? modifiers : 0)<<16;
	state |= uint32_t(down)<<8;
	return state;
}

bool Input::Keyboard::decodeKeyState(uint32_t state, uint8_t& modifiers)
{
	modifiers = (state >> 16) & 0xff;
	return 0 != ( (state >> 8) & 0xff);
}

void Input::Keyboard::setKeyState(Input::Key key, uint8_t modifiers, bool down)
{
	keys[key] = encodeKeyState(modifiers, down);
	once[key] = false;
}

bool Input::Keyboard::getKeyState(Input::Key key, uint8_t* modifiers)
{
	uint8_t mods;
	modifiers = nullptr == modifiers ? &mods : modifiers;
	return decodeKeyState(keys[key], *modifiers);
}

uint8_t Input::Keyboard::getModifiersState()
{
	uint8_t modifiers = 0;
	for (uint32_t i = 0; i < Input::Key::KEY_COUNT; ++i)
	{
		modifiers |= (keys[i] >> 16) & 0xff;
	}
	return modifiers;
}

void  Input::Keyboard::pushChar(uint8_t len, const uint8_t ch[4])
{
	for (uint32_t i = ring.reserve(4); i < len; i = ring.reserve(4))
	{
		popChar();
	}
	bx::memCopy(&chrs[ring.m_current], ch, 4);
	ring.commit(4);
}

const uint8_t* Input::Keyboard::popChar()
{
	if (ring.available() > 0)
	{
		uint8_t* utf8 = &chrs[ring.m_read];
		ring.consume(4);
		return utf8;
	}
	return nullptr;
}

void Input::Keyboard::charFlush()
{
	ring.m_current = 0;
	ring.m_write   = 0;
	ring.m_read    = 0;
}

Input::Mouse::Mouse() 
	: width(GP_GRAPHICS_WIDTH), 
	  height(GP_GRAPHICS_HEIGHT), 
	  wheelDelta(120), 
	  lock(false)
{
}
void Input::Mouse::reset()
{
	if (lock)
	{
		normalized[0] = 0.0f;
		normalized[1] = 0.0f;
		normalized[2] = 0.0f;
	}
	bx::memSet(buttons, 0, sizeof(buttons) );
}

void  Input::Mouse::setResolution(uint16_t width, uint16_t height)
{
	this->width = width;
	this->height = height;
}

void Input::Mouse::setPosition(int32_t mx, int32_t my, int32_t mz)
{
	absolute[0] = mx;
	absolute[1] = my;
	absolute[2] = mz;
	normalized[0] = float(mx) / float(width);
	normalized[1] = float(my) / float(height);
	normalized[2] = float(mz) / float(wheelDelta);
}

void Input::Mouse::setButtonState(Input::MouseButton button, uint8_t state)
{
	this->buttons[button] = state;
}

Input::Gamepad::Gamepad()
{
	reset();
}

void Input::Gamepad::reset()
{
	bx::memSet(axis, 0, sizeof(axis) );
}

void Input::Gamepad::setAxis(Input::GamepadAxis axis, int32_t value)
{
	this->axis[axis] = value;
}

int32_t Input::Gamepad::getAxis(Input::GamepadAxis axis)
{
	return this->axis[axis];
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
	return mouse.lock;;
}

void Input::setMouseLocked(bool lock)
{
	if (mouse.lock != lock)
	{
		mouse.lock = lock;
		Platform::WindowHandle handle = { 0 };
		setMouseLock(handle, lock);
		if (lock)
		{
			mouse.normalized[0] = 0.0f;
			mouse.normalized[1] = 0.0f;
			mouse.normalized[2] = 0.0f;
		}
	}
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

void Input::addBindings(const char* name, const Binding* bindings)
{
	inputBindings.insert(std::make_pair(std::string(name), bindings) );
}

void Input::removeBindings(const char* name)
{
	std::unordered_map<std::string, const Binding*>::iterator it = inputBindings.find(std::string(name));
	if (it != inputBindings.end() )
	{
		inputBindings.erase(it);
	}
}

void Input::process()
{
	for (std::unordered_map<std::string, const Binding*>::const_iterator it = inputBindings.begin(); it != inputBindings.end(); ++it)
	{
		process(it->second);
	}
}

void Input::process(const Binding* bindings)
{
	for (const Binding* binding = bindings; binding->key != Input::Key::KEY_NONE; ++binding)
	{
		uint8_t modifiers;
		bool down = Input::keyboard.decodeKeyState(Input::keyboard.keys[binding->key], modifiers);

		if (binding->flags == 1)
		{
			if (down)
			{
				if (modifiers == binding->keyModifiers
				&&  !Input::keyboard.once[binding->key])
				{
					if (binding->fn == nullptr)
					{
						cmdExec((const char*)binding->userData);
					}
					else
					{
						binding->fn(binding->userData);
					}
					Input::keyboard.once[binding->key] = true;
				}
			}
			else
			{
				Input::keyboard.once[binding->key] = false;
			}
		}
		else
		{
			if (down && (modifiers == binding->keyModifiers))
			{
				if (binding->fn == nullptr)
				{
					cmdExec((const char*)binding->userData);
				}
				else
				{
					binding->fn(binding->userData);
				}
			}
		}
	}
}

}
