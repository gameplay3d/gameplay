#pragma once

#include "App.h"

namespace gameplay
{
struct VideoMode
{
    int32_t width;
    int32_t height;
    int32_t redBits;
    int32_t greenBits;
    int32_t blueBits;
    int32_t refreshRate;
};

struct WorkArea
{
    Int2 pos;
    Int2 size;
};

enum class MonitorChangeEvent : uint32_t
{
    UNKNOWN,
    CONNECTED,
    DISCONNECTED
};

struct MonitorHandle;


/**
 * Defines a monitor.
 */
class GP_API Monitor
{
public:

    /**
     * Constructor. 
     */
    Monitor();

    /**
     * Destructor.
     */
    ~Monitor();

    /**
     * Gets the name of the monitor.
     *
     * @return The name of the monitor.
     */
    const char* get_name() const;

    /**
     * Gets the video mode information.
     *
     * @return The video mode information.
     */
    VideoMode get_video_mode() const;

    /**
     * Gets the physical size, in millimetres, of the display area of the specified monitor.
     *
     * @return The physical size.
     */
    Int2 get_physical_size() const;

    /**
     * Gets the position of the monitor's viewport on the virtual screen.
     *
     * @return The position of the monitor's viewport.
     */
    Int2 get_pos() const;

    /**
     * Gets the content scale for the monitor.
     *
     * The content scale is the ratio between the current DPI and
     * the platform's default DPI. This is especially important for
     * text and any UI elements. If the pixel dimensions of your UI
     * scaled by this look appropriate on your machine then it should
     * appear at a reasonable size on other machines regardless of
     * their DPI and scaling settings. This relies on the system DPI
     * and scaling settings being somewhat correct.
     *
     * @return The content scale for the monitor. 
     */
    Float2 get_content_scale() const;

    /**
     * Gets the position and size of the work area.
     * 
     * The position, in screen coordinates, of the upper-left corner
     * of the work area of the specified monitor along with the
     * work area size in screen coordinates. The work area is defined
     * as the area of the monitor not occluded by the operating
     * system task bar where present. If no task bar exists then the
     * work area is the monitor resolution in screen coordinates.
     *
     * @return The work area.
     */
    WorkArea get_work_area() const;

    /**
     * Sets user pointer associated with this monitor.
     *
     * @param userPtr The user pointer to associate.
     */
    void set_user_ptr(void* userPtr);

    /**
     * Gets the user pointer associated with this monitor.
     *
     * @return The user pointer to associate.
     */
    void* get_user_ptr() const;

    MonitorHandle* handle = nullptr;
};
}
