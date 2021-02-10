#pragma once

#include "App.h"
#include "Types.h"
#include "Monitor.h"
#include "Input.h"
#include "Signal.h"


namespace gameplay
{
class Window;
class Monitor;
struct Cursor;

typedef uint32_t WindowHints;
constexpr WindowHints WINDOW_HINT_NONE = 0;
constexpr WindowHints WINDOW_HINT_NO_RESIZE = 1 << 0;
constexpr WindowHints WINDOW_HINT_NO_DECORATION = 1 << 1;
constexpr WindowHints WINDOW_HINT_NO_AUTO_ICONIFY = 1 << 2;
constexpr WindowHints WINDOW_HINT_NO_FOCUS_ON_SHOW = 1 << 3;
constexpr WindowHints WINDOW_HINT_SCALE_TO_MONITOR = 1 << 4;
constexpr WindowHints WINDOW_HINT_FLOATING = 1 << 5;
constexpr WindowHints WINDOW_HINT_MAXIMIZED = 1 << 6;

struct WindowDesc
{
    const char* title;
    int width;
    int height;
    bool fullscreen;
    WindowHints hints;
};

enum class CursorStandardShape : uint32_t
{
    ARROW,
    IBEAM,
    CROSSHAIR,
    HAND,
    HRESIZE,
    VRESIZE
};


/**
 * Defines the windowing system for window management.
 */
class GP_API Windowing
{
    friend class App;
public:
    /**
     * Create a new platform window.
     *
     * @param desc The window descriptor.
     * @return The window created.
     */
    Window* create_window(const WindowDesc& desc);

    /**
     * Destroy a platform window.
     *
     * @param window The window to be destroyed.
     */
    void destroy_window(Window* window);

    /**
     * Parses a string containing window hints as string separated by "|"
     *
     * @param str The string of hints Ex "NO_RESIZE|NO_DECORATION"
     * @return The bitwise combination of parsed hints.
     */
    static WindowHints parse_window_hints(const char* str);

    /**
     * Gets the primary monitor.
     *
     * @return The primary monitor.
     */
    Monitor* get_primary_monitor();

    /**
     * Gets the monitor the window occupies the most of.
     *
     * @param window The window to check against.
     * @return The monitor the window occupies the most of.
     */
    Monitor* get_monitor(Window* window);

    /**
     * Gets the monitors connected.
     *
     * @param monitorCount The number of monitors returned
     * @return The monitors connected.
     */
    Monitor** get_monitors(size_t* monitorCount);

    /**
     * Signals that a monitor has changed and been added or removed to the system.
     *
     * emits:
     * <Monitor*> The monitor being changed.
     * <MonitorChangeEvent> The change event.
     */
    Signal<Monitor*, MonitorChangeEvent> on_monitor_change;

    /**
     * Creates a cursor for the standard cursor shape.
     *
     * @param shape The (standard) cursor shape.
     * @return The cursor created.
     */
    Cursor* create_cursor(CursorStandardShape shape);

    /**
     * Creates a cursor for an pixmap image data.
     *
     * @param image The pixmap image data to create the cursor from.
     * @param hotspotPos The top-left position of the exact click hotspot.\
     * @return The cursor created.
     */
    Cursor* create_cursor(const Pixmap* image, Int2 hotspotPos);

    /**
     * Destroys a cursor.
     *
     * @param cursor The cursor to destroy.
     */
    void destroy_cursor(Cursor* cursor);

    /**
     * Sets the running system time (in seconds).
     *
     * @param timeInSecs The running system time (in seconds).
     */
    void set_time(double time);

    /**
     * Gets the running system time (in seconds).
     *
     * @return The running system time (in seconds).
     */
    double get_time() const;

    /**
     * Polls for processes only those events that have
     * already been received and then returns immediately.
     */
    void poll_events();

    /**
     * Puts the calling thread to sleep until at
     * least one event is available in the event queue.
     */
    void wait_events(double timeout = 0);
    
    /**
     * Posts an empty event from the current thread
     * to the event queue, causing wait_events to return.
     */
    void post_empty_event();

    /**
     * Sets the system clipboard to the specified, utf-8 encoded string.
     *
     * @param str The utf8 encoded string, to be put into the system clipboard.
     */
    void set_clip_board_string(const char* str);

    /**
     * Gets the system clipboard to the specified, utf-8 encoded string.
     *
     * @return The utf8 encoded string, that is in the system clipboard.
     */
    const char* get_clipboard_string() const;

private:
    Windowing();
    ~Windowing();
    void startup();
    void shutdown();
    struct Impl;
    Impl* _impl = nullptr;
};
}

