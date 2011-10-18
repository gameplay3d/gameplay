/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
// A home for commonly used utility functions. These are mostly for internal DOM
// use, but the automated tests use some of these functions, so we'll export
// them.
#ifndef daeUtils_h
#define daeUtils_h

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <dae/daePlatform.h>

namespace cdom {
	// System type info. We only need to distinguish between Posix and Winodws for now.
	enum systemType {
		Posix,
		Windows
	};

	// Get the system type at runtime.
	DLLSPEC systemType getSystemType();
	
	// String replace function. Usage: replace("abcdef", "cd", "12") --> "ab12ef".
	DLLSPEC std::string replace(const std::string& s, 
	                            const std::string& replace, 
	                            const std::string& replaceWith);

    // Removes whitespaces (" \t\f\v\n\r") at the beginning and the end of str.
    // If str consists of whitespaces only it will be erased.
    // Usage:
    //   trimWhitespaces("   a b") --> "a b"
    //   trimWhitespaces("a b   ") --> "a b"
    //   trimWhitespaces("   a b   ") --> "a b"
    //   trimWhitespaces("      ") --> ""
    DLLSPEC void trimWhitespaces(std::string& str);

	// Usage:
	//   tokenize("this/is some#text", "/#", true) --> ("this" "/" "is some" "#" "text")
	//   tokenize("this is some text", " ", false) --> ("this" "is" "some" "text")
	DLLSPEC std::list<std::string> tokenize(const std::string& s,
	                                        const std::string& separators,
	                                        bool separatorsInResult = false);
	// Same as the previous function, but returns the result via a parameter to avoid an object copy.
	DLLSPEC void tokenize(const std::string& s,
	                      const std::string& separators,
	                      /* out */ std::list<std::string>& tokens,
	                      bool separatorsInResult = false);

	typedef std::list<std::string>::iterator tokenIter;
	
	DLLSPEC std::vector<std::string> makeStringArray(const char* s, ...);
	DLLSPEC std::list<std::string> makeStringList(const char* s, ...);

	DLLSPEC std::string getCurrentDir();
	DLLSPEC std::string getCurrentDirAsUri();

    // Returns platform specific file separator.
    // \ on windows
    // / on other platforms
    DLLSPEC char getFileSeparator();

#ifndef NO_BOOST
    // Returns system wide temporary directory.
    // Reads environment variable TMP.
    DLLSPEC const std::string& getSystemTmpDir();

    // Returns a filename obtained via tmpnam().
    // On systems where tmpnam()'s result is preceded
    // with a directory, that directory is cutoff.
    DLLSPEC std::string getRandomFileName();

    // Returns getSystemTmpDir() appended with a randomly
    // generated directory name.
    // This directory will be deleted when DAE gets destroyed.
    DLLSPEC const std::string& getSafeTmpDir();
#endif //NO_BOOST

    DLLSPEC int strcasecmp(const char* str1, const char* str2);
	DLLSPEC std::string tolower(const std::string& s);

	// Disable VS warning
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4267)
#endif
	template<typename T>
	std::string toString(const T& val) {
		std::ostringstream stream;
		stream << val;
		return stream.str();
	}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#endif
