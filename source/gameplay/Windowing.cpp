#include "Windowing.h"
#include "WindowingGLFW.h"
#include "Window.h"
#include "Monitor.h"
#include "Input.h"
#include "App.h"
#include "Config.h"
#include "Logging.h"
#include "FileSystem.h"
#include "StringUtils.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include <vector>

namespace gameplay
{

static void __on_error_callback(int error, const char* description)
{
    printf("Glfw Error %d: %s\n", error, description);
}

static Float2 __mouse_scale_cursor_pos(Window* window, Double2 scale)
{
    return {
        static_cast<float>(scale.x * window->handle->mouseScale.x),
        static_cast<float>(scale.y * window->handle->mouseScale.y)
    };
}

static void __on_monitor_callback(GLFWmonitor* mon, int evt)
{
    auto windowing = App::get_app()->get_windowing();
    Monitor* monitor = nullptr;
    MonitorChangeEvent changeEvent = MonitorChangeEvent::UNKNOWN;
    bool shouldDelete = false;
    switch (evt)
    {
    case GLFW_CONNECTED:
        changeEvent = MonitorChangeEvent::CONNECTED;
        monitor = new Monitor();
        monitor->handle = std::make_unique<MonitorHandle>();
        monitor->handle->glfwMonitor = mon;
        break;
    case GLFW_DISCONNECTED:
        changeEvent = MonitorChangeEvent::DISCONNECTED;
        monitor = (Monitor*)glfwGetMonitorUserPointer(mon);
        shouldDelete = true;
        break;
    }
    windowing->on_monitor_change.emit(monitor, changeEvent);
    if (shouldDelete)
    {
        delete monitor;
    }
}

static void __on_joystick_callback(int jid, int evt)
{
    GamepadChangeEvent changeEvent = GamepadChangeEvent::UNKNOWN;
    switch (evt)
    {
    case GLFW_CONNECTED:
        changeEvent = GamepadChangeEvent::CONNECTED;
        Input::on_gamepad_changed.emit(jid, changeEvent);
        break;
    case GLFW_DISCONNECTED:
        changeEvent = GamepadChangeEvent::DISCONNECTED;
        Input::on_gamepad_changed.emit(jid, changeEvent);
        break;
    }
}

static void __on_window_pos_callback(GLFWwindow* glfwWindow, int x, int y)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_move.emit(window, { x, y });
}

static void __on_window_size_callback(GLFWwindow* glfwWindow, int width, int height)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->handle->mouseScale.x = 1 / float(width);
    window->handle->mouseScale.y = 1 / float(height);
    window->on_resize.emit(window, { width, height });
}

static void __on_window_close_callback(GLFWwindow* glfwWindow)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_close.emit(window);
}

static void __on_window_focus_callback(GLFWwindow* glfwWindow, int focused)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_focus.emit(window, focused == GLFW_TRUE);
}

static void __on_window_iconify_callback(GLFWwindow* glfwWindow, int iconified)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_minimize.emit(window, iconified == GLFW_TRUE);
}

static void __on_window_maximize_callback(GLFWwindow* glfwWindow, int maximied)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_maximize.emit(window, maximied == GLFW_TRUE);
}

static void __on_window_content_scale_callback(GLFWwindow* glfwWindow, float xScale, float yScale)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_content_scale.emit(window, { xScale, yScale });
}

static void __on_cursor_move_callback(GLFWwindow* glfwWindow, double screenCursorX, double screenCursorY)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    Float2 windowCursorPos = __mouse_scale_cursor_pos(window, {screenCursorX, screenCursorY});
    window->on_cursor_move.emit(window, { windowCursorPos.x, windowCursorPos.y });
}

static void __on_cursor_enter_callback(GLFWwindow* glfwWindow, int entered)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_cursor_enter.emit(window, entered == GLFW_TRUE);
}

static void __on_scroll_callback(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_scroll.emit(window, {(float)xoffset, (float)yoffset});
}

static void __on_key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    KeyAction keyAction = KeyAction::RELEASE;
    switch(action)
    {
    case GLFW_RELEASE:
        keyAction = KeyAction::RELEASE;
        break;
    case GLFW_PRESS:
        keyAction = KeyAction::PRESS;
        break;
    case GLFW_REPEAT:
        keyAction = KeyAction::REPEAT;
        break;
    }
    window->on_key.emit(window, GLFWUtils::to_key(key), keyAction, GLFWUtils::to_key_mod_flags(mods));
}

