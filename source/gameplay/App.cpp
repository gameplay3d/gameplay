#include "App.h"
#include "Path.h"
#include "FileSystem.h"
#include "Config.h"
#include "Logging.h"
#include "Windowing.h"
#include "Window.h"
#include "Renderer.h"
#include "UI.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <imgui.h>

namespace gameplay
{

struct App::Impl
{
    bool running = false;
    std::shared_ptr<FileSystem> fs{nullptr};
    std::shared_ptr<Config> config{nullptr};
    std::shared_ptr<Logging> logging{nullptr};
    std::shared_ptr<Windowing> windowing{nullptr};
    std::shared_ptr<Window> window{nullptr};
    std::shared_ptr<Renderer> renderer{nullptr};
    std::shared_ptr<UI> ui{nullptr};
    std::unordered_map<std::string, std::string> resourceAliases;
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
    
    // startup the windowing system
    _impl->windowing = std::make_shared<Windowing>();
    _impl->windowing->startup();

    // load the main window from config
    std::string windowTitle = _impl->config->get_string("window.title", "");
    int windowWidth = _impl->config->get_int("window.width", 1280);
    int windowHeight = _impl->config->get_int("window.height", 720);
    bool windowFullscreen = _impl->config->get_bool("window.fullscreen", false);
    std::string windowHintsStr = _impl->config->get_string("window.hints", "NONE");
    WindowHints windowHints = Windowing::parse_window_hints(windowHintsStr.c_str());

    // create the main window
    WindowDesc windowDesc = { windowTitle.c_str(), windowWidth, windowHeight, windowFullscreen, windowHints };
    _impl->window = _impl->windowing->create_window(windowDesc);
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
        _impl->windowing->poll_events();

        _impl->renderer->next_frame();

        _impl->renderer->update();
        _impl->ui->update();

        _impl->renderer->render_frame();
        _impl->renderer->present_frame();
    }

    // destroy main window and shutdown
    _impl->windowing->destroy_window(_impl->window);
    _impl->windowing->shutdown();

	return 0;
}

void App::exit()
{
    _impl->window->close();
}

void App::set_time(double timeSecs)
{
    return _impl->windowing->set_time(timeSecs);
}

double App::get_time() const
{
    return _impl->windowing->get_time();
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

std::shared_ptr<Windowing> App::get_windowing() const
{
	return _impl->windowing;
}

std::shared_ptr<Window> App::get_window() const
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
}
