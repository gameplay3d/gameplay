#ifndef PLATFORM_H_
#define PLATFORM_H_

namespace gameplay
{

class Game;

/**
 * Defines a platform.
 */
class Platform
{
public:

    /**
     * Destructor.
     */
    ~Platform();

    /**
     * Creates a platform for the specified game which is will interacte with.
     *
     * @param game The game to create a platform for.
     * 
     * @return The created platform interface.
     */
    static Platform* create(Game* game);

    /**
     * Begins processing the platform messages.
     *
     * This method handles all OS window messages and drives the game loop.
     * It normally does not return until the application is closed.
     * 
     * @return The platform message pump return code.
     */
    int enterMessagePump();

    /**
     * Gets the absolute platform time starting from when the message pump was started.
     *
     * @return The absolute platform time. (in milliseconds)
     */
    static long getAbsoluteTime();

    /**
     * Sets the absolute platform time since the start of the message pump.
     *
     * @param time The time to set (in milliseconds).
     */
    static void setAbsoluteTime(long time);

    /**
     * Gets whether vertical sync is enabled for the game display.
     * 
     * @return true if vsync is enabled; false if not.
     */
    static bool isVsync();

    /**
     * Sets whether vertical sync is enable for the game display.
     *
     * @param enable true if vsync is enabled; false if not.
     */
    static void setVsync(bool enable);

    /**
     * Gets the orientation angle the device is currently in.
     * 
     * @return The orientation angle.
     */
    static int getOrientationAngle();

    /**
     * Indicates whether the platform supports an accelerometer.
     * 
     * @return true if the accelerometer is supported, false otherwise.
     */
    static bool isAccelerometerSupported();

    /**
     * Gets the platform accelerometer pitch and roll.
     * 
     * @param pitch The accelerometer pitch.
     * @param roll The accelerometer roll.
     */
    static void getAccelerometerPitchAndRoll(float* pitch, float* roll);

    /**
     * Swaps the 
     */
    static void swapBuffers();

private:

    /**
     * Constructor.
     */
    Platform(Game* game);

    /**
     * Constructor.
     */
    Platform(const Platform& copy);

    Game* _game;

};

}

#endif
