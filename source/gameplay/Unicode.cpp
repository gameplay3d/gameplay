#include "Unicode.h"
#if GP_PLATFORM_WINDOWS
#   include <codecvt>
#   include <locale>
#else
#   include <iconv.h>
#endif

namespace gameplay
{
#if GP_PLATFORM_WINDOWS
class Locale
{
public:
    Locale(const char* localeName)
    {
        locale = _create_locale(LC_ALL, localeName);
    }

    ~Locale()
    {
        _free_locale(locale);
    }

    _locale_t get_locale()
    {
        return locale;
    }

private:
    _locale_t locale;
};

static inline auto _get_system_default_locale()
{
    static Locale locale("");
    return locale.get_locale();
}

static inline auto& _get_wide_converter()
{
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter(
        Unicode::WIDE_TO_UTF8_FAILURE, Unicode::UTF8_TO_WIDE_FAILURE);
    GP_IGNOREWARNING_MSC_POP
    return converter;
}

std::string Unicode::convert_wide_to_utf8(const wchar_t* wide)
{
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    return _get_wide_converter().to_bytes(wide);
    GP_IGNOREWARNING_MSC_POP
}

std::string Unicode::convert_wide_to_utf8(const std::wstring& wide)
{
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    return _get_wide_converter().to_bytes(wide);
    GP_IGNOREWARNING_MSC_POP
}

std::wstring Unicode::convert_utf8_to_wide(const char* utf8)
{
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    return _get_wide_converter().from_bytes(utf8);
    GP_IGNOREWARNING_MSC_POP
}

std::wstring Unicode::convert_utf8_to_wide(const std::string& utf8)
{
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    return _get_wide_converter().from_bytes(utf8);
    GP_IGNOREWARNING_MSC_POP
}

int Unicode::compare_wide_strings_case_insensitive(const wchar_t* str1, const wchar_t* str2)
{
    return _wcsicmp_l(str1, str2, _get_system_default_locale());
}

int Unicode::compare_wide_strings_case_insensitive(const std::wstring& str1, const std::wstring& str2)
{
    return compare_wide_strings_case_insensitive(str1.c_str(), str2.c_str());
}

std::wstring Unicode::convert_wide_string_to_uppercase(const std::wstring& str)
{
    std::wstring result = str;
    _wcsupr_s_l(&result[0], result.size() + 1, _get_system_default_locale());
    return result;
}

void Unicode::convert_wide_string_to_uppercase_in_place(std::wstring& str)
{
    _wcsupr_s_l(&str[0], str.size() + 1, _get_system_default_locale());
}

std::wstring Unicode::convert_wide_string_to_lowercase(const std::wstring& str)
{
    std::wstring result = str;
    _wcslwr_s_l(&result[0], result.size() + 1, _get_system_default_locale());
    return result;
}

void Unicode::convert_wide_string_to_cowercase_in_place(std::wstring& string)
{
    _wcslwr_s_l(&string[0], string.size() + 1, _get_system_default_locale());
}
#endif

std::string Unicode::convert_utf32_to_utf8(uint32_t codepoint)
{
    std::string u8str;
#if GP_PLATFORM_WINDOWS
    GP_IGNOREWARNING_MSC_WITH_PUSH(4996)
    static std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> converter(UTF32_TO_UTF8_FAILURE);
    u8str = converter.to_bytes(codepoint);
    GP_IGNOREWARNING_MSC_POP
#else
    char* input = reinterpret_cast<char*>(&codepoint);
    size_t in_size = sizeof(uint32_t);
    char output_buffer[5] = { '\0' };
    char* output = output_buffer;
    size_t out_size = sizeof(output_buffer) - 1;
    iconv_t cd = iconv_open("UTF-8", "UTF-32");
    size_t ret = iconv(cd, &input, &in_size, &output, &out_size);
    iconv_close(cd);
    if (ret == (size_t)-1)
    {
        u8str = UTF32_TO_UTF8_FAILURE;
    }
    else
    {
        u8str = std::string(output_buffer);
    }
#endif
    return u8str;
}

}
