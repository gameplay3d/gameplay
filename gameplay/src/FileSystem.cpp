#include "Base.h"
#include "FileSystem.h"

#ifdef __ANDROID__
extern AAssetManager* __assetManager;
#endif

namespace gameplay
{

#ifdef __ANDROID__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void makepath(std::string path, int mode)
{
    std::vector<std::string> dirs;
    while (path.length() > 0)
    {
        int index = path.find('/');
        std::string dir = (index == -1 ) ? path : path.substr(0, index);
        if (dir.length() > 0)
            dirs.push_back(dir);
        
        if (index + 1 >= path.length() || index == -1)
            break;
            
        path = path.substr(index + 1);
    }
    
    struct stat s;
    std::string dirPath;
    for (unsigned int i = 0; i < dirs.size(); i++)
    {
        dirPath += "/";
        dirPath += dirs[i];
        if (stat(dirPath.c_str(), &s) != 0)
        {
            // Directory does not exist.
            if (mkdir(dirPath.c_str(), 0777) != 0)
            {
                WARN_VARG("Failed to create directory: '%s'", dirPath.c_str());
                return;
            }
        }
    }
    
    return;
}
#endif

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
    
#ifdef __ANDROID__
    std::string directoryPath = fullPath.substr(0, fullPath.rfind('/'));
    struct stat s;
    if (stat(directoryPath.c_str(), &s) != 0)
        makepath(directoryPath.c_str(), 0777);

    if (stat(fullPath.c_str(), &s) != 0)
    {
        AAsset* asset = AAssetManager_open(__assetManager, path, AASSET_MODE_RANDOM);
        const void* data = AAsset_getBuffer(asset);
        int length = AAsset_getLength(asset);
        FILE* file = fopen(fullPath.c_str(), "wb");
        
        int ret = fwrite(data, sizeof(unsigned char), length, file);
        assert(ret == length);
        fclose(file);
    }
#endif
    
    FILE* fp = fopen(fullPath.c_str(), mode);
    
// Win32 doesn't support an asset or bundle definitions.
#ifdef WIN32
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
