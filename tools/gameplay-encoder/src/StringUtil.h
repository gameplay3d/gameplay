#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

namespace gameplay
{

/**
 * Returns true if the given string starts with the prefix.
 */
bool startsWith(const char* str, const char* prefix, bool ignoreCase = true);

/**
 * Returns true if the given string ends with the suffix.
 */
bool endsWith(const char* str, const char* suffix, bool ignoreCase = true);
bool endsWith(const std::string& str, const char* suffix, bool ignoreCase = true);

/**
 * Return true if the strings are equal. Case sensitive.
 */
bool equals(const std::string& a, const char* b);
bool equals(const std::string& a, const std::string& b);

/**
 * Returns true if the strings are equal. Case insensitive.
 */
bool equalsIgnoreCase(const std::string& a, const char* b);

/**
 * Returns the filename from the given real path.
 */
std::string getFilenameFromFilePath(const std::string& filepath);

std::string getFilenameNoExt(const std::string& filename);

}

#endif