static void __on_char_callback(GLFWwindow* glfwWindow, unsigned int codepoint)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_char.emit(window, codepoint);
}

static void __on_mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    ButtonAction buttonAction = (action == GLFW_PRESS) ? ButtonAction::PRESS : ButtonAction::RELEASE;
    Double2 cursorPos;
    glfwGetCursorPos(window->handle->glfwWindow, &cursorPos.x, &cursorPos.y);
    Float2 scaledCursorPos = __mouse_scale_cursor_pos(window, cursorPos);
    window->on_mouse_button.emit(window, button, buttonAction, scaledCursorPos, GLFWUtils::to_key_mod_flags(mods));
}

static void __on_drop_callback(GLFWwindow* glfwWindow, int pathCount, const char** paths)
{
    Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
    window->on_drop.emit(window, paths, (size_t)pathCount);
}

struct Windowing::Impl
{
    bool initialized = false;
    std::vector<Monitor*> monitors;
    std::unordered_set<std::shared_ptr<Window>> windows;
    std::unordered_set<Cursor*> cursors;
};

Windowing::Windowing()
{
    _impl = std::make_unique<Windowing::Impl>();
}

Windowing::~Windowing()
{
}

void Windowing::startup()
{
    if (_impl->initialized)
        return;

    _impl->initialized = true;
    glfwSetErrorCallback(__on_error_callback);
    if (!glfwInit())
    {
        GP_LOG_ERROR("GLFW initialization failed.");
        return;
    }
    // load the connected monitors
    int monitorCount;
    GLFWmonitor** glfwMonitors = glfwGetMonitors(&monitorCount);
    for (int i = 0; i < monitorCount; i++)
    {
        Monitor* monitor = new Monitor();
        monitor->handle = std::make_unique<MonitorHandle>();
        monitor->handle->glfwMonitor = glfwMonitors[i];
        glfwSetMonitorUserPointer(monitor->handle->glfwMonitor, monitor);
    }
    glfwSetMonitorCallback(__on_monitor_callback);

    // load the SDL gamecontrollerdb.txt mappings database.
    // super smal file. So just load synchronously for now.
    auto fs = App::get_app()->get_file_system();
    {
        auto config = App::get_app()->get_config();
        std::string gamepad_mappings_db = config->get_string("input.gamepad_mappings_db", "");
        if (App::get_app()->resolve_resource_path(gamepad_mappings_db))
        {
            if (fs->exists(gamepad_mappings_db.c_str()))
            {
                auto mappingFile = fs->open_file(gamepad_mappings_db.c_str(), FileMode::READ);
                if (mappingFile)
                {
                    size_t mappingFileSize = fs->get_file_size(mappingFile);
                    char* mappingData = (char*)GP_MALLOC(mappingFileSize);
                    size_t read = fs->read_file_chunk(mappingFile, mappingData, mappingFileSize);
                    if (mappingData && (read == mappingFileSize))
                    {
                        std::string mappingsDbStr = mappingData;
                        Input::set_gamepad_mappings_db(mappingsDbStr.c_str());
                    }
                }
            }
        }
    }
}

void Windowing::shutdown()
{
    // Sync the monitors first and cleanup the monitor objects
    size_t monitorCount;
    get_monitors(&monitorCount);
    std::for_each(_impl->monitors.begin(), _impl->monitors.end(), [](auto& monitor){delete monitor;});
    _impl->monitors.clear();
    // Cleanup the cursors and windows managed objects
    std::for_each(_impl->cursors.begin(), _impl->cursors.end(), [](auto& cursor){delete cursor;});
    _impl->cursors.clear();
    _impl->windows.clear();

    glfwTerminate();
}

