#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

#define LUA_GLOBAL_FILENAME "lua_Global"
#define LUA_ALL_BINDINGS_FILENAME "lua_all_bindings"
#define LUA_OBJECT "gameplay::ScriptUtil::LuaObject"
#define SCOPE_REPLACEMENT ""
#define SCOPE_REPLACEMENT_SIZE strlen(SCOPE_REPLACEMENT)
#define REF_CLASS_NAME "Ref"

#ifdef WIN32
#include <windows.h>
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4345)
#else
#include <dirent.h>
#endif

// Object deletion macro
#define SAFE_DELETE(x) \
    { \
        delete x; \
        x = NULL; \
    }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = NULL; \
    }

extern void printError(const char* format, ...);

extern void writeFile(const string& path, const string& text);

extern std::vector<string> generatedFiles;

// Current function macro.
#ifdef WIN32
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
#endif

// Assert macros.
#ifdef _DEBUG
#define GP_ASSERT(expression) do { \
    if (!(expression)) \
    { \
        printError("%s -- Assertion '" #expression "' failed.\n", __current__func__); \
        assert(expression); \
    } } while (0)
#else
#define GP_ASSERT(expression)
#endif

// Error macro.
#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do \
    { \
        printError("%s -- ", __current__func__); \
        printError(__VA_ARGS__); \
        printError("\n"); \
        assert(0); \
        std::exit(-1); \
    } while (0)
#endif

// Warning macro.
#define GP_WARN(...) do \
    { \
        printError("%s -- ", __current__func__); \
        printError(__VA_ARGS__); \
        printError("\n"); \
    } while (0)

#include "DebugNew.h"