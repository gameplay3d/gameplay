#include "Config.h"
#include "Logging.h"
#include "Path.h"
#include "FileSystem.h"
#include "StringUtils.h"
#include "Unicode.h"
#include <cpptoml.h>
#include <cstring>
#include <unordered_map>

#define GP_CONFIG_SETTING "config"
#define GP_CONFIG_EXT ".toml"
#define GP_CONFIG_FILE_DEFAULT "app.config.toml"
#define GP_CONFIG_DEV_DIR "../../../config"

namespace gameplay
{

struct Config::Impl
{
    void replace_quotes(std::string& str);
    void parse_from_args(int argc, char** argv);
    void parse_arg(const std::string& arg);
    void apply_from_args();
    std::unordered_map<std::string, std::string> args;
    std::string lastKey;
    std::string configPath = "";
    std::shared_ptr<cpptoml::table> root = nullptr;
};


 Config::Config()
{
     _impl = new Config::Impl();
}

Config::~Config()
{
    GP_SAFE_DELETE(_impl);
}

void Config::Impl::replace_quotes(std::string& str)
{
    if (!str.empty() && str[0] == '\'' && str[str.size() - 1] == '\'')
    {
        str[0] = '"';
        str[str.size() - 1] = '"';
    }
}

void Config::Impl::parse_arg(const std::string& arg)
{
    // check if there a key waiting to be filled with a value.
    if (!lastKey.empty())
    {
        if (StringUtils::starts_with(arg, "--"))
        {
            lastKey.clear();
        }
        else
        {
             std::string value = arg;
            replace_quotes(value);
            args[lastKey] = value;
            lastKey.clear();
            return;
        }
    }
    // process only keys starting with '--'
    static std::string prefix = "--";
    if (strncmp(arg.c_str(), prefix.c_str(), prefix.size()) == 0)
    {
        // split to key and value using the '=' sign
        std::size_t pos = arg.find("=");
        if (pos != std::string::npos)
        {
            const std::string key = arg.substr(prefix.size(), pos - prefix.size());
            std::string value = arg.substr(pos + 1, arg.size() - pos - 1);
            replace_quotes(value);
            args[key] = value;
        }
        else
        {
            // save key and wait for next value
            lastKey = arg.substr(prefix.size(), arg.size() - prefix.size());
        }
    }
}

void Config::Impl::parse_from_args(int argc, char** argv)
{
    if (argc < 1 || argv == nullptr)
    {
        return;
    }
    for (int i = 1; i < argc; i++)
    {
        if (argv[i])
        {
            std::string arg = argv[i];
            parse_arg(arg);
        }
    }
}

void Config::load(int argc, char** argv)
{
    // set the application 
    Path appExecutablePath = Path(argv[0]).get_absolute();
    auto fs = App::get_app()->get_file_system();
    fs->set_app_executable_path(appExecutablePath.c_str());

    // parse args 
    if (argc > 1)
    {
        _impl->parse_from_args(argc, argv);
    }

    // check for '--config' argument
    if (_impl->args.find(GP_CONFIG_SETTING) != _impl->args.end())
    {
        _impl->configPath = _impl->args[GP_CONFIG_SETTING];
    }

    // attempt to resolve the a config file in the following:
    //
    // 1st check the specified --config=<path>
    // 2nd check for app.config in the executable directory.
    // 3rd check the repo config path ../../../config/app.config
    bool configFound = true;
    std::string configExt = Path(_impl->configPath).get_extension();
    if (!fs->exists(_impl->configPath.c_str()) || configExt.compare(GP_CONFIG_EXT) != 0) 
    {
        _impl->configPath = Path(appExecutablePath).get_parent().join(Path(GP_CONFIG_FILE_DEFAULT)).get_absolute();
        if (!fs->exists(_impl->configPath.c_str()))
        {
            _impl->configPath = Path(appExecutablePath).get_parent().join(Path(GP_CONFIG_DEV_DIR)).join(Path(GP_CONFIG_FILE_DEFAULT)).get_absolute();
            if (!fs->exists(_impl->configPath.c_str()))
            {
                configFound = false;
            }
        }
    }

    // if we found a config file that exists
    if (configFound)
    {
        try
        {

            _impl->root = cpptoml::parse_file(_impl->configPath);
        }
        catch(const cpptoml::parse_exception& e)
        {
            GP_LOG_ERROR("Failed to parse app .config file: {}, error: {}", _impl->configPath.c_str(), e.what());
        }
    }
    else
    {
        _impl->root = cpptoml::make_table();
    }

    // apply our settings from command line arguments
    if (argc > 0)
    {
        _impl->args.erase(GP_CONFIG_SETTING);
        _impl->apply_from_args();
    }
}

/*bool Config::save()
{
    std::stringstream strBuffer;
    strBuffer << (*_table->root) << std::endl;
    auto fs = App::get_app()->get_file_system();
    std::string prefsPath = _configPath + GP_PREFS_EXT;
    auto file = fs->open_file(prefsPath.c_str(), FileMode::WRITE);
    if (!file)
    {
        GP_LOG_ERROR("Failed to save user .prefs file {}", prefsPath.c_str());
        return false;
    }
    std::string fileContents = strBuffer.str();
    size_t fileSize = fileContents.size() * sizeof(char);
    if (fs->write_file_chunk(file, fileContents.data(), fileSize) != fileSize)
    {
        GP_LOG_ERROR("Failed to write all data to settings file {}", prefsPath.c_str());

        return false;
    }
    fs->close_file(file);
   
    return true;
}*/

static std::pair<std::string, std::shared_ptr<cpptoml::table>> __get_key_and_table(std::shared_ptr<cpptoml::table> root, const char* key)
{
    std::pair<std::string, std::shared_ptr<cpptoml::table>> ret;
    std::string keyStr = key;
    std::string::size_type pos = keyStr.find_last_of('.');
    if (pos != std::string::npos)
    {
        ret.first = keyStr.substr(pos+1);
        ret.second = root->get_table(keyStr.substr(0, pos));
    }
    else
    {
       ret.first = keyStr;
       ret.second = root;
    }
    return ret;
}

void Config::Impl::apply_from_args()
{
    for (const auto& [k, value] : args)
    {
        if (strlen(k.c_str()) == 0)
            continue;

        auto kt = __get_key_and_table(root, k.c_str());
        auto key = kt.first;
        auto table = kt.second;
        if (value.compare("true") == 0)
        {
            table->insert(key, true);
        }
        else if (value.compare("false") == 0)
        {
            table->insert(key, false);
        }
        else if (value.at(0) > 47 && value.at(0) < 58)
        {
            // this is a number
            if (value.find(".") != std::string::npos)
            {
                // is a float number
                double v = std::stod(value);
                table->insert(key, v);
            }
            else
            {
                // is an int number
                int64_t v = std::stol(value);
                table->insert(key, v);
            }
        }
        else
        {
            table->insert(key, value);
        }
    }
}

size_t Config::get_array_size(const char* key)
{
    auto arr = _impl->root->get_array_qualified(key);
    if (!arr)
        return 0;
    return arr->get().size();
}

std::string Config::get_string(const char* key, const char* altValue)
{
    if(strchr(key, '.'))
    {
        return _impl->root->get_qualified_as<std::string>(key).value_or(altValue);
    }
    else
    {
        return _impl->root->get_as<std::string>(key).value_or(altValue);
    }
}

void Config::set_string(const char* key, const char* value)
{
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, value);
}

