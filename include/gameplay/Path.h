#pragma once

#include "Defines.h"

namespace gameplay
{
class Path;
inline Path operator/(const Path& left, const Path& right);
inline Path operator+(const Path& left, const Path& right);

/**
 * Paths are used for file system path manipulations.
 *
 * Paths are all in utf-8 encoding using forward slash as path separator.
 * Paths support implicit casting 'std::string'
 * Paths support explicit cast to a 'const char*' pointer.
 */
class GP_API Path
{
public:

    static constexpr const char* EMPTY_STRING = "";
    static constexpr char DOT_CHAR = '.';
    static constexpr const char* DOT_STRING = ".";
    static constexpr size_t DOT_STRING_LENGTH = 1;
    static constexpr const char* DOTDOT_STRING = "..";
    static constexpr size_t DOTDOT_STRING_LENGTH = 2;
    static constexpr char BACKWARD_SLASH_CHAR = '\\';
    static constexpr char FORWARD_SLASH_CHAR = '/';
    static constexpr const char* FORWARD_SLASH_STRING = "/";
    static constexpr size_t FORWARD_SLASH_STRING_LENGTH = 1;
    static constexpr char COLON_CHAR = ':';
    static constexpr char FIRST_LOWERCASE_LETTER = 'a';
    static constexpr char LAST_LOWERCASE_LETTER = 'z';

    /**
     * Constructor
     */
    Path();

    /**
     * Constructor.
     *
     * Creates a path from a possible non zero-terminated char array containing utf8 string.
     *
     * @param path A pointer to the data
     * @param pathLen the size of the data to be used to create the path object
     */
    Path(const char* path, size_t pathLen);

    /**
     * Constructor.
     *
     * Creates a path from a zero-terminated char array containing utf8 string.
     *
     * @param path A pointer to the char array
     */
    Path(const char* path);

    /**
     *
     * Constructor.
     *
     * Creates a new path from a utf8 std string
     *
     * @param path The source string
     */
    Path(std::string path);

    /**
     * Constructor (Copy).
     */
    Path(const Path& other);

    /**
     * Constructor  (Copy).
     */
    Path(Path&& other) noexcept;

    /*
     * Destructor
     */
    ~Path();

    /**
     * Copy operator.
     */
    Path& operator=(const Path& other);

    /**
     * Copy operator.
     */
    Path& operator=(Path&& other) noexcept;

    /**
     * Gets the std string representation of the path
     *
     * @return The std::string representation
     */
    std::string get_string() const;

    /**
     * Implicit conversion operator to the std string
     *
     * @return The std::string representation
     */
    operator std::string() const;

    /**
     * Get the const char pointer to the path data
     *
     * @return The c_str pointer to the start of the path data
     */
    const char* c_str() const;

    /**
     * Explicit conversion operator to the pointer to const char
     *
     * @return The c_str pointer to the start of the path data
     */
    explicit operator const char*() const
    {
        return c_str();
    }

    /**
     * operator==
     */
    bool operator==(const Path& other) const;

    /**
     * operator==
     */
    bool operator==(const std::string& other) const;

    /**
     * operator==
     */
    bool operator==(const char* other) const;

    /**
     * operator!=
     */
    bool operator!=(const Path& other) const;

    /**
     * operator!=
     */
    bool operator!=(const std::string& other) const;

    /**
     * operator!=
     */
    bool operator!=(const char* other) const;

    /**
     * Gets the length of the path.
     *
     * @return The length of the path
     */
    size_t len() const;

    /**
     * Clears the current path
     *
     * @return Reference to the current path
     */
    Path& clear();

    /**
     * Checks if the path is an empty string
     *
     * @return true if the path contains at least one character, false otherwise
     */
    bool is_empty() const;

    /**
     * Gets the filename component of the path, or an empty path object if there is no filename.
     *
     * @return The path object representing the filename
     */
    Path get_filename() const;

    /**
     * Gets the extension of the filename component of the path, including period (.), or an empty path object.
     *
     * @return The path object representing the extension
     */
    Path get_extension() const;

    /**
     * Gets the path to the parent directory, or an empty path object if there is no parent.
     *
     * @return The path object representing the parent directory
     */
    Path get_parent() const;

    /**
     * Gets the filename component of the path stripped of the extension,
     * or an empty path object if there is no filename.
     *
     * @return The path object representing the stem
     */
    Path get_stem() const;

    /**
     * Gets the root name in the path.
     *
     * @return The path object representing the root name
     */
    Path get_root_name() const;

    /**
     * Gets the relative part of the path (the part after optional root name and root directory).
     *
     * @return The path objects representing the relative part of the path
     */
    Path get_relative_part() const;

    /**
     * Gets the root directory if it's present in the path
     *
     * @return The path object representing the root directory
     */
    Path get_root_directory() const;

    /**
     * Checks if the path has a root directory
     *
     * @return true if the path has a root directory, false if otherwise.
     */
    bool has_root_directory() const noexcept;

    /**
     * Gets the root of the path. (root name + root directory, if present)
     *
     * @return The path object representing the root of the path
     */
    Path get_root() const;

    /**
     * Concatenates together two paths without checking for a separator and adding it
     *
     * @return The path object that has the unified data from the both paths
     */
    Path concat(const Path& concatedPart) const;

    /**
     * operator+=
     */
    Path& operator+=(const Path& path);

    /**
     * Joins together two path with checking for a separator and adding it if needed
     *
     * @ return The path object that has the unified data from the both paths
     */
    Path join(const Path& joinedPart) const;

