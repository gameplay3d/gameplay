#include "Base.h"
#include "Generator.h"

//TRACK_MEMORY();

// Generated file list (extern from Base.h)
std::vector<string> generatedFiles;

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

void writeFile(const std::string& path, const std::string& text)
{
    generatedFiles.push_back(path);

    // Read in content of path to compare before writing
    bool changed = true;
    ifstream in(path.c_str());
    if (in.is_open())
    {
        changed = false;
        istringstream textStream(text, istringstream::in);
        string line1, line2;
        while (in.good() && textStream.good())
        {
            getline(in, line1);
            getline(textStream, line2);
            if (line1 != line2 || in.good() != textStream.good())
            {
                // Files differ
                changed = true;
                break;
            }
        }
        in.close();
    }

    if (changed)
    {
        ofstream o(path.c_str());
        o << text;
        o.close();
    }
}

int main(int argc, char** argv)
{
    // Ensure the user is calling the program correctly.
    if (argc < 2 || argc > 4)
    {
        printf("Usage: gameplay-luagen <doxygen-xml-input-directory> [output-directory] [binding-namespace]\n");
        exit(0);
    }

    // Generate the bindings.
    string* bindingNS = (argc >= 4) ? new string(argv[3]) : NULL;
    Generator::getInstance()->run(argv[1], (argc >= 3) ? argv[2] : argv[1], bindingNS);
    Generator::releaseInstance();
    SAFE_DELETE(bindingNS);

#ifdef WIN32
    system("pause");
#endif
    
    return 0;
}
