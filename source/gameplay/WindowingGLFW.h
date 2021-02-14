#pragma once

#include "Types.h"
#include "Input.h"
#include <vector>

#if GP_PLATFORM_WINDOWS
#   define GLFW_EXPOSE_NATIVE_WIN32
#elif GP_PLATFORM_LINUX
#   define GLFW_EXPOSE_NATIVE_X11
#endif
#if GP_PLATFORM_WINDOWS
#   define NOMINMAX
#endif
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace gameplay
{
class Windowing;

struct WindowHandle
{
    GLFWwindow* glfwWindow{nullptr};
    void* platformWindow{nullptr};
    void* platformDisplay{nullptr};
    bool fullscreen{false};
    void* userPtr{nullptr};
    Int2 pos{0, 0};
    Int2 size{0, 0};
    Float2 mouseScale{0.0f, 0.0f};
};

struct MonitorHandle
{
public:
    GLFWmonitor* glfwMonitor{nullptr};
    void* userPtr{nullptr};
};

struct Cursor
{
    GLFWcursor* glfwCursor{nullptr};
};

class GLFWUtils
{
public:

    static inline Key to_key(int glfwKey)
    {
        switch(glfwKey)
        {
        case GLFW_KEY_UNKNOWN:
            return Key::KEY_UNKNOWN;
        case GLFW_KEY_SPACE:
            return Key::KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:
            return Key::KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:
            return Key::KEY_COMMA;
        case GLFW_KEY_MINUS:
            return Key::KEY_MINUS;
        case GLFW_KEY_PERIOD:
            return Key::KEY_PERIOD;
        case GLFW_KEY_SLASH:
            return Key::KEY_SLASH;
        case GLFW_KEY_0:
            return Key::KEY_0;
        case GLFW_KEY_1:
            return Key::KEY_1;
        case GLFW_KEY_2:
            return Key::KEY_2;
        case GLFW_KEY_3:
            return Key::KEY_3;
        case GLFW_KEY_4:
            return Key::KEY_4;
        case GLFW_KEY_5:
            return Key::KEY_5;
        case GLFW_KEY_6:
            return Key::KEY_6;
        case GLFW_KEY_7:
            return Key::KEY_7;
        case GLFW_KEY_8:
            return Key::KEY_8;
        case GLFW_KEY_9:
            return Key::KEY_9;
        case GLFW_KEY_SEMICOLON:
            return Key::KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:
            return Key::KEY_EQUAL;
        case GLFW_KEY_A:
            return Key::KEY_A;
        case GLFW_KEY_B:
            return Key::KEY_B;
        case GLFW_KEY_C:
            return Key::KEY_C;
        case GLFW_KEY_D:
            return Key::KEY_D;
        case GLFW_KEY_E:
            return Key::KEY_E;
        case GLFW_KEY_F:
            return Key::KEY_F;
        case GLFW_KEY_G:
            return Key::KEY_G;
        case GLFW_KEY_H:
            return Key::KEY_H;
        case GLFW_KEY_I:
            return Key::KEY_I;
        case GLFW_KEY_J:
            return Key::KEY_J;
        case GLFW_KEY_K:
            return Key::KEY_K;
        case GLFW_KEY_L:
            return Key::KEY_L;
        case GLFW_KEY_M:
            return Key::KEY_M;
        case GLFW_KEY_N:
            return Key::KEY_N;
        case GLFW_KEY_O:
            return Key::KEY_O;
        case GLFW_KEY_P:
            return Key::KEY_P;
        case GLFW_KEY_Q:
            return Key::KEY_Q;
        case GLFW_KEY_R:
            return Key::KEY_R;
        case GLFW_KEY_S:
            return Key::KEY_S;
        case GLFW_KEY_T:
            return Key::KEY_T;
        case GLFW_KEY_U:
            return Key::KEY_U;
        case GLFW_KEY_V:
            return Key::KEY_V;
        case GLFW_KEY_W:
            return Key::KEY_W;
        case GLFW_KEY_X:
            return Key::KEY_X;
        case GLFW_KEY_Y:
            return Key::KEY_Y;
        case GLFW_KEY_Z:
            return Key::KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:
            return Key::KEY_LEFT_BRACKET;
        case GLFW_KEY_RIGHT_BRACKET:
            return Key::KEY_RIGHT_BRACKET;
        case GLFW_KEY_BACKSLASH:
            return Key::KEY_BACKSLASH;
        case GLFW_KEY_GRAVE_ACCENT:
            return Key::KEY_GRAVE_ACCENT;
        case GLFW_KEY_ESCAPE:
            return Key::KEY_ESCAPE;
        case GLFW_KEY_TAB:
            return Key::KEY_TAB;
        case GLFW_KEY_ENTER:
            return Key::KEY_ENTER;
        case GLFW_KEY_DELETE:
            return Key::KEY_DELETE;
        case GLFW_KEY_RIGHT:
            return Key::KEY_RIGHT;
        case GLFW_KEY_LEFT:
            return Key::KEY_LEFT;
        case GLFW_KEY_DOWN:
            return Key::KEY_DOWN;
        case GLFW_KEY_UP:
            return Key::KEY_UP;
        case GLFW_KEY_PAGE_UP:
            return Key::KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:
            return Key::KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:
            return Key::KEY_HOME;
        case GLFW_KEY_END:
            return Key::KEY_END;
        case GLFW_KEY_CAPS_LOCK:
            return Key::KEY_CAPS_LOCK;
        case GLFW_KEY_NUM_LOCK:
            return Key::KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:
            return Key::KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:
            return Key::KEY_PAUSE;
        case GLFW_KEY_F1:
            return Key::KEY_F1;
        case GLFW_KEY_F2:
            return Key::KEY_F2;
        case GLFW_KEY_F3:
            return Key::KEY_F3;
        case GLFW_KEY_F4:
            return Key::KEY_F4;
        case GLFW_KEY_F5:
            return Key::KEY_F5;
        case GLFW_KEY_F6:
            return Key::KEY_F6;
        case GLFW_KEY_F7:
            return Key::KEY_F7;
        case GLFW_KEY_F8:
            return Key::KEY_F8;
        case GLFW_KEY_F9:
            return Key::KEY_F9;
        case GLFW_KEY_F10:
            return Key::KEY_F10;
        case GLFW_KEY_F11:
            return Key::KEY_F11;
        case GLFW_KEY_F12:
            return Key::KEY_F12;
        case GLFW_KEY_KP_0:
            return Key::KEY_NUMPAD_0;
        case GLFW_KEY_KP_1:
            return Key::KEY_NUMPAD_1;
        case GLFW_KEY_KP_2:
            return Key::KEY_NUMPAD_2;
        case GLFW_KEY_KP_3:
            return Key::KEY_NUMPAD_3;
        case GLFW_KEY_KP_4:
            return Key::KEY_NUMPAD_4;
        case GLFW_KEY_KP_5:
            return Key::KEY_NUMPAD_5;
        case GLFW_KEY_KP_6:
            return Key::KEY_NUMPAD_6;
        case GLFW_KEY_KP_7:
            return Key::KEY_NUMPAD_7;
        case GLFW_KEY_KP_8:
            return Key::KEY_NUMPAD_8;
        case GLFW_KEY_KP_9:
            return Key::KEY_NUMPAD_9;
        case GLFW_KEY_KP_DECIMAL:
            return Key::KEY_NUMPAD_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:
            return Key::KEY_NUMPAD_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:
            return Key::KEY_NUMPAD_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:
            return Key::KEY_NUMPAD_SUBTRACT;
        case GLFW_KEY_KP_ADD:
            return Key::KEY_NUMPAD_ADD;
        case GLFW_KEY_KP_ENTER:
            return Key::KEY_NUMPAD_ENTER;
        case GLFW_KEY_KP_EQUAL:
            return Key::KEY_NUMPAD_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:
            return Key::KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return Key::KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:
            return Key::KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:
            return Key::KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:
            return Key::KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL:
            return Key::KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:
            return Key::KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:
            return Key::KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:
            return Key::KEY_MENU;
        }
        return Key::KEY_UNKNOWN;
    }

    static inline int to_glfw_key(Key key)
    {
        switch(key)
        {
        case Key::KEY_UNKNOWN:
            return GLFW_KEY_UNKNOWN;
        case Key::KEY_SPACE:
            return GLFW_KEY_SPACE;
        case Key::KEY_APOSTROPHE:
            return GLFW_KEY_APOSTROPHE;
        case Key::KEY_COMMA:
            return GLFW_KEY_COMMA;
        case Key::KEY_MINUS:
            return GLFW_KEY_MINUS;
        case Key::KEY_PERIOD:
            return GLFW_KEY_PERIOD;
        case Key::KEY_SLASH:
            return GLFW_KEY_SLASH;
        case Key::KEY_0:
            return GLFW_KEY_0;
        case Key::KEY_1:
            return GLFW_KEY_1;
        case Key::KEY_2:
            return GLFW_KEY_2;
        case Key::KEY_3:
            return GLFW_KEY_3;
        case Key::KEY_4:
            return GLFW_KEY_4;
        case Key::KEY_5:
            return GLFW_KEY_5;
        case Key::KEY_6:
            return GLFW_KEY_6;
        case Key::KEY_7:
            return GLFW_KEY_7;
        case Key::KEY_8:
            return GLFW_KEY_8;
        case Key::KEY_9:
            return GLFW_KEY_9;
        case Key::KEY_SEMICOLON:
            return GLFW_KEY_SEMICOLON;
        case Key::KEY_EQUAL:
            return GLFW_KEY_EQUAL;
        case Key::KEY_A:
            return GLFW_KEY_A;
        case Key::KEY_B:
            return GLFW_KEY_B;
        case Key::KEY_C:
            return GLFW_KEY_C;
        case Key::KEY_D:
            return GLFW_KEY_D;
        case Key::KEY_E:
            return GLFW_KEY_E;
        case Key::KEY_F:
            return GLFW_KEY_F;
        case Key::KEY_G:
            return GLFW_KEY_G;
        case Key::KEY_H:
            return GLFW_KEY_H;
        case Key::KEY_I:
            return GLFW_KEY_I;
        case Key::KEY_J:
            return GLFW_KEY_J;
        case Key::KEY_K:
            return GLFW_KEY_K;
        case Key::KEY_L:
            return GLFW_KEY_L;
        case Key::KEY_M:
            return GLFW_KEY_M;
        case Key::KEY_N:
            return GLFW_KEY_N;
        case Key::KEY_O:
            return GLFW_KEY_O;
        case Key::KEY_P:
            return GLFW_KEY_P;
        case Key::KEY_Q:
            return GLFW_KEY_Q;
        case Key::KEY_R:
            return GLFW_KEY_R;
        case Key::KEY_S:
            return GLFW_KEY_S;
        case Key::KEY_T:
            return GLFW_KEY_T;
        case Key::KEY_U:
            return GLFW_KEY_U;
        case Key::KEY_V:
            return GLFW_KEY_V;
        case Key::KEY_W:
            return GLFW_KEY_W;
        case Key::KEY_X:
            return GLFW_KEY_X;
        case Key::KEY_Y:
            return GLFW_KEY_Y;
        case Key::KEY_Z:
            return GLFW_KEY_Z;
        case Key::KEY_LEFT_BRACKET:
            return GLFW_KEY_LEFT_BRACKET;
        case Key::KEY_RIGHT_BRACKET:
            return GLFW_KEY_RIGHT_BRACKET;
        case Key::KEY_BACKSLASH:
            return GLFW_KEY_BACKSLASH;
        case Key::KEY_GRAVE_ACCENT:
            return GLFW_KEY_GRAVE_ACCENT;
        case Key::KEY_ESCAPE:
            return GLFW_KEY_ESCAPE;
        case Key::KEY_TAB:
            return GLFW_KEY_TAB;
        case Key::KEY_ENTER:
            return GLFW_KEY_ENTER;
        case Key::KEY_DELETE:
            return GLFW_KEY_DELETE;
        case Key::KEY_RIGHT:
            return GLFW_KEY_RIGHT;
        case Key::KEY_LEFT:
            return GLFW_KEY_LEFT;
        case Key::KEY_DOWN:
            return GLFW_KEY_DOWN;
        case Key::KEY_UP:
            return GLFW_KEY_UP;
        case Key::KEY_PAGE_UP:
            return GLFW_KEY_PAGE_UP;
        case  Key::KEY_PAGE_DOWN:
            return GLFW_KEY_PAGE_DOWN;
        case Key::KEY_HOME:
            return GLFW_KEY_HOME;
        case Key::KEY_END:
            return GLFW_KEY_END;
        case Key::KEY_CAPS_LOCK:
            return GLFW_KEY_CAPS_LOCK;
        case Key::KEY_NUM_LOCK:
            return GLFW_KEY_NUM_LOCK;
        case Key::KEY_PRINT_SCREEN:
            return GLFW_KEY_PRINT_SCREEN;
        case Key::KEY_PAUSE:
            return GLFW_KEY_PAUSE;
        case Key::KEY_F1:
            return GLFW_KEY_F1;
        case Key::KEY_F2:
            return GLFW_KEY_F2;
        case Key::KEY_F3:
            return GLFW_KEY_F3;
        case Key::KEY_F4:
            return GLFW_KEY_F4;
        case Key::KEY_F5:
            return GLFW_KEY_F5;
        case Key::KEY_F6:
            return GLFW_KEY_F6;
        case Key::KEY_F7:
            return GLFW_KEY_F7;
        case Key::KEY_F8:
            return GLFW_KEY_F8;
        case Key::KEY_F9:
            return GLFW_KEY_F9;
        case Key::KEY_F10:
            return GLFW_KEY_F10;
        case Key::KEY_F11:
            return GLFW_KEY_F11;
        case Key::KEY_F12:
            return GLFW_KEY_F12;
        case Key::KEY_NUMPAD_0:
            return GLFW_KEY_KP_0;
        case Key::KEY_NUMPAD_1:
            return GLFW_KEY_KP_1;
        case Key::KEY_NUMPAD_2:
            return GLFW_KEY_KP_2;
        case Key::KEY_NUMPAD_3:
            return GLFW_KEY_KP_3;
        case Key::KEY_NUMPAD_4:
            return GLFW_KEY_KP_4;
        case Key::KEY_NUMPAD_5:
            return GLFW_KEY_KP_5;
        case Key::KEY_NUMPAD_6:
            return GLFW_KEY_KP_6;
        case Key::KEY_NUMPAD_7:
            return GLFW_KEY_KP_7;
        case Key::KEY_NUMPAD_8:
            return GLFW_KEY_KP_8;
        case Key::KEY_NUMPAD_9:
            return GLFW_KEY_KP_9;
        case Key::KEY_NUMPAD_DECIMAL:
            return GLFW_KEY_KP_DECIMAL;
        case Key::KEY_NUMPAD_DIVIDE:
            return GLFW_KEY_KP_DIVIDE;
        case Key::KEY_NUMPAD_MULTIPLY:
            return GLFW_KEY_KP_MULTIPLY;
        case Key::KEY_NUMPAD_SUBTRACT:
            return GLFW_KEY_KP_SUBTRACT;
        case Key::KEY_NUMPAD_ADD:
            return GLFW_KEY_KP_ADD;
        case Key::KEY_NUMPAD_ENTER:
            return GLFW_KEY_KP_ENTER;
        case Key::KEY_NUMPAD_EQUAL:
            return GLFW_KEY_KP_EQUAL;
        case Key::KEY_LEFT_SHIFT:
            return GLFW_KEY_LEFT_SHIFT;
        case Key::KEY_LEFT_CONTROL:
            return GLFW_KEY_LEFT_CONTROL;
        case Key::KEY_LEFT_ALT:
            return GLFW_KEY_LEFT_ALT;
        case Key::KEY_LEFT_SUPER:
            return GLFW_KEY_LEFT_SUPER;
        case Key::KEY_RIGHT_SHIFT:
            return GLFW_KEY_RIGHT_SHIFT;
        case Key::KEY_RIGHT_CONTROL:
            return GLFW_KEY_RIGHT_CONTROL;
        case Key::KEY_RIGHT_ALT:
            return GLFW_KEY_RIGHT_ALT;
        case Key::KEY_RIGHT_SUPER:
            return GLFW_KEY_RIGHT_SUPER;
        case Key::KEY_MENU:
            return GLFW_KEY_MENU;
        }
        return GLFW_KEY_UNKNOWN;
    }

    static inline int to_glfw_button(MouseButton button)
    {
        switch(button)
        {
        case MouseButton::LEFT:
            return GLFW_MOUSE_BUTTON_LEFT;
        case MouseButton::RIGHT:
            return GLFW_MOUSE_BUTTON_RIGHT;
        case MouseButton::MIDDLE:
            return GLFW_MOUSE_BUTTON_MIDDLE;
        case MouseButton::X1:
            return GLFW_MOUSE_BUTTON_4;
        case MouseButton::X2:
            return GLFW_MOUSE_BUTTON_5;
        }
        return GLFW_MOUSE_BUTTON_LEFT;
    }


    static inline KeyModFlags to_key_mod_flags(int glfwMods)
    {
        KeyModFlags flags = KEY_MOD_FLAGS_NONE;
        if (glfwMods & GLFW_MOD_SHIFT)
            flags |= KEY_MOD_FLAGS_SHIFT;
        if (glfwMods & GLFW_MOD_CONTROL)
            flags |= KEY_MOD_FLAGS_CONTROL;
        if (glfwMods & GLFW_MOD_ALT)
            flags |= KEY_MOD_FLAGS_ALT;
        if (glfwMods & GLFW_MOD_SUPER)
            flags |= KEY_MOD_FLAGS_SUPER;
        if (glfwMods & GLFW_MOD_CAPS_LOCK)
            flags |= KEY_MOD_FLAGS_CAPS_LOCK;
        if (glfwMods & GLFW_MOD_NUM_LOCK)
            flags |= KEY_MOD_FLAGS_NUM_LOCK;
        return flags;
    }
};
}
