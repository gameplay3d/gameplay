#pragma once

#include "App.h"

namespace gameplay
{

/**
 * Defines a way to get/set config values from the system.
 *
 * Config system uses .toml files with subset of features within this api.
 *
 * It supports string, bool, int and float, simple arrays and table arrays.
 *
 * For ease of developer usage for configuration purposes:
 * int are casted down from int64
 * float are casted down from double
 * all time/date configuration is not supported.
 * Only simple non-mixed array types are supported.
 *
 * For more information on toml markup see:
 * https://github.com/toml-lang/toml
 *
 * Sub-systems supporting config values should set their default values using set_xxx in their constuctors.
 * IF the config has specified values, they will be used instead of the system specified values.
 */
class GP_API Config
{
    friend class App;

public:
    /**
     * Gets a string value from config using the lookup key.
     *
     * @param key The lookup key for the string value.
     * @param altValue The alternative value to use if key not found.
     */
    std::string get_string(const char* key, const char* altValue);

    /**
     * Sets a (default) string value for config to use for the specified key.
     *
     * If the key has a value already loaded from config file. The value will not be set.
     *
     * @param key The key for which to set the value.
     * @param value The (default) value to be set into config.
     */
    void set_string(const char* key, const char* value);

    /**
     * Gets a boolean value from config using the lookup key.
     *
     * @param key The lookup key for the boolean value.
     * @param altValue The alternative value to use if key not found.
     */
    bool get_bool(const char* key, bool altValue);

    /**
     * Sets a (default) boolean value for config to use for the specified key.
     *
     * If the key has a value already loaded from config file. The value will not be set.
     *
     * @param key The key for which to set the value.
     * @param value The (default) value to be set into config.
     */
    void set_bool(const char* key, bool value);

    /**
     * Gets a integer value from config using the lookup key.
     *
     * @param key The lookup key for the integer value.
     * @param altValue The alternative value to use if key not found.
     */
    int get_int(const char* key, int altValue);

    /**
     * Sets a (default) integer value for config to use for the specified key.
     *
     * If the key has a value already loaded from config file. The value will not be set.
     *
     * @param key The key for which to set the value.
     * @param value The (default) value to be set into config.
     */
    void set_int(const char* key, int value);

    /**
     * Gets a floating-point value from config using the lookup key.
     *
     * @param key The lookup key for the floating-point value.
     * @param altValue The alternative value to use if key not found.
     */
    float get_float(const char* key, float altValue);

    /**
     * Sets a (default) floating-point value for config to use for the specified key.
     *
     * If the key has a value already loaded from config file. The value will not be set.
     *
     * @param key The key for which to set the value.
     * @param value The (default) value to be set into config.
     */
    void set_float(const char* key, float value);

    /**
     * Gets the size (number of elements) for the array at the specified key.
     *
     * @param key The key for which to lookup the array size.
     * @return The size of the array.
     */
    size_t get_array_size(const char* key);

    /**
     * Gets the string array (copies the contents) from config using the lookup key.
     *
     * @param key The key for which to get the string array.
     * @param arr The destination array to be copied into from config.
     * @param arrSize The size of the array passed.
     */
    void get_string_array(const char* key, std::string* arr, size_t arrSize);

    /**
     * Sets the string array (copies the contents) to config using the lookup key.
     *
     * @param key The key for which to set the string array.
     * @param arr The source array to be copied from.
     *  @param arrSize The size of the array passed.
     */
    void set_string_array(const char* key, const std::string* arr, size_t arrSize);

    /**
     * Gets the boolean array (copies the contents) from config using the lookup key.
     *
     * @param key The key for which to get the boolean array.
     * @param arr The destination array to be copied into from config.
     * @param arrSize The size of the array passed.
     */
    void get_bool_array(const char* key, bool* arr, size_t arrSize);

    /**
     * Sets the boolean array (copies the contents) to config using the lookup key.
     *
     * @param key The key for which to set the boolean array.
     * @param arr The source array to be copied from.
     *  @param arrSize The size of the array passed.
     */
    void set_bool_array(const char* key, const bool* arr, size_t arrSize);

    /**
     * Gets the integer array (copies the contents) from config using the lookup key.
     *
     * @param key The key for which to get the integer array.
     * @param arr The destination array to be copied into from config.
     * @param arrSize The size of the array passed.
     */
    void get_int_array(const char* key, int* arr, size_t arrSize);

    /**
     * Sets the integer array (copies the contents) to config using the lookup key.
     *
     * @param key The key for which to set the integer array.
     * @param arr The source array to be copied from.
     * @param arrSize The size of the array passed.
     */
    void set_int_array(const char* key, const int* arr, size_t arrSize);

    /**
     * Gets the floating-point array (copies the contents) from config using the lookup key.
     *
     * @param key The key for which to get the floating-point array.
     * @param arr The destination array to be copied into from config.
     * @param arrSize The size of the array passed.
     */
    void get_float_array(const char* key, float* arr, size_t arrSize);

    /**
     * Sets the floating-point array (copies the contents) to config using the lookup key.
     *
     * @param key The key for which to set the floating-point array.
     * @param arr The source array to be copied from.
     * @param arrSize The size of the array passed.
     */
    void set_float_array(const char* key, const float* arr, size_t arrSize);

    /**
     * Callback function used for iterating over array of tables for the specified key.
     *
     * This function is called for each visit to a table in the array of tables.
     *
     * @param The config interface to use for accessing internal table config.
     * @param userPtr The user pointer specified when for_each_table
     */
    typedef bool(*OnVisitTableFn)(Config* config, void* userPtr);

    /**
     * Iterates of the table of array at the specifed key.
     *
     * This function calls the callback function on each visit to table in the array.
     *
     * @param key The key for the table array to iterate over.
     * @param fn The function to call on each vist to an element in the table array.
     * @param userPtr The user pointer to be pass to each callback function call.
     */
    void for_each_table(const char* key, OnVisitTableFn fn, void* userPtr);

 private:
    Config();
	~Config();
    void load(int argc, char** argv);
    struct Impl;
    Impl* _impl = nullptr;
};

}

