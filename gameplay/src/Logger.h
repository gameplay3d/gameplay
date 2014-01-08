#ifndef LOGGER_H_
#define LOGGER_H_

namespace gameplay
{

/**
 * Defines a class providing a basic logging system for a game.
 *
 * By default, this class logs messages using the gameplay::print function, which
 * is implemented in a platform dependent manner and typically prints to stderr
 * as well as to other possibly platform specific locations. Logging behavior
 * can be modified for a specific log level by passing a custom C or Lua logging
 * function to the Logger::set method. Logging can also be toggled using the
 * setEnabled method.
 */
class Logger
{
public:

    /** 
     * Enumeration of valid log levels.
     */
    enum Level
    {
        LEVEL_INFO = 0,
        LEVEL_WARN = 1,
        LEVEL_ERROR = 2
    };

    /**
     * Logs a message at the specified log level.
     *
     * This method accepts a variable argument list with the same formatting specification
     * as printf. Therefore, the message parameter can include any format specifiers that
     * are supported by printf.
     *
     * @param level Log level.
     * @param message Log message.
     */
    static void log(Level level, const char* message, ...);

    /**
     * Determines if logging is currently enabled for the given level.
     *
     * @param level Log level.
     *
     * @return True if logging is enabled for this level, or false if it is disabled.
     */
    static bool isEnabled(Level level);

    /**
     * Enables or disables logging at the given level.
     *
     * @param level Log level to enable or disable.
     * @param enabled True to enable the logger for the given level, false to disable it.
     */
    static void setEnabled(Level level, bool enabled);

    /**
     * Sets a C callback function to handle logging requests for the specified
     * log level.
     *
     * When a call to log is made with the given level, the specified C function will
     * be called to handle the request.
     *
     * Passing NULL for logFunction restores the default log behavior for this level.
     *
     * @param level Log level to set logging callback for.
     * @param logFunction Pointer to a C function to call for each log request at the given log level.
     * @script{ignore}
     */
    static void set(Level level, void (*logFunction) (Level, const char*));

    /**
     * Sets a Lua function as the log handler for the specified log level.
     *
     * When a call to log is made with the given level, the specified 
     * Lua function will be called to handle the request.
     *
     * Passing NULL for logFunction restores the default log behavior for this level.
     *
     * @param level Log level.
     * @param logFunction The Lua function to call for each log request at the given log level.
     */
    static void set(Level level, const char* logFunction);

private:

    struct State
    {
        State();
        void (*logFunctionC) (Level, const char*);
        const char* logFunctionLua;
        bool enabled;
    };

    /**
     * Hidden constructor.
     */
    Logger();

    /**
     * Hidden destructor.
     */
    ~Logger();

    /**
     * Hidden copy constructor.
     */
    Logger(const Logger& copy);

    /**
     * Hidden copy assignment operator.
     */
    Logger& operator=(const Logger&);

    static State _state[3];

};

}

#endif