std::shared_ptr<Window> Windowing::create_window(const WindowDesc& desc)
{
    std::shared_ptr<Window> window = nullptr;

    // default window hints
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    // apply user supplied window hints
    glfwWindowHint(GLFW_RESIZABLE, (desc.hints & WINDOW_HINT_NO_RESIZE) ? GLFW_FALSE: GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, (desc.hints & WINDOW_HINT_NO_DECORATION) ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, (desc.hints & WINDOW_HINT_NO_AUTO_ICONIFY) ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, (desc.hints & WINDOW_HINT_NO_FOCUS_ON_SHOW) ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, (desc.hints & WINDOW_HINT_SCALE_TO_MONITOR) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, (desc.hints & WINDOW_HINT_FLOATING) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, (desc.hints & WINDOW_HINT_MAXIMIZED) ? GLFW_TRUE : GLFW_FALSE);
    // apply fullscreen window hints
    if (desc.fullscreen)
    {
        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    }
    // create the window
    window = std::make_shared<Window>();
    window->handle = std::make_unique<WindowHandle>();
    window->handle->glfwWindow = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
    if (!window->handle->glfwWindow)
    {
        return nullptr;
    }

    // store this window in the glfw user pointer
    glfwSetWindowUserPointer(window->handle->glfwWindow, window.get());

    // setup glfw window callbacks
    glfwSetWindowPosCallback(window->handle->glfwWindow, __on_window_pos_callback);
    glfwSetWindowSizeCallback(window->handle->glfwWindow, __on_window_size_callback);
    glfwSetWindowCloseCallback(window->handle->glfwWindow, __on_window_close_callback);
    glfwSetWindowFocusCallback(window->handle->glfwWindow, __on_window_focus_callback);
    glfwSetWindowIconifyCallback(window->handle->glfwWindow, __on_window_iconify_callback);
    glfwSetWindowMaximizeCallback(window->handle->glfwWindow, __on_window_maximize_callback);
    glfwSetWindowContentScaleCallback(window->handle->glfwWindow, __on_window_content_scale_callback);
    glfwSetCursorPosCallback(window->handle->glfwWindow, __on_cursor_move_callback);
    glfwSetCursorEnterCallback(window->handle->glfwWindow, __on_cursor_enter_callback);
    glfwSetDropCallback(window->handle->glfwWindow, __on_drop_callback);
    glfwSetKeyCallback(window->handle->glfwWindow, __on_key_callback);
    glfwSetCharCallback(window->handle->glfwWindow, __on_char_callback);
    glfwSetMouseButtonCallback(window->handle->glfwWindow, __on_mouse_button_callback);
    glfwSetScrollCallback(window->handle->glfwWindow, __on_scroll_callback);
    glfwSetJoystickCallback(__on_joystick_callback);

    // store the last size, pos and mouse scale for toggline fullscreen
    glfwGetWindowSize(window->handle->glfwWindow, &window->handle->size.x, &window->handle->size.y);
    glfwGetWindowPos(window->handle->glfwWindow, &window->handle->pos.x, &window->handle->pos.y);
    window->handle->mouseScale.x = 1 / float(window->handle->size.x);
    window->handle->mouseScale.y = 1 / float(window->handle->size.y);

    // get the platform window/display
#if GP_PLATFORM_WINDOWS
    window->handle->platformWindow = glfwGetWin32Window(window->handle->glfwWindow);
 #elif GP_PLATFORM_LINUX
    window->handle->platformWindow = (void*)glfwGetX11Window(window->handle->glfwWindow);
    window->handle->platformDisplay = glfwGetX11Display();
#endif
    // set fullscreen if enabled
    if (desc.fullscreen)
    {
        window->set_fullscreen(true);
    }
    _impl->windows.insert(window);

    return window;
}

void Windowing::destroy_window(std::shared_ptr<Window> window)
{
    _impl->windows.erase(window);
}

WindowHints Windowing::parse_window_hints(const char* str)
{
    WindowHints res = WINDOW_HINT_NONE;
    std::vector<std::string> hintsVec = StringUtils::split(str, '|');
    for (auto& hintStr : hintsVec)
    {
        StringUtils::trim(hintStr);
        StringUtils::to_upper(hintStr);
        if (hintStr.compare("NONE") == 0)
            res |= WINDOW_HINT_NONE;
        else if (hintStr.compare("NO_RESIZE") == 0)
            res |= WINDOW_HINT_NO_RESIZE;
        else if (hintStr.compare("NO_DECORATION") == 0)
            res |= WINDOW_HINT_NO_DECORATION;
        else if (hintStr.compare("NO_AUTO_ICONIFY") == 0)
            res |= WINDOW_HINT_NO_AUTO_ICONIFY;
        else if (hintStr.compare("NO_FOCUS_ON_SHOW") == 0)
            res |= WINDOW_HINT_NO_FOCUS_ON_SHOW;        
        else if (hintStr.compare("SCALE_TO_MONITOR") == 0)
            res |= WINDOW_HINT_SCALE_TO_MONITOR;      
        else if (hintStr.compare("SCALE_TO_MONITOR") == 0)
            res |= WINDOW_HINT_SCALE_TO_MONITOR;      
        else if (hintStr.compare("FLOATING") == 0)
            res |= WINDOW_HINT_FLOATING;
        else if (hintStr.compare("MAXIMIZED") == 0)
            res |= WINDOW_HINT_MAXIMIZED;
        else
            GP_LOG_WARN("Failed to parse WindowHints string {}", hintStr);
    }
    return res;
}

