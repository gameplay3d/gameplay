#include "Logging.h"
#include "FileSystem.h"
#include "Path.h"
#include "Config.h"
#include "StringUtils.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace gameplay
{

Logging::Logging()
{
    if (GP_DEBUG)
    {
        spdlog::set_level(spdlog::level::debug);
    }
    else
    {
        spdlog::set_level(spdlog::level::info);
    }
}

Logging::~Logging()
{
}

void Logging::startup()
{
    auto config = App::get_app()->get_config();

    // setup the file logger path from settings
    std::string logLevelStr = config->get_string("logging.level", "INFO");
    std::string logFileStr = config->get_string("logging.file", "");
    auto fs = App::get_app()->get_file_system();
    if (logFileStr.size() > 0)
    {
        std::string logPath = Path(fs->get_app_directory_path()).join(logFileStr);
        try 
        {
            // create a console and file logger
            std::vector<spdlog::sink_ptr> sinks;
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath));
            auto logger = std::make_shared<spdlog::logger>("gameplay_logger", begin(sinks), end(sinks));
            spdlog::register_logger(logger);
            spdlog::set_default_logger(logger);
            Logging::Level level = Logging::parse_level(logLevelStr.c_str());
            set_level(level);
            spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S.%e] [%^%l%$] %v");
            spdlog::flush_on(spdlog::level::info);
            //spdlog::flush_every(std::chrono::seconds(1));
            GP_LOG_INFO("=================================================================================");
            GP_LOG_INFO("LOG LEVEL: {}", logLevelStr.c_str());
            GP_LOG_INFO("LOG FILE: {}", logPath.c_str());
            GP_LOG_INFO("==================================================================================", logPath.c_str());
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            
            GP_LOG_WARN("Failed to initialize file logger: {}, {}", logPath.c_str(), ex.what());
        }
    }
}

void Logging::shutdown()
{
}

Logging::Level Logging::parse_level(const char* str)
{
    std::string levelStr = str;
    StringUtils::to_upper(levelStr);
    if (levelStr.compare("TRACE") == 0)
        return Level::LEVEL_TRACE;
    else if (levelStr.compare("DEBUG") == 0)
        return Level::LEVEL_DEBUG;
    else if (levelStr.compare("INFO") == 0)
        return Level::LEVEL_INFO;
    else if (levelStr.compare("WARN") == 0)
        return Level::LEVEL_WARN;
    else if (levelStr.compare("ERROR") == 0)
        return Level::LEVEL_ERROR;
    else if (levelStr.compare("CRITICAL") == 0)
        return Level::LEVEL_CRITICAL;
    else if (levelStr.compare("OFF") == 0)
        return Level::LEVEL_OFF;

    GP_LOG_WARN("Failed to parse Logging::Level str: {}", str);
    return Level::LEVEL_INFO;
}

void Logging::set_level(Logging::Level level)
{
    switch(level)
    {
    case Logging::Level::LEVEL_TRACE:
        spdlog::set_level(spdlog::level::trace);
        break;
    case Logging::Level::LEVEL_DEBUG:
        spdlog::set_level(spdlog::level::debug);
        break;
    case Logging::Level::LEVEL_INFO:
        spdlog::set_level(spdlog::level::info);
        break;
   case Logging::Level::LEVEL_WARN:
        spdlog::set_level(spdlog::level::warn);
        break;
    case Logging::Level::LEVEL_ERROR:
        spdlog::set_level(spdlog::level::err);
        break;
    case Logging::Level::LEVEL_CRITICAL:
        spdlog::set_level(spdlog::level::critical);
        break;
    case Logging::Level::LEVEL_OFF:
        spdlog::set_level(spdlog::level::off);
        break;
    }
}

Logging::Level Logging::get_level() const
{
    auto level = spdlog::get_level();
    switch(level)
    {
    case spdlog::level::trace:
        return Logging::Level::LEVEL_TRACE;
    case spdlog::level::debug:
        return Logging::Level::LEVEL_DEBUG;
    case spdlog::level::info:
        return Logging::Level::LEVEL_INFO;
    case spdlog::level::warn:
        return Logging::Level::LEVEL_WARN;
    case spdlog::level::err:
        return Logging::Level::LEVEL_ERROR;
    case spdlog::level::critical:
        return Logging::Level::LEVEL_CRITICAL;
    case spdlog::level::off:
        return Logging::Level::LEVEL_OFF;
    }
    return Logging::Level::LEVEL_INFO;
}

}
