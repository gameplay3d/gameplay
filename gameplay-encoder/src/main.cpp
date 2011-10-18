#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#ifdef WIN32
    #define PATH_MAX    _MAX_PATH
    #define realpath(A,B)    _fullpath(B,A,PATH_MAX)
#endif

#include "DAESceneEncoder.h"
#include "TTFFontEncoder.h"
#include "StringUtil.h"
#include "GPBDecoder.h"

using namespace std;
using namespace gameplay;

enum FileFormat
{
    FILEFORMAT_UNKNOWN,
    FILEFORMAT_DAE,
    FILEFORMAT_TTF,
    FILEFORMAT_GPB
};


/**
 * Gets the file format from the file path based on extension.
 */
FileFormat getFileFormat(const std::string& filename)
{
    // Extract the extension
    std::string ext = "";
    size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos)
        ext = filename.substr(pos + 1);
    
    // Match every supported extension with its format constant
    if (ext.compare("dae") == 0 || ext.compare("DAE") == 0)
    {
        return FILEFORMAT_DAE;
    }
    if (ext.compare("ttf") == 0 || ext.compare("TTF") == 0)
    {
        return FILEFORMAT_TTF;
    }
    if (ext.compare("gpb") == 0 || ext.compare("GPB") == 0)
    {
        return FILEFORMAT_GPB;
    }

    return FILEFORMAT_UNKNOWN;
}


/**
 * Tests if a file exists on the file system.
 */
bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}


/**
 * Replaces all instance of oldChar with newChar in str.
 */
void replace_char(char* str, char oldChar, char newChar)
{
    for (; *str != '\0'; str++)
    {
        if (*str == oldChar)
            *str = newChar;
    }
}

std::string getFileName(const std::string& filepath)
{
    size_t index1 = filepath.find_last_of('\\');
    size_t index2 = filepath.find_last_of('/');
    size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
    size_t length = filepath.length();
    string filename = filepath.substr(index + 1, length);
    length = filename.length();
    string output = filename.substr(0, (length-4));
    return output;
    
}


std::string getRealpath(const std::string& filepath)
{
    char path[PATH_MAX + 1]; /* not sure about the "+ 1" */
    realpath(filepath.c_str(), path);
    replace_char(path, '\\', '/');
    return std::string(path);
}


void usage()
{
    fprintf(stderr,"Usage: gameplay-encoder [options] <filepath>\n");
    fprintf(stderr,".dae file options:\n");
    fprintf(stderr," -i<id>\tFilter by node ID\n");
    fprintf(stderr," -t\tWrite text/xml\n");
    fprintf(stderr,".ttf file options:\n");
    fprintf(stderr,"-s<size of font> -p \n");
    exit(8);
}


/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments.
 *
 * usage:   gameplay-encoder[options] <file_list>
 * example: gameplay-encoder C:/assets/seymour.dae
 * example: gameplay-encoder -i boy seymour.dae
 *
 * @stod: Improve argument parsing.
 */
int main(int argc, const char** argv)
{
    std::string filepath;
    const char* id = NULL;
    unsigned int size = 0;
    bool fontpreview = false;
    bool text = false;

    if (argc <= 1)
    {
        usage();
    }

    if ( endsWith(argv[1], ".dae") || endsWith(argv[1], ".gpb"))
    {
        filepath = argv[1];
    }
    else
    {
        if (argc < 3)
        {
            usage();
        }
        else
        {   
            filepath = argv[argc-1];
        }
    }

    // Check if the file exists.
    if (!fileExists(filepath))
    {
        fprintf(stderr, "Error: File not found: %s\n", filepath.c_str());
        return -1;
    }
    // File exists
    fprintf(stderr, "Encoding file: %s\n", filepath.c_str());
    
    // Get the file format
    FileFormat fileFormat = getFileFormat(filepath);

    // Parse arguments
    while ((argc > 1) && (argv[1][0] == '-'))
    {
        switch (argv[1][1])
        {
            case 'i':
            case 'o':
                id = &argv[1][2];
                break;

            case 's':
                size = atoi(&argv[1][2]);
                break;
            
            case 'p':
                fontpreview = true;
                break;
            case 't':
                text = true;
                break;

            default:
                usage();
        }
        ++argv;
        --argc;
    }
    

    switch (fileFormat)
    {
    case FILEFORMAT_DAE:
        {
            std::string realpath = getRealpath(filepath);
            DAESceneEncoder daeEncoder;
            daeEncoder.write(realpath, id, text);
            break;
        }
    case FILEFORMAT_TTF:
        {
            std::string realpath = getRealpath(filepath);
            std::string id = getFileName(realpath);
            writeFont(realpath.c_str(), size, id.c_str(), fontpreview);
            break;
        }

    case FILEFORMAT_GPB:
        {
            std::string realpath = getRealpath(filepath);
            GPBDecoder decoder;
            decoder.readBinary(realpath);
            break;
        }
               
   default:
        {
            fprintf(stderr, "Error: Unsupported file format: %s\n", filepath.c_str());
            return -1;
        }
    }

    return 0;
}

