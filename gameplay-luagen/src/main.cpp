#include "Base.h"
#include "Generator.h"

//TRACK_MEMORY();

void printError(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
#ifdef WIN32
    int sz = vfprintf(stderr, format, argptr);
    if (sz > 0)
    {
        char* buf = new char[sz + 1];
        vsprintf(buf, format, argptr);
        buf[sz] = 0;
        OutputDebugStringA(buf);
        SAFE_DELETE_ARRAY(buf);
    }
#else
    vfprintf(stderr, format, argptr);
#endif
    va_end(argptr);
}

int main(int argc, char** argv)
{
    // Ensure the user is calling the program correctly.
    if (argc < 2 || argc > 3)
    {
        GP_ERROR("Usage: luagen <input-directory> [output-directory]");
    }

    // Generate the bindings.
    Generator::getInstance()->run(argv[1], (argc == 3) ? argv[2] : argv[1]);
    Generator::releaseInstance();

    system("pause");
    return 0;
}