    /**
     * operator/=
     */
    Path& operator/=(const Path& path);

    /**
     * Replaces the extension part of the current path with a new one.
     *
     * @param newExtension The path containing the data for the new extension
     * @return The current path with the extension changed.
     */
    Path& replace_extension(const Path& newExtension);

    /**
     * Gets the absolute path as normalizing the addition of the current path to the root.
     *
     * @return The path representing the constructed absolute path
     */
    Path get_absolute(const Path& root = "") const;

    /**
     * Checks if the current path is an absolute path.
     *
     * @return True if the current path is an absolute path, false otherwise.
     */
    bool is_absolute() const;

    /**
     * Gets the result of the normalization of the current path
     *
     * @return The new path representing the normalized current path
     */
    Path get_normalized() const;

    /**
     * Normalizes current path in place
     *
     * @return The reference to the current object
     */
    Path& normalize();

    /**
     * Gets the current path made relative to base.
     * 
     * The function does NOT normalize the paths prior to the operation.
     *
     * @param base The base path.
     * @return An empty path if it's impossible to match roots (different root names, different states of being
     * relative/absolute with a base path, not having a root directory while the base has it), otherwise a non-empty
     * relative path
     */
    Path get_relative(const Path& base) const noexcept;

    /**
     * Checks if the current path is the relative path.
     *
     * @return true if the current path is a relative path, false otherwise.
     */
    bool is_relative() const;

#if GP_PLATFORM_WINDOWS
    /**
     * Converts a utf-8 file path to Windows system file path.
     * Slashes are replaced with backslashes, long path prefix is appended if required.

     * @param path Input string to convert, in utf-8 encoding.
     * @return The wide string containing Windows system file path or empty string if conversion cannot be performed.
     */
    static std::wstring convert_utf8_to_windows_path(const std::string& path);

    /**
     * Converts Windows system file path to a utf-8 file path.
     * Backslashes are replaced with slashes, long path prefix is removed.
     *
     * @param path The input string to convert, in unicode (Windows native) encoding.
     * @return The utf-8 encoded file path or empty string if conversion cannot be performed.
     */
    static std::string convert_windows_to_utf8_path(const std::wstring& path);

    /**
     * Gets a Windows path string into a canonical form.
     * If it's not possible, original path is returned.
     *
     * @param path The windows system file path, in Unicode (Windows native) encoding.
     * @return The canonical form of the input path.
     */
    static std::wstring get_windows_canonical_path(const std::wstring& path);

    /**
     * Gets the full path and file name of the specified file.
     * If it's not possible, original path is returned.
     *
     * @param path The Windows system file path, in Unicode (Windows native) encoding.
     * @return The full path and file name of the input file.
     */
    static std::wstring get_windows_full_path(const std::wstring& path);

    /**
     * Fixes the windows path prefixes.
     *
     * @param str The windows system fix to be fixed.
     */
    static std::wstring fix_windows_path_prefixes(const std::wstring& str);

#endif

private:
    enum class PathTokenType
    {
        SLASH,
        ROOT_NAME,
        DOT,
        DOTDOT,
        NAME
    };
    static const char* _get_token_end(const char* bufferBegin, const char* bufferEnd, PathTokenType& resultType);
    struct PathPartDesc
    {
        const char* data;
        size_t size;
    };
    static Path _concat(const PathPartDesc* pathParts, size_t numParts);

    template <class Pred = std::equal_to<char>>
    static const char* _find_from_end(const char* data, size_t dataSize, char ch)
    {
        if (!data || dataSize == 0)
        {
            return nullptr;
        }
        --data;
        Pred pred;
        while (dataSize > 0)
        {
            if (pred(data[dataSize], ch))
            {
                return data + dataSize;
            }
            --dataSize;
        }
        return nullptr;
    }

    template <class Pred = std::equal_to<char>>
    static const char* _find_from_start(const char* data, size_t dataSize, char ch)
    {
        if (!data || dataSize == 0)
        {
            return nullptr;
        }
        Pred pred;
        for (const char* const dataEnd = data + dataSize; data != dataEnd; ++data)
        {
            if (pred(*data, ch))
            {
                return data;
            }
        }
        return nullptr;
    }

    const char* _get_filename_ptr() const;
    const char* _get_extension_ptr() const;
    const char* _get_root_name_end_ptr() const;
    const char* _get_relative_part_ptr() const;
    const char* _get_root_directory_end_ptr() const;
    void _sanitize_path();

    std::string _pathStr;
};


inline Path operator+(const Path& left, const Path& right)
{
    return left.concat(right);
}

inline Path operator/(const Path& left, const Path& right)
{
    return left.join(right);
}

inline Path get_parent_path(std::string path)
{
    return Path(std::move(path)).get_parent();
}

inline Path get_path_extension(std::string path)
{
    return Path(std::move(path)).get_extension();
}

inline Path get_path_stem(std::string path)
{
    return Path(std::move(path)).get_stem();
}

inline Path get_path_relative(std::string path, std::string base)
{
    return Path(std::move(path)).get_relative(std::move(base));
}

inline bool operator==(const std::string& left, const Path& right)
{
    return right == left;
}

inline bool operator==(const char* left, const Path& right)
{
    return right == left;
}

inline bool operator!=(const std::string& left, const Path& right)
{
    return right != left;
}

inline bool operator!=(const char* left, const Path& right)
{
    return right != left;
}

}
