#pragma once

#include "Defines.h"
#include <cstring>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>

namespace gameplay
{
class GP_API StringUtils
{
public:
    /**
     * Checks if the string begins with the given prefix.
     *
     * @param str a non-null pointer to the 0-terminated string.
     * @param prefix a non-null pointer to the 0-terminated prefix.
     * @return true if the string begins with provided prefix, false otherwise.
     */
    static inline bool starts_with(const char* str, const char* prefix);

    /**
     * Checks if the string begins with the given prefix.
     *
     * @param str a non-null pointer to the 0-terminated string.
     * @param prefix a const reference the std::string prefix.
     * @return true if the string begins with provided prefix, false otherwise.
     */
    inline bool starts_with(const char* str, const std::string& prefix);

    /**
     * Checks if the string begins with the given prefix.
     *
     * @param str a const reference to the std::string object.
     * @param prefix a non-null pointer to the 0-terminated prefix.
     * @return true if the string begins with provided prefix, false otherwise.
     */
    static inline bool starts_with(const std::string& str, const char* prefix);

    /**
     * Checks if the string begins with the given prefix.
     *
     * @param str a const reference to the std::string object.
     * @param prefix a const reference to the std::string prefix.
     *
     * @return true if the string begins with provided prefix, false otherwise.
     */
    static inline bool starts_with(const std::string& str, const std::string& prefix);

    /**
     * Checks if the string ends with the given suffix.
     *
     * @param[in] str a non-null pointer to the 0-terminated string.
     * @param[in] prefix a non-null pointer to the 0-terminated suffix.
     *
     * @return true if the string ends with provided suffix, false otherwise.
     */
    static inline bool ends_with(const char* str, const char* suffix);

    /**
     * Checks if the string ends with the given suffix.
     *
     * @param str a non-null pointer to the 0-terminated string.
     * @param prefix a const reference to the std::string suffix.
     * @return true if the string ends with provided suffix, false otherwise.
     */
    static inline bool ends_with(const char* str, const std::string& suffix);

    /**
     * Checks if the string ends with the given suffix.
     *
     * @param str a const reference to the std::string object.
     * @param prefix a non-null pointer to the 0-terminated suffix.
     * @return true if the string ends with provided suffix, false otherwise.
     */
    static inline bool ends_with(const std::string& str, const char* suffix);

    /**
     * Checks if the string ends with the given suffix.
     *
     * @param str a const reference to the std::string object.
     * @param prefix a const reference to the std::string suffix.
     * @return true if the string ends with provided suffix, false otherwise.
     */
    static inline bool ends_with(const std::string& str, const std::string& suffix);

    /**
     * Splits a string with a specified delimter string into a std::vector or std::strings.
     *
     * @param str The string to split.
     * @param delimiter The delimter character to split on.
     * @return The vector of string that have been split.
     */
    static inline std::vector<std::string> split(const std::string& str, char delimiter);

    /**
     * Converts a string to lowercase
     *
     * @param str The string to be converted to lowercase.
     */
    static inline void to_lower(std::string& str);

    /**
     * Converts a string to uppercase
     *
     * @param str The string to be converted to uppercase.
     */
    static void to_upper(std::string& str);

    /**
     * Trims any white space to the left of a string.
     *
     * @param The string to trim.
     * @return The string with the trimed spaces.
     */
    static inline void ltrim(std::string &str);

    /**
     * Trims any white space to the right of a string.
     *
     * @param The string to trim.
     * @return The string with the trimed spaces.
     */
    static inline void rtrim(std::string &str);

