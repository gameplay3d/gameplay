#pragma once

#include <bx/ringbuffer.h>

namespace gameplay
{

/**
 * Defines user input api.
 */
class Input
{
    friend class Platform;

public:

	struct GamepadHandle 
	{ 
		unsigned short idx; 
	};
	
	static bool isGamepadValid(GamepadHandle handle);

    /**
     * Defines input keys.
     */
    enum Key
    {
		KEY_NONE = 0,
		KEY_ESC,
		KEY_RETURN,
		KEY_TAB,
		KEY_SPACE,
		KEY_BACKSPACE,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HOME,
		KEY_END,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_PRINT,
		KEY_PLUS,
		KEY_MINUS,
		KEY_LEFT_BRACKET,
		KEY_RIGHT_BRACKET,
		KEY_SEMICOLON,
		KEY_QUOTE,
		KEY_COMMA,
		KEY_PERIOD,
		KEY_SLASH,
		KEY_BACKSLASH,
		KEY_TILDE,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_NUMPAD0,
		KEY_NUMPAD1,
		KEY_NUMPAD2,
		KEY_NUMPAD3,
		KEY_NUMPAD4,
		KEY_NUMPAD5,
		KEY_NUMPAD6,
		KEY_NUMPAD7,
		KEY_NUMPAD8,
		KEY_NUMPAD9,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_GAMEPAD_A,
		KEY_GAMEPAD_B,
		KEY_GAMEPAD_X,
		KEY_GAMEPAD_Y,
		KEY_GAMEPAD_THUMB_LEFT,
		KEY_GAMEPAD_THUMB_RIGHT,
		KEY_GAMEPAD_SHOULDER_LEFT,
		KEY_GAMEPAD_SHOULDER_RIGHT,
		KEY_GAMEPAD_UP,
		KEY_GAMEPAD_DOWN,
		KEY_GAMEPAD_LEFT,
		KEY_GAMEPAD_RIGHT,
		KEY_GAMEPAD_BACK,
		KEY_GAMEPAD_START,
		KEY_GAMEPAD_GUIDE,
		KEY_COUNT
	};

	/**
	 * Defines key modifiers
	 */
	enum KeyModifier
	{
		KEY_MODIFIER_NONE = 0,
		KEY_MODIFIER_LEFT_ALT = 0x01,
		KEY_MODIFIER_RIGHT_ALT = 0x02,
		KEY_MODIFIER_LEFT_CTRL   = 0x04,
		KEY_MODIFIER_RIGHT_CTRL  = 0x08,
		KEY_MODIFIER_LEFT_SHIFT  = 0x10,
		KEY_MODIFIER_RIGHT_SHIFT = 0x20,
		KEY_MODIFIER_LEFT_META   = 0x40,
		KEY_MODIFIER_RIGHT_META  = 0x80
	};

    /**
     * Defines input mouse buttons.
     */
    enum MouseButton
    {
	MOUSE_BUTTON_NONE,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
	MOUSE_BUTTON_COUNT
    };


    /**
     * Defines gamepad axis
     */
	enum GamepadAxis
	{
		GAMEPAD_AXIS_LEFT_X,
		GAMEPAD_AXIS_LEFT_Y,
		GAMEPAD_AXIS_LEFT_Z,
		GAMEPAD_AXIS_RIGHT_X,
		GAMEPAD_AXIS_RIGHT_Y,
		GAMEPAD_AXIS_RIGHT_Z,
		GAMEPAD_AXIS_COUNT
	};

	typedef void (*BindingFn)(const void* userData);

	struct Binding
	{
		void set(Input::Key key, uint8_t keyModifiers, uint8_t flags, BindingFn fn, const void* userData = nullptr)
		{
			this->key = key;
			this->keyModifiers = keyModifiers;
			this->flags = flags;
			this->fn = fn;
			this->userData = userData;
		}

		void end()
		{
			this->key = Input::Key::KEY_NONE;
			this->keyModifiers = Input::KeyModifier::KEY_MODIFIER_NONE;
			this->flags = 0;
			this->fn = nullptr;
			this->userData = nullptr;
		}

		Input::Key key;
		uint8_t keyModifiers;
		uint8_t flags;
		BindingFn fn;
		const void* userData;
	};

	struct Keyboard
	{
		Keyboard();
		static uint32_t encodeKeyState(uint8_t modifiers, bool down);
		static bool decodeKeyState(uint32_t state, uint8_t& modifiers);
		void reset();
		void setKeyState(Input::Key key, uint8_t modifiers, bool down);
		bool getKeyState(Input::Key key, uint8_t* modifiers);
		uint8_t getModifiersState();
		void pushChar(uint8_t len, const uint8_t ch[4]);
		const uint8_t* popChar();
		void charFlush();

		uint32_t keys[256];
		bool once[256];
		uint8_t chrs[256];
		bx::RingBufferControl ring;
	};

	struct Mouse
	{
		Mouse();
		void reset();
		void setResolution(uint16_t width, uint16_t height);
		void setPosition(int32_t mx, int32_t my, int32_t mz);
		void setButtonState(Input::MouseButton button, uint8_t state);

		int32_t absolute[3];
		float normalized[3];
		int32_t wheel;
		uint8_t buttons[Input::MouseButton::MOUSE_BUTTON_COUNT];
		uint16_t wheelDelta;
		uint16_t width;
		uint16_t height;
		bool lock;
	};

	struct Gamepad
	{
		Gamepad();
		void reset();
		void setAxis(Input::GamepadAxis axis, int32_t value);
		int32_t getAxis(Input::GamepadAxis axis);

		int32_t axis[Input::GamepadAxis::GAMEPAD_AXIS_COUNT];
	};

	static std::string getKeyName(Input::Key key);

	static char keyToAscii(Input::Key key, uint8_t keyModifiers);

    static bool isButtonHeld(std::string buttonName);

    static bool isButtonDown(std::string buttonName);

    static bool isButtonUp(std::string buttonName);

    static bool getAxis(std::string axisName);

    static bool isMouseButtonHeld(MouseButton button);

    static bool isMouseButtonDown(MouseButton button);

    static bool isMouseButtonUp(MouseButton button);

	static bool isMouseLocked();

	static void setMouseLocked(bool lock);

    static float getMouseScroll();

    static bool isKeyHeld(Input::Key key);

    static bool isKeyDown(Input::Key key);

    static bool isKeyUp(Input::Key key);
    
	static void process();

	static void addBindings(const char* name, const Binding* bindings);

	static void removeBindings(const char* name);

	static void process(const Binding* bindings);

	static std::unordered_map<std::string, const Binding*> inputBindings;
	static Keyboard keyboard;
	static Mouse mouse;
	static Gamepad gamepads[GP_GAMEPADS_MAX];
};

}
