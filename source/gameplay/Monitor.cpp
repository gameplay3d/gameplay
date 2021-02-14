#include "Monitor.h"
#include "WindowingGLFW.h"


namespace gameplay
{

Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

const char* Monitor::get_name() const
{
    return glfwGetMonitorName(handle->glfwMonitor);
}

VideoMode Monitor::get_video_mode() const
{
    VideoMode videoMode;
    const GLFWvidmode* vidMode = glfwGetVideoMode(handle->glfwMonitor);
    videoMode.width = vidMode->width;
    videoMode.height = vidMode->height;
    videoMode.redBits = vidMode->redBits;
    videoMode.greenBits = vidMode->greenBits;
    videoMode.blueBits = vidMode->blueBits;
    videoMode.refreshRate = vidMode->refreshRate;
    return videoMode;
}   

Int2 Monitor::get_physical_size() const
{
    Int2 size;
    glfwGetMonitorPhysicalSize(handle->glfwMonitor, &size.x, &size.y);
    return size;
}

Int2 Monitor::get_pos() const
{
    Int2 pos;
    glfwGetMonitorPos(handle->glfwMonitor, &pos.x, &pos.y);
    return pos;
}

Float2 Monitor::get_content_scale() const
{
    Float2 scale;
    glfwGetMonitorContentScale(handle->glfwMonitor, &scale.x, &scale.y);
    return scale;
}

WorkArea Monitor::get_work_area() const
{
    WorkArea workArea;
    glfwGetMonitorWorkarea(handle->glfwMonitor, &workArea.pos.x, &workArea.pos.y, &workArea.size.x, &workArea.size.y);
    return workArea;
}

void Monitor::set_user_ptr(void* userPtr)
{
    handle->userPtr = userPtr;
}

void* Monitor::get_user_ptr() const
{
    return handle->userPtr;
}
}
