#pragma once

#include "App.h"
#define SPDLOG_HEADER_ONLY
#include <spdlog/spdlog.h>

#define GP_LOG_TRACE SPDLOG_TRACE
#define GP_LOG_DEBUG SPDLOG_DEBUG
#define GP_LOG_INFO SPDLOG_INFO
#define GP_LOG_WARN SPDLOG_WARN
#define GP_LOG_ERROR SPDLOG_ERROR
#define GP_LOG_CRITICAL SPDLOG_CRITICAL

namespace gameplay
{
/**
 * Defines the system for managing and controlling logging output.
 *
 * Example:
 * 
 * const int x = 9;
 * 
 * GP_LOG_INFO("Hello Logger number: {}, x);
 *
 * To log message use:
 * 
 * @see GP_LOG_TRACE()
 * @see GP_LOG_DEBUG()
 * @see GP_LOG_INFO()
 * @see GP_LOG_WARN()
 * @see GP_LOG_ERROR()
 * @see GP_LOG_CRITICAL()
 */
class GP_API Logging
{
    friend class App;
public:

    enum class Level : uint32_t
    {
        LEVEL_TRACE,
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARN,
        LEVEL_ERROR,
        LEVEL_CRITICAL,
        LEVEL_OFF
    };

    /**
     * Parses the logging level from a string.
     */
    static Logging::Level parse_level(const char* str);

    /**
     * Sets the current logging level.
     *
     * @param level The logging level to be set to.
     */
    void set_level(Level level);

    /**
     * Gets the current logging level.
     *
     * @return The current logging level.
     */
    Level get_level() const;

private:
    Logging();
    ~Logging();
    void startup();
    void shutdown();
};
}

