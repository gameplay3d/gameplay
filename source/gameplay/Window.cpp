#include "Window.h"
#include "Windowing.h"
#include "WindowingGLFW.h"
#include "Monitor.h"
#include "Input.h"
#include "App.h"

namespace gameplay
{

Window::Window()
{
}

Window::~Window()
{
    if (handle->glfwWindow)
    {
        glfwDestroyWindow(handle->glfwWindow);
    }
}

void Window::set_title(const char* title)
{
    glfwSetWindowTitle(handle->glfwWindow, title);
}

void Window::set_size(const Int2& windowSize)
{
    glfwSetWindowSize(handle->glfwWindow, windowSize.x, windowSize.y);
}

Int2 Window::get_size() const
{
    Int2 size;
    glfwGetWindowSize(handle->glfwWindow, &size.x, &size.y);
    return size;
}

void Window::set_pos(const Int2& pos)
{
    glfwSetWindowPos(handle->glfwWindow, pos.x, pos.y);
}

Int2 Window::get_pos() const
{
   Int2 pos;
   glfwGetWindowSize(handle->glfwWindow, &pos.x, &pos.y);
   return pos;
}

void Window::set_fullscreen(bool fullscreen)
{
    if (handle->fullscreen == fullscreen)
    {
        return;
    }
    if (fullscreen)
    {
        glfwGetWindowPos(handle->glfwWindow, &handle->pos.x, &handle->pos.y);
        glfwGetWindowSize(handle->glfwWindow, &handle->size.x, &handle->size.y);
        auto monitor = App::get_app()->get_windowing()->get_monitor(this);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor->handle->glfwMonitor);
        glfwSetWindowMonitor(handle->glfwWindow, monitor->handle->glfwMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        glfwSetWindowSize(handle->glfwWindow, mode->width, mode->height);
        handle->fullscreen = true;
    }
    else
    {
        glfwSetWindowMonitor(handle->glfwWindow, nullptr, handle->pos.x, handle->pos.y,
                             handle->size.x, handle->size.y, GLFW_DONT_CARE);
        glfwSetWindowSize(handle->glfwWindow, handle->size.x, handle->size.y);
        handle->fullscreen = false;
    }
}

bool Window::is_fullscreen() const
{
    return handle->fullscreen;
}

void Window::get_frame_size(int* left, int* top, int* right, int* bottom) const
{
    glfwGetWindowFrameSize(handle->glfwWindow, left, top, right, bottom);
}

void Window::set_opacity(float opacity)
{
    glfwSetWindowOpacity(handle->glfwWindow, opacity);
}

float Window::get_opacity() const
{
    return glfwGetWindowOpacity(handle->glfwWindow);
}

void Window::set_icon(const Pixmap* images, size_t imageCount)
{
    glfwSetWindowIcon(handle->glfwWindow, (int)imageCount, (const GLFWimage*)images);
}

bool Window::should_close()
{
    return glfwWindowShouldClose(handle->glfwWindow);
}

void Window::close()
{
    glfwWindowShouldClose(handle->glfwWindow);
}

void Window::show()
{
    glfwShowWindow(handle->glfwWindow);
    glfwFocusWindow(handle->glfwWindow);
}

void Window::hide()
{
    glfwHideWindow(handle->glfwWindow);
}

bool Window::is_visible() const
{
    return glfwGetWindowAttrib(handle->glfwWindow, GLFW_VISIBLE) == GLFW_TRUE;
}

void Window::focus()
{
    glfwFocusWindow(handle->glfwWindow);
}

bool Window::is_focused() const
{
    return glfwGetWindowAttrib(handle->glfwWindow, GLFW_FOCUSED) == GLFW_TRUE;
}

void Window::minimize()
{
    glfwIconifyWindow(handle->glfwWindow);
}

bool Window::is_minimized() const
{
    return glfwGetWindowAttrib(handle->glfwWindow, GLFW_ICONIFIED) == GLFW_TRUE;
}

void Window::maximize()
{
    glfwMaximizeWindow(handle->glfwWindow);
}

bool Window::is_maximized() const
{
    return glfwGetWindowAttrib(handle->glfwWindow, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void Window::restore()
{
    glfwRestoreWindow(handle->glfwWindow);
}

Float2 Window::get_content_scale() const
{
    Float2 scale;
    glfwGetWindowContentScale(handle->glfwWindow, &scale.x, &scale.y);
    return scale;
}

void Window::set_cursor(Cursor* cursor)
{
    glfwSetCursor(handle->glfwWindow, cursor->glfwCursor);
}

void Window::set_cursor_pos(const Double2& pos)
{
    glfwSetCursorPos(handle->glfwWindow, pos.x, pos.y);
}

Double2 Window::get_cursor_pos() const
{
    Double2 pos;
    glfwGetCursorPos(handle->glfwWindow, &pos.x, &pos.y);
    return pos;
}

void Window::set_cursor_mode(CursorMode mode)
{
    int32_t glfwValue = 0;
    switch (mode)
    {
    case CursorMode::NORMAL:
        glfwValue = GLFW_CURSOR_NORMAL;
        break;
    case CursorMode::HIDDEN:
        glfwValue = GLFW_CURSOR_HIDDEN;
        break;
    case CursorMode::DISABLED:
        glfwValue = GLFW_CURSOR_DISABLED;
        break;
    }

    glfwSetInputMode(handle->glfwWindow, GLFW_CURSOR, glfwValue);
}

CursorMode Window::get_cursor_mode() const
{
    CursorMode mode = CursorMode::NORMAL;
    int32_t glfwModeValue = glfwGetInputMode(handle->glfwWindow, GLFW_CURSOR);
    switch (glfwModeValue)
    {
    case GLFW_CURSOR_NORMAL:
        mode = CursorMode::NORMAL;
        break;
    case GLFW_CURSOR_HIDDEN:
        mode = CursorMode::HIDDEN;
        break;
    case GLFW_CURSOR_DISABLED:
        mode = CursorMode::DISABLED;
        break;
    }
    return mode;
}

void Window::set_input_mode_enabled(gameplay::InputMode mode, bool enabled)
{
    int32_t setMode = 0;

    switch (mode)
    {
        case gameplay::InputMode::STICKY_KEYS:
        setMode = GLFW_STICKY_KEYS;
        break;
    case gameplay::InputMode::STICKY_MOUSE_BUTTONS:
        setMode = GLFW_STICKY_MOUSE_BUTTONS;
        break;
    case gameplay::InputMode::LOCK_KEY_MODS:
        setMode = GLFW_LOCK_KEY_MODS;
        break;
    case gameplay::InputMode::RAW_MOUSE_MOTION:
        setMode = GLFW_RAW_MOUSE_MOTION;
        break;
    default:
        GP_ASSERT(false);
    }
    glfwSetInputMode(handle->glfwWindow, setMode, enabled);
}

bool Window::is_input_mode_enabled(InputMode mode) const
{
    int32_t setMode = 0;
    switch (mode)
    {
    case gameplay::InputMode::STICKY_KEYS:
        setMode = GLFW_STICKY_KEYS;
        break;
    case gameplay::InputMode::STICKY_MOUSE_BUTTONS:
        setMode = GLFW_STICKY_MOUSE_BUTTONS;
        break;
    case gameplay::InputMode::LOCK_KEY_MODS:
        setMode = GLFW_LOCK_KEY_MODS;
        break;
    case gameplay::InputMode::RAW_MOUSE_MOTION:
        setMode = GLFW_RAW_MOUSE_MOTION;
        break;
    default:
        GP_ASSERT(false);
    }
    return glfwGetInputMode(handle->glfwWindow, setMode) == GLFW_TRUE;
}

ButtonAction Window::get_mouse_button_action(MouseButton button)
{
    int glfwAction = glfwGetMouseButton(handle->glfwWindow, GLFWUtils::to_glfw_button(button));
    switch(glfwAction)
    {
    case GLFW_PRESS:
        return ButtonAction::PRESS;
    case GLFW_RELEASE:
        return ButtonAction::RELEASE;
    }
    return ButtonAction::RELEASE;
}

KeyAction Window::get_key_action(Key key)
{
    int glfwAction = glfwGetKey(handle->glfwWindow, GLFWUtils::to_glfw_key(key));
    switch(glfwAction)
    {
    case GLFW_PRESS:
        return KeyAction::PRESS;
    case GLFW_RELEASE:
        return KeyAction::RELEASE;
    case GLFW_REPEAT:
        return KeyAction::REPEAT;
    }
    return KeyAction::RELEASE;
}

void Window::set_user_ptr(void* userPtr)
{
    handle->userPtr = userPtr;
}

void* Window::get_user_ptr() const
{
    return handle->userPtr;
}
}
