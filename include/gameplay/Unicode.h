#pragma once

#include "Defines.h"
#include <cstdint>
#include <string>

namespace gameplay
{
/**
 * Collection of unicode conversion utilities.
 *
 * Typically only used when using directly Windown API and other external libraries using unicode.
 *
 * Note:
 * Use the 'Path' class instead when converting windows file system paths instead of this class.
 * 
 * @see Path::convert_utf8_to_windows_path
 * @see Path::convert_windows_to_utf8_path
 */
class GP_API Unicode
{
public:
    /**
     * Failure string returned from 'convert_utf32_to_utf8' code point function,
     * usually because of invalid input code point
     *
     * @see convert_utf32_to_utf8
     */
    static inline const std::string UTF32_TO_UTF8_FAILURE = "[?]";

    /**
     * Convert a utf-32 codepoint to utf-8 encoded string
     *
     * @param codepoint The code point to convert, in utf-32 encoding
     * @return The utf-8 encoded version of codepoint.
     */
    static std::string convert_utf32_to_utf8(uint32_t codepoint);

#if GP_PLATFORM_WINDOWS
    /**
     * Failure string returned from 'convert_wide_to_utf8' functions when conversion
     * cannot be performed, usually because of invalid input string.
     *
     * @see WIDE_TO_UTF8_FAILURE
     */
    static inline const std::string WIDE_TO_UTF8_FAILURE = "[failure-converting-to-utf8]";

    /**
     * Converts a Windows wide string to utf-8 string
     *
     * @param wide Input string to convert, in Windows utf-16 encoding.
     * @return The utf-8 encoded version of wide or WIDE_TO_UTF8_FAILURE message if conversion cannot be performed.
     */
    static std::string convert_wide_to_utf8(const wchar_t* wide);

    /**
     * Converts a Windows wide string to utf-8 string
     *
     * @param wide Input string to convert, in Windows utf-16 encoding.
     * @return The utf-8 encoded version of wide or UNICODE_TO_UTF8_FAILURE message if conversion cannot be performed.
     */
    static std::string convert_wide_to_utf8(const std::wstring& wide);

    /**
     * Failure string returned from 'convert_utf8_to_wide' functions when conversion
     * cannot be performed, usually because of invalid input string.
     *
     * @see convert_utf8_to_wide
     */
    static inline const std::wstring UTF8_TO_WIDE_FAILURE = L"[failure-converting-to-wide]";

    /**
     * Converts a utf-8 encoded string to Windows wide character array
     *
     * @param utf8 Input string to convert, in UTF-8 encoding.
     * @return Wide string version of utf8 or UTF8_TO_WIDE_FAILURE message if conversion cannot be performed.
     */
    static std::wstring convert_utf8_to_wide(const char* utf8);

    /**
     * Converts a utf-8 encoded string to Windows wide character array
     * Do not use this function for Windows file path conversion! 
     *
     * @param utf8 Input string to convert, in UTF-8 encoding.
     * @return Wide string version of utf8 or UTF8_TO_WIDE_FAILURE message if conversion cannot be performed.
     */
    static std::wstring convert_utf8_to_wide(const std::string& utf8);
    
    /**
     * Performs a case-sensitive comparison of wide strings in Unicode (Windows) using system default locale.
     *
     * @param str1 First input string.
     * @param str2 Second input string.
     * @return < 0 if string1 less than str2, 0 if str1 identical to str2, > 0 if str1 greater than str2,
     *  INT_MAX on error.
     */
    static int compare_wide_strings_case_insensitive(const wchar_t* str1, const wchar_t* str2);

    /**
     * Performs a case-insensitive comparison of wide strings in Unicode (Windows) using system default locale.
     *
     * @param str1 The first input string.
     * @param str2 The second input string.
     * @return < 0 if string1 less than str2, 0 if str1 identical to str2, > 0 if str1 greater than str2,
     *  INT_MAX on error.
     */
    static int compare_wide_strings_case_insensitive(const std::wstring& str1, const std::wstring& str2);

    /**
     * Converts wide string in Unicode (Windows) to uppercase using system default locale.
     *
     * @param str Input string.
     * @return The uppercased string.
     */
    static std::wstring convert_wide_string_to_uppercase(const std::wstring& str);

    /**
     * Converts wide string in Unicode (Windows) to uppercase using system default locale, in-place version.
     *
     * @param str The string to be pass in and converted.
     */
    static void convert_wide_string_to_uppercase_in_place(std::wstring& str);

    /**
     * Converts wide string in Unicode (Windows) to lowercase using system default locale.
     *
     * @param str The input string.
     * @return The converted lowercased string.
     */
    static std::wstring convert_wide_string_to_lowercase(const std::wstring& str);

    /**
     * Converts wide string in Unicode (Windows) to lowercase using system default locale, in-place version.
     *
     * @param string The input and output string.
     */
    static void convert_wide_string_to_cowercase_in_place(std::wstring& string);
#endif
};


}
