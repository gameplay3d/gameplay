#include "Input.h"
#include "App.h"
#include "Window.h"
#include "WindowingGLFW.h"

namespace gameplay
{

Signal<int32_t, GamepadChangeEvent> Input::on_gamepad_changed;

Input::Input()
{
}

Input::~Input()
{
}

void Input::set_cursor_pos(const Double2& pos)
{
    static auto window = App::get_app()->get_window();
    window->set_cursor_pos(pos);
}

Double2 Input::get_cursor_pos()
{
    static auto window = App::get_app()->get_window();
    return window->get_cursor_pos();
}

void Input::set_cursor_mode(CursorMode mode)
{
    static auto window = App::get_app()->get_window();
    window->set_cursor_mode(mode);
}

CursorMode Input::get_cursor_mode()
{
    static auto window = App::get_app()->get_window();
    return window->get_cursor_mode();
}

void Input::set_input_mode_enabled(InputMode mode, bool enabled)
{
    static auto window = App::get_app()->get_window();
    window->set_input_mode_enabled(mode, enabled);
}

bool Input::is_input_mode_enabled(InputMode mode)
{
    static auto window = App::get_app()->get_window();
    return window->is_input_mode_enabled(mode);
}

ButtonAction Input::get_mouse_button_action(MouseButton button)
{
    static auto window = App::get_app()->get_window();
    return window->get_mouse_button_action(button);
}

KeyAction Input::get_key_action(Key key)
{
    static auto window = App::get_app()->get_window();
    return window->get_key_action(key);
}

const char* Input::get_key_name(Key key)
{
    return glfwGetKeyName(GLFWUtils::to_glfw_key(key), 0);
}

bool Input::is_gamepad_connected(int32_t gamepadIndex)
{
    return glfwJoystickIsGamepad(gamepadIndex);
}

bool Input::has_gamepad_mappings_db(int32_t gamepadIndex)
{
    return glfwJoystickIsGamepad(gamepadIndex);
}

bool Input::set_gamepad_mappings_db(const char* mappingsDb)
{
    return (glfwUpdateGamepadMappings(mappingsDb) == GLFW_TRUE);
}

const char* Input::get_gamepad_name(int32_t gamepadIndex)
{
    if (glfwJoystickIsGamepad(gamepadIndex))
    {
        return glfwGetGamepadName(gamepadIndex);
    }
    return glfwGetJoystickName(gamepadIndex);
}

const char* Input::get_gamepad_guid(int32_t gamepadIndex)
{
    return glfwGetJoystickGUID(gamepadIndex);
}

void Input::get_gamepad_state(int32_t gamepadIndex, GamepadState* state)
{
    glfwGetGamepadState(gamepadIndex, (GLFWgamepadstate*)state);
}

void Input::set_gamepad_user_ptr(int32_t gamepadIndex, void* userPtr)
{
    glfwSetJoystickUserPointer(gamepadIndex, userPtr);
}

void* Input::get_gamepad_user_ptr(int32_t gamepadIndex)
{
    return glfwGetJoystickUserPointer(gamepadIndex);
}
}
GP_ASSERT_STRUCTS_MATCH(GLFWgamepadstate, gameplay::GamepadState);
