/**
 * FileSystem.cpp
 */

#include "Base.h"
#include "FileSystem.h"

namespace gameplay
{

static std::string __resourcePath("./");

FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::setResourcePath(const char* path)
{
    __resourcePath = path == NULL ? "" : path;
}

const char* FileSystem::getResourcePath()
{
    return __resourcePath.c_str();
}

FILE* FileSystem::openFile(const char* path, const char* mode)
{
    std::string fullPath(__resourcePath);
    fullPath += path;

    FILE* fp = fopen(fullPath.c_str(), mode);

#ifdef WIN32
    // HACK: For testing purposes, search the gameplay-resources folder as well.
    if (fp == NULL)
    {
        fullPath = __resourcePath;
        fullPath += "../../gameplay/";
        fullPath += path;
        fp = fopen(fullPath.c_str(), mode);
    }
#endif

    return fp;
}

char* FileSystem::readAll(const char* filePath, int* fileSize)
{
    // Open file for reading.
    FILE* file = openFile(filePath, "rb");
    if (file == NULL)
    {
        LOG_ERROR_VARG("Failed to load file: %s", filePath);
        return NULL;
    }

    // Obtain file length.
    fseek(file, 0, SEEK_END);
    int size = (int)ftell(file);
     fseek(file, 0, SEEK_SET);

    // Read entire file contents.
    char* buffer = new char[size + 1];
    int read = (int)fread(buffer, 1, size, file);
    assert(read == size);
    if (read != size)
    {
        LOG_ERROR_VARG("Read error for file: %s (%d < %d)", filePath, (int)read, (int)size);
        SAFE_DELETE_ARRAY(buffer);
        return NULL;
    }

    // Force the character buffer to be NULL-terminated.
    buffer[size] = '\0';

    // Close file and return.
    fclose(file);
    if (fileSize)
    {
        *fileSize = size; 
    }
    return buffer;
}


}