Monitor* Windowing::get_primary_monitor()
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    return (Monitor*)glfwGetMonitorUserPointer(primaryMonitor);
}

static int __get_rect_overlap(Int2 r0Pos, Int2 r0Size, Int2 r1Pos, Int2 r1Size)
{
    return std::max(0, std::min(r0Pos.x + r0Size.x, r1Pos.x + r1Size.x) - std::max(r0Pos.x, r1Pos.x)) *
           std::max(0, std::min(r0Pos.y + r0Size.y, r1Pos.y + r1Size.y) - std::max(r0Pos.y, r1Pos.y));
}

Monitor* Windowing::get_monitor(Window* window)
{
    Monitor* retMonitor = nullptr;
    Int2 windowPos = window->get_pos();
    Int2 windowSize = window->get_size();
    int mostOverlap = 0;
    for (size_t i = 0; i < _impl->monitors.size(); i++)
    {
        Monitor* monitor = _impl->monitors[i];
        Int2 monitorPos = monitor->get_pos();
        Int2 monitorSize = { monitor->get_video_mode().width, monitor->get_video_mode().height};
        Int2 windowPosSize;
        windowPosSize.x = windowPos.x + windowSize.x;
        windowPosSize.y = windowPos.y + windowSize.y;
        int overlap = __get_rect_overlap(windowPosSize, windowSize, monitorPos, monitorSize);
        if (mostOverlap < overlap)
        {
            mostOverlap = overlap;
            retMonitor = monitor;
        }
    }
    return retMonitor;
}

Monitor** Windowing::get_monitors(size_t* monitorCount)
{
    _impl->monitors.clear();
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    for (int i = 0; i < count; i++)
    {
        _impl->monitors.push_back((Monitor*)glfwGetMonitorUserPointer(monitors[i]));
    }
    *monitorCount = count;
    return _impl->monitors.data();
}

Cursor* Windowing::create_cursor(CursorStandardShape shape)
{
    Cursor* cursor = nullptr;
    GLFWcursor* glfwCursor = nullptr;
    switch (shape)
    {
    case CursorStandardShape::ARROW:
        glfwCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        break;
    case CursorStandardShape::IBEAM:
        glfwCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        break;
    case CursorStandardShape::CROSSHAIR:
        glfwCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        break;
    case CursorStandardShape::HAND:
        glfwCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        break;
    case CursorStandardShape::HRESIZE:
        glfwCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        break;
    case CursorStandardShape::VRESIZE:
        glfwCursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
        break;
    default:
        GP_ASSERT(false);
        glfwCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
        break;
    }
    if (glfwCursor)
    {
        cursor = new Cursor();
        cursor->glfwCursor = glfwCursor;
        _impl->cursors.insert(cursor);
    }
    return cursor;
}

Cursor* Windowing::create_cursor(const Pixmap* image, Int2 hotspotPos)
{
    Cursor* cursor = nullptr;
    GLFWcursor* glfwCursor = glfwCreateCursor((const GLFWimage*)image, hotspotPos.x, hotspotPos.y);
    if (glfwCursor)
    {
        cursor = new Cursor();
        cursor->glfwCursor = glfwCursor;
        _impl->cursors.insert(cursor);
    }
    return cursor;
}

void Windowing::destroy_cursor(Cursor* cursor)
{
    if (cursor && cursor->glfwCursor)
    {
        glfwDestroyCursor(cursor->glfwCursor);
        _impl->cursors.erase(cursor);
        delete cursor;
    }
}

 void Windowing::set_time(double time)
{
    return glfwSetTime(time);
}

double Windowing::get_time() const
{
    return glfwGetTime();
}

void Windowing::poll_events()
{
    glfwPollEvents();
}

void Windowing::wait_events(double timeout)
{
    if (timeout == 0.0)
    {
        glfwWaitEvents();
    }
    else
    {
        glfwWaitEventsTimeout(timeout);
    }
}

void Windowing::post_empty_event()
{
    glfwPostEmptyEvent();
}

void Windowing::set_clip_board_string(const char* str)
{
    glfwSetClipboardString(nullptr, str);
}

 const char* Windowing::get_clipboard_string() const
 {
     return glfwGetClipboardString(nullptr);
 }
}
GP_ASSERT_STRUCTS_MATCH(GLFWimage, gameplay::Pixmap);
