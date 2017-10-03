#pragma once

#include "Platform.h"
#include "Graphics.h"

namespace gameplay
{
    
/**
 * Defines a macos platform impl.
 */
class PlatformMacOS : public Platform
{
public:
    
    /**
     * Constructor.
     */
    PlatformMacOS();
    
    /**
     * Destructor
     */
    ~PlatformMacOS();
    
    /**
     * @see Platform::startup
     */
    bool startup(int argc, char** argv);
    
    /**
     * @see Platform::shutdown
     */
    void shutdown();
    
    /**
     * @see Platform::enterMessagePump
     */
    int enterMessagePump();
    
    /**
     * @see Platform::getWindow
     */
    unsigned long getWindow();
    
    /**
     * @see Platform::getConnection
     */
    unsigned long getConnection();
    
    /**
     * @see Platform::getGraphics
     */
    Graphics* getGraphics();
    
private:
    
    Graphics* _graphics;
};
    
}
