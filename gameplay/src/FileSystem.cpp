#include "Base.h"
#include "FileSystem.h"
#include "Properties.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    #define gp_stat _stat
    #define gp_stat_struct struct stat
#else
    #include <dirent.h>
    #define gp_stat stat
    #define gp_stat_struct struct stat
#endif

#ifdef __ANDROID__
#include <android/asset_manager.h>
extern AAssetManager* __assetManager;
#endif

namespace gameplay
{

// Creates a file on the file system from the specified asset (Android-specific).
static void createFileFromAsset(const char* path);

#ifdef __ANDROID__
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
                GP_ERROR("Failed to create directory: '%s'", dirPath.c_str());
                return;
            }
        }
    }
    
    return;
}
#endif

/** @script{ignore} */
static std::string __resourcePath("./");
static std::map<std::string, std::string> __aliases;

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

void FileSystem::loadResourceAliases(const char* aliasFilePath)
{
    Properties* properties = Properties::create(aliasFilePath);
    if (properties)
    {
        Properties* aliases;
        while ((aliases = properties->getNextNamespace()) != NULL)
        {
            loadResourceAliases(aliases);
        }
    }
    SAFE_DELETE(properties);
}

void FileSystem::loadResourceAliases(Properties* properties)
{
    assert(properties);

    const char* name;
    while ((name = properties->getNextProperty()) != NULL)
    {
        __aliases[name] = properties->getString();
    }
}

const char* FileSystem::resolvePath(const char* path)
{
    GP_ASSERT(path);

    size_t len = strlen(path);
    if (len > 1 && path[0] == '@')
    {
        std::string alias(path + 1);
        std::map<std::string, std::string>::const_iterator itr = __aliases.find(alias);
        if (itr == __aliases.end())
            return path; // no matching alias found
        return itr->second.c_str();
    }

    return path;
}

bool FileSystem::listFiles(const char* dirPath, std::vector<std::string>& files)
{
#ifdef WIN32
    std::string path(FileSystem::getResourcePath());
    if (dirPath && strlen(dirPath) > 0)
    {
        path.append(dirPath);
    }
    path.append("/*");
    // Convert char to wchar
    std::basic_string<TCHAR> wPath;
    wPath.assign(path.begin(), path.end());

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(wPath.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        return false;
    }
    do
    {
        // Add to the list if this is not a directory
        if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
        {
            // Convert wchar to char
            std::basic_string<TCHAR> wfilename(FindFileData.cFileName);
            std::string filename;
            filename.assign(wfilename.begin(), wfilename.end());
            files.push_back(filename);
        }
    } while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);
    return true;
#else
    std::string path(FileSystem::getResourcePath());
    if (dirPath && strlen(dirPath) > 0)
    {
        path.append(dirPath);
    }
    path.append("/.");
    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if (!dir)
    {
        return false;
    }
    while ((dp = readdir(dir)) != NULL)
    {
        std::string filepath(path);
        filepath.append("/");
        filepath.append(dp->d_name);

        struct stat buf;
        if (!stat(filepath.c_str(), &buf))
        {
            // Add to the list if this is not a directory
            if (!S_ISDIR(buf.st_mode))
            {
                files.push_back(dp->d_name);
            }
        }
    }
    closedir(dir);
    return true;
#endif
}

bool FileSystem::fileExists(const char* filePath)
{
    GP_ASSERT(filePath);

    std::string fullPath(__resourcePath);
    fullPath += resolvePath(filePath);

    createFileFromAsset(filePath);

    gp_stat_struct s;

#ifdef WIN32
    if (stat(fullPath.c_str(), &s) != 0)
    {
        fullPath = __resourcePath;
        fullPath += "../../gameplay/";
        fullPath += filePath;
        
        int result = stat(fullPath.c_str(), &s);
        if (result != 0)
        {
            fullPath = __resourcePath;
            fullPath += "../gameplay/";
            fullPath += filePath;
            return stat(fullPath.c_str(), &s) == 0;
        }
    }
    return true;
#else
    return stat(fullPath.c_str(), &s) == 0;
#endif
}

