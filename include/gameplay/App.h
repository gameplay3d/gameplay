#pragma once

#include "Defines.h"
#include "Signal.h"
#include "Types.h"
#include <memory>
#include <string>

namespace gameplay
{
class FileSystem;
class Config;
class Logging;
class Windowing;
class Window;
class Renderer;
class UI;


/**
 * The main application.
 *
 * Controls the application runtime and main sub-systems. 
 */
class GP_API App
{
public:

    /**
     * Destructor.
     */
    ~App();

	/**
	 * Gets the application instance.
     *
     * @return The application instance
	 */
	static App* get_app();

	/**
	 * Runs the main event loop and waits until exit() is called.
     *
     * @param argc The number of comand-line arguments passed.
     * @param argv The command-line arguement strings.
	 */
	int exec(int argc, char** argv);

	/**
	 * Exits the application.
	 */
	void exit();

    /**
     * Set the current application running time (in seconds).
     *
     * @param time The time to be set (in seconds).
     */
    void set_time(double time);

    /**
     * Get the current application running time (in seconds).
     *
     * @return The current application running time (in seconds).
     */
    double get_time() const;

    /**
     * Gets the application file system.
     *
     * @return The application file system.
     */
    std::shared_ptr<FileSystem> get_file_system() const;

    /**
     * Gets the application config system.
     *
     * @return The application config system.
     */
    std::shared_ptr<Config> get_config() const;

    /**
     * Gets the application logging system.
     *
     * @return The application logging system.
     */
    std::shared_ptr<Logging> get_logging() const;

    /**
     * Gets the application windowing system.
     *
     * @return The application windowing system.
     */
    std::shared_ptr<Windowing> get_windowing() const;

    /**
     * Gets the main applicaton window.
     *
     * @return The application main window.
     */
    std::shared_ptr<Window> get_window() const;

    /**
     * Gets the application renderer.
     *
     * @return The applications renderer.
     */
    std::shared_ptr<Renderer> get_renderer() const;

    /**
     * Gets the application user interface (UI) system.
     *
     * @return The applications user interface (UI) system.
     */
    std::shared_ptr<UI> get_ui() const;

    /**
     * sets a resource path for the specified alias.
     *
     * note: Resource paths may include other resource paths
     * that already exist.
     *
     * The system is populated with the following resource paths:
     * 
     * @app.dir
     * @config.dir
     * 
     * @param alias The alias to be set.
     * @param path The directory path for the alias to use.
     */
    void set_resource_path(const char* alias, const char* path);

    /**
     * Clears a resource path.
     *
     * @param alias The resource path to be cleared.
     */
    void clear_resource_path(const char* alias);

    /**
     * Resolves an application resource path in place
     *
     * Ex. @images/tree.png will replace the @images with the full resolved path from app.config
     *
     * This does not validate the paths exist. So you can also
     * resolve path that maybe intended for directory creation.
     * 
     * @param path The resource path to be resolved.
     * @return true if the path can be resolved, false cannot be resolved due to missing tokens.
     */
    bool resolve_resource_path(std::string& path);

    /**
     * Signals the application has completed startup.
     *
     * emits:
     * <App*> The running application.
     */
    Signal<App*> on_startup;

     /**
     * Signals the application has started to shutdown.
     *
     * emits:
     * <App*> The running application.
     */
    Signal<App*> on_shutdown;

private:
	App();
	struct Impl;
    std::unique_ptr<Impl> _impl;
};
}