    /**
     * Trims any white space to the lef and right of a string.
     *
     * note: Doesn't trim the space in the middle
     * 
     * @param The string to trim.
     * @return The string with the trimed spaces.
     */
    static inline void trim(std::string &s);
};

//////////////////////////////////////////////////////////////////////////////
// impl.

inline bool StringUtils::starts_with(const char* str, const char* prefix)
{
    GP_ASSERT(str != nullptr);
    GP_ASSERT(prefix != nullptr);

    if (*str == 0)
    {
        // empty string vs empty (or not) prefix
        return (*prefix == 0);
    }
    else if (*prefix == 0)
    {
        // non empty string with empty prefix
        return true;
    }
    size_t n2 = strlen(prefix);
    return (strncmp(str, prefix, n2) == 0);
}

inline bool StringUtils::starts_with(const char* str, const std::string& prefix)
{
    GP_ASSERT(str != nullptr);

    if (*str == 0)
    {
        // empty string vs empty (or not) prefix
        return (prefix.empty());
    }
    else if (prefix.empty())
    {
        // non empty string with empty prefix
        return true;
    }
    std::string::size_type n2 = prefix.length();
    std::string::size_type n1 = strnlen(str, static_cast<size_t>(n2 + 1));
    if (n1 < n2)
    {
        // string is shorter than prefix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (prefix.compare(0, n2, str, n2) == 0);
}

inline bool StringUtils::starts_with(const std::string& str, const char* prefix)
{
    GP_ASSERT(prefix != nullptr);
    if (str.empty())
    {
        // empty string vs empty (or not) prefix
        return (*prefix == 0);
    }
    else if (*prefix == 0)
    {
        // non empty string with empty prefix
        return true;
    }
    std::string::size_type n2 = strlen(prefix);
    std::string::size_type n1 = str.length();
    if (n1 < n2)
    {
        // string is shorter than prefix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (str.compare(0, n2, prefix) == 0);
}

inline bool StringUtils::starts_with(const std::string& str, const std::string& prefix)
{
    if (str.empty())
    {
        // empty string vs empty (or not) prefix
        return prefix.empty();
    }
    else if (prefix.empty())
    {
        // non empty string with empty prefix
        return true;
    }
    std::string::size_type n2 = prefix.length();
    std::string::size_type n1 = str.length();
    if (n1 < n2)
    {
        // string is shorter than prefix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (str.compare(0, n2, prefix) == 0);
}

inline bool StringUtils::ends_with(const char* str, const char* suffix)
{
    GP_ASSERT(str != nullptr);
    GP_ASSERT(suffix != nullptr);
    if (*str == 0)
    {
        // empty string vs empty (or not) suffix
        return (*suffix == 0);
    }
    else if (*suffix == 0)
    {
        // non empty string with empty suffix
        return true;
    }
    size_t n2 = strlen(suffix);
    size_t n1 = strlen(str);
    if (n1 < n2)
    {
        // string is shorter than suffix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (memcmp(str + n1 - n2, suffix, n2) == 0);
}

inline bool StringUtils::ends_with(const char* str, const std::string& suffix)
{
    GP_ASSERT(str != nullptr);
    if (*str == 0)
    {
        // empty string vs empty (or not) suffix
        return (suffix.empty());
    }
    else if (suffix.empty())
    {
        // non empty string with empty suffix
        return true;
    }
    std::string::size_type n2 = suffix.length();
    std::string::size_type n1 = strlen(str);
    if (n1 < n2)
    {
        // string is shorter than suffix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (suffix.compare(0, n2, str + n1 - n2, n2) == 0);
}

inline bool StringUtils::ends_with(const std::string& str, const char* suffix)
{
    GP_ASSERT(suffix != nullptr);
    if (str.empty())
    {
        // empty string vs empty (or not) suffix
        return (*suffix == 0);
    }
    else if (*suffix == 0)
    {
        // non empty string with empty suffix
        return true;
    }
    std::string::size_type n2 = strlen(suffix);
    std::string::size_type n1 = str.length();
    if (n1 < n2)
    {
        // string is shorter than suffix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (str.compare(n1 - n2, n2, suffix) == 0);
}

inline bool StringUtils::ends_with(const std::string& str, const std::string& suffix)
{
    if (str.empty())
    {
        // empty string vs empty (or not) prefix
        return suffix.empty();
    }
    else if (suffix.empty())
    {
        // non empty string with empty prefix
        return true;
    }
    std::string::size_type n2 = suffix.length();
    std::string::size_type n1 = str.length();
    if (n1 < n2)
    {
        // string is shorter than suffix
        return false;
    }
    GP_ASSERT(n2 > 0 && n1 >= n2);
    return (str.compare(n1 - n2, n2, suffix) == 0);
}

inline std::vector<std::string> StringUtils::split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/*void replace_all(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}*/

inline void StringUtils::to_lower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
        [](char ch) {  return static_cast<char>(std::tolower(ch)); });
}

inline void StringUtils::to_upper(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(),
        [](char ch) { return static_cast<char>(std::toupper(ch)); });
}

inline void StringUtils::ltrim(std::string& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(),
        [](char ch) { return !std::isspace(ch); }));
}

inline void StringUtils::rtrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
        [](char ch) { return !std::isspace(ch); }).base(), str.end());
}

inline void StringUtils::trim(std::string& str)
{
    ltrim(str);
    rtrim(str);
}
}