void Config::get_string_array(const char* key, std::string* arr, size_t arrSize)
{
    auto values = _impl->root->get_qualified_array_of<std::string>(key);
    GP_ASSERT(arr && arrSize >= values->size());
    size_t i = 0;
    for (const auto& value : *values)
    {
        arr[i] = value;
        i++;
    }
}

void Config::set_string_array(const char* key, const std::string* arr, size_t arrSize)
{
    auto new_array = cpptoml::make_array();
    for (size_t i = 0; i < arrSize; i++)
    {
        new_array->push_back(arr[i]);
    }
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, new_array);

}

bool Config::get_bool(const char* key, bool altValue)
{
    if(strchr(key, '.'))
    {
        return _impl->root->get_qualified_as<bool>(key).value_or(altValue);
    }
    else
    {
        return _impl->root->get_as<bool>(key).value_or(altValue);
    }
}

void Config::set_bool(const char* key, bool value)
{
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, value);
}

void Config::get_bool_array(const char* key, bool* arr, size_t arrSize)
{
    auto values = _impl->root->get_qualified_array_of<bool>(key);
    GP_ASSERT(arr && arrSize >= values->size());
    size_t i = 0;
    for (const auto& value : *values)
    {
        arr[i] = value;
        i++;
    }
}

void Config::set_bool_array(const char* key, const bool* arr, size_t arrSize)
{
    auto new_array = cpptoml::make_array();
    for (size_t i = 0; i < arrSize; i++)
    {
        new_array->push_back(arr[i]);
    }
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, new_array);
}

int Config::get_int(const char* key, int altValue)
{
    if(strchr(key, '.'))
    {
        return (int)_impl->root->get_qualified_as<int64_t>(key).value_or(altValue);
    }
    else
    {
        return (int)_impl->root->get_as<int64_t>(key).value_or(altValue);
    }
}

void Config::set_int(const char* key, int value)
{
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, (int64_t)value);
}

void Config::get_int_array(const char* key, int* arr, size_t arrSize)
{
    auto values = _impl->root->get_qualified_array_of<int64_t>(key);
    GP_ASSERT(arr && arrSize >= values->size());
    size_t i = 0;
    for (const auto& value : *values)
    {
        arr[i] = (int)value;
        i++;
    }
}

void Config::set_int_array(const char* key, const int* arr, size_t arrSize)
{
    auto new_array = cpptoml::make_array();
    for (size_t i = 0; i < arrSize; i++)
    {
        new_array->push_back((int64_t)arr[i]);
    }
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, new_array);
}

float Config::get_float(const char* key, float altValue)
{
    if(strchr(key, '.'))
    {
        return (float)_impl->root->get_qualified_as<double>(key).value_or((double)altValue);
    }
    else
    {
        return (float)_impl->root->get_as<double>(key).value_or((double)altValue);
    }
}

void Config::set_float(const char* key, float value)
{
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, (double)value);
}

void Config::get_float_array(const char* key, float* arr, size_t arrSize)
{
    auto values = _impl->root->get_qualified_array_of<double>(key);
    GP_ASSERT(arr && arrSize >= values->size());
    size_t i = 0;
    for (const auto& value : *values)
    {
        arr[i] = (float)value;
        i++;
    }
}

void Config::set_float_array(const char* key, const float* arr, size_t arrSize)
{
    auto new_array = cpptoml::make_array();
    for (size_t i = 0; i < arrSize; i++)
    {
        new_array->push_back((float)arr[i]);
    }
    auto kt = __get_key_and_table(_impl->root, key);
    kt.second->insert(kt.first, new_array);
}

void Config::for_each_table(const char* key,  Config::OnVisitTableFn fn, void* userPtr)
{
    std::shared_ptr<cpptoml::table> root = _impl->root;

    auto table_arr = root->get_table_array_qualified(key);
    for (const auto& table : *table_arr)
    {
        _impl->root = table;
        if (!fn(this, userPtr))
            break;
    }
    _impl->root = root;
}

}
