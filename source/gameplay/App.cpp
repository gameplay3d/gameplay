#include "App.h"
#include "AppGLFW.h"
#include "Config.h"
#include "FileSystem.h"
#include "Logging.h"
#include "Monitor.h"
#include "Path.h"
#include "Renderer.h"
#include "StringUtils.h"
#include "UI.h"
#include "Window.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_set>
#include <vector>

#include <imgui.h>

namespace gameplay
{

struct App::Impl
{
    bool running = false;
    Window* window{nullptr};
    std::unordered_set<Window*> windows;
    std::vector<Monitor*> monitors;
    std::unordered_set<std::shared_ptr<Cursor>> cursors;
    std::shared_ptr<FileSystem> fs{nullptr};
    std::shared_ptr<Config> config{nullptr};
    std::shared_ptr<Logging> logging{nullptr};
    std::shared_ptr<Renderer> renderer{nullptr};
    std::shared_ptr<UI> ui{nullptr};
    std::unordered_map<std::string, std::string> resourceAliases;

    Window* create_window(const WindowDesc& desc);
    void destroy_window(Window* window);
    Monitor* get_primary_monitor();
    Monitor* get_monitor(Window* window);
    Monitor** get_monitors(size_t* monitorCount);
    std::shared_ptr<Cursor> create_cursor(CursorStandardShape shape);
    std::shared_ptr<Cursor> create_cursor(const Pixmap* image, Int2 hotspotPos);
    void destroy_cursor(std::shared_ptr<Cursor> cursor);
};


App::App()
{
    _impl = std::make_unique<App::Impl>();
}

App::~App()
{
}

App* App::get_app()
{
    static App app;
    return &app;
}

int App::exec(int argc, char** argv)
{
	if (_impl->running)
		return 1;
	_impl->running = true;

    // create the file system
    _impl->fs = std::make_shared<FileSystem>();

    // create and load settings
    _impl->config = std::make_shared<Config>();
    _impl->config->load(argc, argv);

    // register application executable directory as a file system alias
    _impl->resourceAliases["app.dir"] = _impl->fs->get_app_directory_path();

    // register file system aliases from config
    _impl->config->for_each_table("resource.alias",
        [](Config* config, void* userPtr) -> bool
        {
            App::Impl* impl = (App::Impl*)userPtr;
            std::string name = config->get_string("name", "");
            std::string pathStr = config->get_string("path", "");
            if (App::get_app()->resolve_resource_path(pathStr))
            {
                impl->resourceAliases[name] = pathStr;
            }
            return true;
        }, (void*)_impl.get());


    // startup the logging system
    _impl->logging = std::make_shared<Logging>();
    _impl->logging->startup();
    
    // startup the glfw windowing
    glfwSetErrorCallback(GLFWUtils::on_error_callback);
    if (!glfwInit())
    {
        GP_LOG_ERROR("GLFW initialization failed.");
        return 1;
    }
    

    // load the main window from config
    std::string windowTitle = _impl->config->get_string("window.title", "");
    int windowWidth = _impl->config->get_int("window.width", 1280);
    int windowHeight = _impl->config->get_int("window.height", 720);
    bool windowFullscreen = _impl->config->get_bool("window.fullscreen", false);
    std::string windowHintsStr = _impl->config->get_string("window.hints", "NONE");
    WindowHints windowHints = GLFWUtils::parse_window_hints(windowHintsStr.c_str());

    // create the main window
    WindowDesc windowDesc = { windowTitle.c_str(), windowWidth, windowHeight, windowFullscreen, windowHints };
    _impl->window = _impl->create_window(windowDesc);
    // adjust the window position where the frame is at 0, 0
    int top;
    _impl->window->get_frame_size(nullptr, &top, nullptr, nullptr);
    _impl->window->set_pos({0, top});

    // startup the ui system
    _impl->ui = std::make_shared<UI>();
    _impl->ui->startup();

    // startup the renderer
    _impl->renderer = std::make_shared<Renderer>();
    _impl->renderer->startup();
   
    // run the main window event loop until we should close
    while (!_impl->window->should_close())
    {
         glfwPollEvents();

        _impl->renderer->next_frame();

        _impl->renderer->update();
        _impl->ui->update();

        _impl->renderer->render_frame();
        _impl->renderer->present_frame();
    }

    // destroy main window and shutdown
    _impl->destroy_window(_impl->window);
    // Sync the monitors first and cleanup the monitor objects
    size_t monitorCount;
    get_monitors(&monitorCount);
    std::for_each(_impl->monitors.begin(), _impl->monitors.end(), [](auto& monitor){delete monitor;});
    _impl->monitors.clear();
    // Cleanup the cursors and windows managed objects
    _impl->cursors.clear();
    std::for_each(_impl->windows.begin(), _impl->windows.end(), [](auto& window){delete window;});
    _impl->windows.clear();

    glfwTerminate();
    

	return 0;
}

void App::exit()
{
    _impl->window->close();
}

void App::set_time(double timeSecs)
{
    glfwSetTime(timeSecs);
}

double App::get_time() const
{
    return glfwGetTime();
}

Monitor* App::get_primary_monitor()
{
    return _impl->get_primary_monitor();
}


Monitor* App::get_monitor(Window* window)
{
    return _impl->get_monitor(window);
}

Monitor** App::get_monitors(size_t* monitorCount)
{
    return _impl->get_monitors(monitorCount);
}

std::shared_ptr<Cursor> App::create_cursor(CursorStandardShape shape)
{
    return _impl->create_cursor(shape);
}

std::shared_ptr<Cursor> App::create_cursor(const Pixmap* image, Int2 hotspotPos)
{
    return _impl->create_cursor(image, hotspotPos);
}

void App::destroy_cursor(std::shared_ptr<Cursor> cursor)
{
    _impl->destroy_cursor(cursor);
}

void App::set_clip_board_string(const char* str)
{
    glfwSetClipboardString(nullptr, str);
}

const char* App::get_clipboard_string() const
{
    return glfwGetClipboardString(nullptr);
}

std::shared_ptr<FileSystem> App::get_file_system() const
{
	return _impl->fs;
}

std::shared_ptr<Config> App::get_config() const
{
	return _impl->config;
}

std::shared_ptr<Logging> App::get_logging() const
{
	return _impl->logging;
}

Window* App::get_window() const
{
	return _impl->window;
}

std::shared_ptr<Renderer> App::get_renderer() const
{
	return _impl->renderer;
}

std::shared_ptr<UI> App::get_ui() const
{
	return _impl->ui;
}

void App::set_resource_path(const char* alias, const char* path)
{
    _impl->resourceAliases[alias] = path;
}

void App::clear_resource_path(const char* alias)
{
    _impl->resourceAliases.erase(alias);
}

bool App::resolve_resource_path(std::string& resourcePath)
{
    std::string path = resourcePath;
    size_t startPos = 0;
    const std::string startDelim = "@";
    while((startPos = path.find(startDelim, startPos)) != std::string::npos)
    {
        const std::string stopDelim = "/";
        size_t endPos = path.find(stopDelim, startPos);
        if (endPos != std::string::npos)
        {
            size_t aliasLen = endPos - startPos;
            std::string alias = path.substr(startPos + 1, aliasLen - 1);
            auto pair = _impl->resourceAliases.find(alias);
            if ( pair == _impl->resourceAliases.end())
                return false;
            std::string aliasPath = _impl->resourceAliases[alias];
            path.replace(startPos, aliasLen, aliasPath);
        }
    }
    resourcePath = path;
    return true;
}


Window* App::Impl::create_window(const WindowDesc& desc)
{
    Window* window = nullptr;

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
    window = new Window();
    window->handle = std::make_unique<WindowHandle>();
    window->handle->glfwWindow = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
    if (!window->handle->glfwWindow)
    {
        return nullptr;
    }

    // store this window in the glfw user pointer
    glfwSetWindowUserPointer(window->handle->glfwWindow, window);

    // setup glfw window callbacks
    glfwSetWindowPosCallback(window->handle->glfwWindow, GLFWUtils::on_window_pos_callback);
    glfwSetWindowSizeCallback(window->handle->glfwWindow, GLFWUtils::on_window_size_callback);
    glfwSetWindowCloseCallback(window->handle->glfwWindow, GLFWUtils::on_window_close_callback);
    glfwSetWindowFocusCallback(window->handle->glfwWindow, GLFWUtils::on_window_focus_callback);
    glfwSetWindowIconifyCallback(window->handle->glfwWindow, GLFWUtils::on_window_iconify_callback);
    glfwSetWindowMaximizeCallback(window->handle->glfwWindow, GLFWUtils::on_window_maximize_callback);
    glfwSetWindowContentScaleCallback(window->handle->glfwWindow, GLFWUtils::on_window_content_scale_callback);
    glfwSetCursorPosCallback(window->handle->glfwWindow, GLFWUtils::on_cursor_move_callback);
    glfwSetCursorEnterCallback(window->handle->glfwWindow, GLFWUtils::on_cursor_enter_callback);
    glfwSetDropCallback(window->handle->glfwWindow, GLFWUtils::on_drop_callback);
    glfwSetKeyCallback(window->handle->glfwWindow, GLFWUtils::on_key_callback);
    glfwSetCharCallback(window->handle->glfwWindow, GLFWUtils::on_char_callback);
    glfwSetMouseButtonCallback(window->handle->glfwWindow, GLFWUtils::on_mouse_button_callback);
    glfwSetScrollCallback(window->handle->glfwWindow, GLFWUtils::on_scroll_callback);
    glfwSetJoystickCallback(GLFWUtils::on_joystick_callback);

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
    windows.insert(window);

    return window;
}

void App::Impl::destroy_window(Window* window)
{
    windows.erase(window);
    GP_SAFE_DELETE(window);
}

Monitor* App::Impl::get_primary_monitor()
{
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    return (Monitor*)glfwGetMonitorUserPointer(primaryMonitor);
}

Monitor* App::Impl::get_monitor(Window* window)
{
    Monitor* retMonitor = nullptr;
    Int2 windowPos = window->get_pos();
    Int2 windowSize = window->get_size();
    int mostOverlap = 0;
    for (size_t i = 0; i < monitors.size(); i++)
    {
        Monitor* monitor = monitors[i];
        Int2 monitorPos = monitor->get_pos();
        Int2 monitorSize = { monitor->get_video_mode().width, monitor->get_video_mode().height};
        Int2 windowPosSize;
        windowPosSize.x = windowPos.x + windowSize.x;
        windowPosSize.y = windowPos.y + windowSize.y;
        int overlap = GLFWUtils::get_rect_overlap(windowPosSize, windowSize, monitorPos, monitorSize);
        if (mostOverlap < overlap)
        {
            mostOverlap = overlap;
            retMonitor = monitor;
        }
    }
    return retMonitor;
}

Monitor** App::Impl::get_monitors(size_t* monitorCount)
{
    monitors.clear();
    int count;
    GLFWmonitor** glfwMonitors = glfwGetMonitors(&count);
    for (int i = 0; i < count; i++)
    {
        monitors.push_back((Monitor*)glfwGetMonitorUserPointer(glfwMonitors[i]));
    }
    *monitorCount = count;
    return monitors.data();
}

std::shared_ptr<Cursor> App::Impl::create_cursor(CursorStandardShape shape)
{
    std::shared_ptr<Cursor> cursor = nullptr;
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
        cursor = std::make_shared<Cursor>();
        cursor->glfwCursor = glfwCursor;
        cursors.insert(cursor);
    }
    return cursor;
}

std::shared_ptr<Cursor> App::Impl::create_cursor(const Pixmap* image, Int2 hotspotPos)
{
    std::shared_ptr<Cursor> cursor{nullptr};
    GLFWcursor* glfwCursor = glfwCreateCursor((const GLFWimage*)image, hotspotPos.x, hotspotPos.y);
    if (glfwCursor)
    {
        cursor = std::make_shared<Cursor>();
        cursor->glfwCursor = glfwCursor;
        cursors.insert(cursor);
    }
    return cursor;
}

void App::Impl::destroy_cursor(std::shared_ptr<Cursor> cursor)
{
    if (cursor && cursor->glfwCursor)
    {
        glfwDestroyCursor(cursor->glfwCursor);
        cursors.erase(cursor);
    }
}
}

GP_ASSERT_STRUCTS_MATCH(GLFWimage, gameplay::Pixmap);
