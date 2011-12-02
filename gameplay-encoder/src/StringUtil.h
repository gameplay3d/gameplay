#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

namespace gameplay
{

bool startsWith(const char* str, const char* prefix, bool ignoreCase = true);
bool endsWith(const char* str, const char* suffix, bool ignoreCase = true);

/**
 * Return true if the strings are equal. Case sensitive.
 */
bool equals(const std::string& a, const char* b);

/**
 * Returns true if the strings are equal. Case insensitive.
 */
bool equalsIgnoreCase(const std::string& a, const char* b);

std::string getFilenameFromFilePath(const std::string& filepath);

std::string getFilenameNoExt(const std::string& filename);

}

#endif