FILE* FileSystem::openFile(const char* path, const char* mode)
{
    GP_ASSERT(path);
    GP_ASSERT(mode);

    std::string fullPath(__resourcePath);
    fullPath += resolvePath(path);

    createFileFromAsset(path);
    
    FILE* fp = fopen(fullPath.c_str(), mode);
    
#ifdef WIN32
    if (fp == NULL)
    {
        fullPath = __resourcePath;
        fullPath += "../../gameplay/";
        fullPath += path;
        
        fp = fopen(fullPath.c_str(), mode);
        if (!fp)
        {
            fullPath = __resourcePath;
            fullPath += "../gameplay/";
            fullPath += path;
            fp = fopen(fullPath.c_str(), mode);
        }
    }
#endif

    return fp;
}

char* FileSystem::readAll(const char* filePath, int* fileSize)
{
    GP_ASSERT(filePath);

    // Open file for reading.
    FILE* file = openFile(filePath, "rb");
    if (file == NULL)
    {
        GP_ERROR("Failed to load file: %s", filePath);
        return NULL;
    }

    // Obtain file length.
    if (fseek(file, 0, SEEK_END) != 0)
    {
        GP_ERROR("Failed to seek to the end of the file '%s' to obtain the file length.", filePath);
        return NULL;
    }
    int size = (int)ftell(file);
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        GP_ERROR("Failed to seek to beginning of the file '%s' to begin reading in the entire file.", filePath);
        return NULL;
    }

    // Read entire file contents.
    char* buffer = new char[size + 1];
    int read = (int)fread(buffer, 1, size, file);
    if (read != size)
    {
        GP_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %d < %d).", filePath, (int)read, (int)size);
        SAFE_DELETE_ARRAY(buffer);
        return NULL;
    }

    // Force the character buffer to be NULL-terminated.
    buffer[size] = '\0';

    // Close file and return.
    if (fclose(file) != 0)
    {
        GP_ERROR("Failed to close file '%s'.", filePath);
    }

    if (fileSize)
    {
        *fileSize = size; 
    }
    return buffer;
}

bool FileSystem::isAbsolutePath(const char* filePath)
{
    if (filePath == 0 || filePath[0] == '\0')
        return false;
#ifdef WIN32
    if (strlen(filePath) >= 2)
    {
        char first = filePath[0];
        if (filePath[1] == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')))
            return true;
    }
    return false;
#else
    return filePath[0] == '/';
#endif
}

void createFileFromAsset(const char* path)
{
#ifdef __ANDROID__
    static std::set<std::string> upToDateAssets;

    GP_ASSERT(path);
    std::string fullPath(__resourcePath);
    std::string resolvedPath = FileSystem::resolvePath(path);
    fullPath += resolvedPath;

    std::string directoryPath = fullPath.substr(0, fullPath.rfind('/'));
    struct stat s;
    if (stat(directoryPath.c_str(), &s) != 0)
        makepath(directoryPath, 0777);

    // To ensure that the files on the file system corresponding to the assets in the APK bundle
    // are always up to date (and in sync), we copy them from the APK to the file system once
    // for each time the process (game) runs.
    if (upToDateAssets.find(fullPath) == upToDateAssets.end())
    {
        AAsset* asset = AAssetManager_open(__assetManager, resolvedPath.c_str(), AASSET_MODE_RANDOM);
        if (asset)
        {
            const void* data = AAsset_getBuffer(asset);
            int length = AAsset_getLength(asset);
            FILE* file = fopen(fullPath.c_str(), "wb");
            if (file != NULL)
            {
                int ret = fwrite(data, sizeof(unsigned char), length, file);
                if (fclose(file) != 0)
                {
                    GP_ERROR("Failed to close file on file system created from APK asset '%s'.", path);
                    return;
                }
                if (ret != length)
                {
                    GP_ERROR("Failed to write all data from APK asset '%s' to file on file system.", path);
                    return;
                }
            }
            else
            {
                GP_ERROR("Failed to create file on file system from APK asset '%s'.", path);
                return;
            }

            upToDateAssets.insert(fullPath);
        }
    }
#endif
}

}
