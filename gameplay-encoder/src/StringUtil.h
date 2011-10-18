#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>

namespace gameplay
{

bool endsWith(const char* str, const char* suffix, bool ignoreCase = true);

/**
 * Return true if the strings are equal. Case sensitive.
 */
bool equals(const std::string& a, const char* b);

/**
 * Returns true if the strings are equal. Case insensitive.
 */
bool equalsIgnoreCase(const std::string& a, const char* b);

}
#endif
