#include "Base.h"
#include "FileSystem.h"
#include "Properties.h"
#include "Stream.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    #include <direct.h>
    #define gp_stat _stat
    #define gp_stat_struct struct stat
#else
    #define __EXT_POSIX2
    #include <libgen.h>
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

#ifdef __ANDROID__
#include <unistd.h>

static void makepath(std::string path, int mode)
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

/**
 * Returns true if the file exists in the android read-only asset directory.
 */
static bool androidFileExists(const char* filePath)
{
    AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
    if (asset)
    {
        int lenght = AAsset_getLength(asset);
        AAsset_close(asset);
        return length > 0;
    }
    return false;
}

#endif

/** @script{ignore} */
static std::string __resourcePath("./");
static std::map<std::string, std::string> __aliases;

/**
 * Gets the fully resolved path.
 * If the path is relative then it will be prefixed with the resource path.
 * Aliases will be converted to a relative path.
 * 
 * @param path The path to resolve.
 * @param fullPath The full resolved path. (out param)
 */
static void getFullPath(const char* path, std::string& fullPath)
{
    if (FileSystem::isAbsolutePath(path))
    {
        fullPath.assign(path);
    }
    else
    {
        fullPath.assign(__resourcePath);
        fullPath += FileSystem::resolvePath(path);
    }
}

/**
 * 
 * @script{ignore}
 */
class FileStream : public Stream
{
public:
    friend class FileSystem;
    
    ~FileStream();
    virtual bool canRead();
    virtual bool canWrite();
    virtual bool canSeek();
    virtual void close();
    virtual size_t read(void* ptr, size_t size, size_t count);
    virtual char* readLine(char* str, int num);
    virtual size_t write(const void* ptr, size_t size, size_t count);
    virtual bool eof();
    virtual size_t length();
    virtual long int position();
    virtual bool seek(long int offset, int origin);
    virtual bool rewind();

    static FileStream* create(const char* filePath, const char* mode);

private:
    FileStream(FILE* file);

private:
    FILE* _file;
    bool _canRead;
    bool _canWrite;
};

#ifdef __ANDROID__

/**
 * 
 * @script{ignore}
 */
class FileStreamAndroid : public Stream
{
public:
    friend class FileSystem;
    
    ~FileStreamAndroid();
    virtual bool canRead();
    virtual bool canWrite();
    virtual bool canSeek();
    virtual void close();
    virtual size_t read(void* ptr, size_t size, size_t count);
    virtual char* readLine(char* str, int num);
    virtual size_t write(const void* ptr, size_t size, size_t count);
    virtual bool eof();
    virtual size_t length();
    virtual long int position();
    virtual bool seek(long int offset, int origin);
    virtual bool rewind();

    static FileStreamAndroid* create(const char* filePath, const char* mode);

private:
    FileStreamAndroid(AAsset* asset);

private:
    AAsset* _asset;
};

#endif

/////////////////////////////

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
    bool result = false;

    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if (dir != NULL)
    {
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
        result = true;
    }

#ifdef __ANDROID__
    // List the files that are in the android APK at this path
    AAssetDir* assetDir = AAssetManager_openDir(__assetManager, dirPath);
    if (assetDir != NULL)
    {
        AAssetDir_rewind(assetDir);
        const char* file = NULL;
        while ((file = AAssetDir_getNextFileName(assetDir)) != NULL)
        {
            std::string filename(file);
            // Check if this file was already added to the list because it was copied to the SD card.
            if (find(files.begin(), files.end(), filename) == files.end())
            {
                files.push_back(filename);
            }
        }
        AAssetDir_close(assetDir);
        result = true;
    }
#endif

    return result;
#endif
}

bool FileSystem::fileExists(const char* filePath)
{
    GP_ASSERT(filePath);

#ifdef __ANDROID__
    if (androidFileExists(resolvePath(filePath)))
    {
        return true;
    }
#endif

    std::string fullPath;
    getFullPath(filePath, fullPath);

    gp_stat_struct s;

#ifdef WIN32
    if (!isAbsolutePath(filePath) && stat(fullPath.c_str(), &s) != 0)
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

Stream* FileSystem::open(const char* path, size_t mode)
{
    char modeStr[] = "rb";
    if ((mode & WRITE) != 0)
        modeStr[0] = 'w';
#ifdef __ANDROID__
    if ((mode & WRITE) != 0)
    {
        // Open a file on the SD card
        std::string fullPath(__resourcePath);
        fullPath += resolvePath(path);

        size_t index = fullPath.rfind('/');
        if (index != std::string::npos)
        {
            std::string directoryPath = fullPath.substr(0, index);
            struct stat s;
            if (stat(directoryPath.c_str(), &s) != 0)
                makepath(directoryPath, 0777);
        }
        return FileStream::create(fullPath.c_str(), modeStr);
    }
    else
    {
        // Open a file in the read-only asset directory
        return FileStreamAndroid::create(resolvePath(path), modeStr);
    }
#else
    std::string fullPath;
    getFullPath(path, fullPath);
    
#ifdef WIN32
    gp_stat_struct s;
    if (!isAbsolutePath(path) && stat(fullPath.c_str(), &s) != 0 && (mode & WRITE) == 0)
    {
        fullPath = __resourcePath;
        fullPath += "../../gameplay/";
        fullPath += path;
        
        int result = stat(fullPath.c_str(), &s);
        if (result != 0)
        {
            fullPath = __resourcePath;
            fullPath += "../gameplay/";
            fullPath += path;
            if (stat(fullPath.c_str(), &s) != 0)
            {
                return NULL;
            }
        }
    }
#endif
    FileStream* stream = FileStream::create(fullPath.c_str(), modeStr);
    return stream;
#endif
}

FILE* FileSystem::openFile(const char* filePath, const char* mode)
{
    GP_ASSERT(filePath);
    GP_ASSERT(mode);

    std::string fullPath;
    getFullPath(filePath, fullPath);

    createFileFromAsset(filePath);
    
    FILE* fp = fopen(fullPath.c_str(), mode);
    
#ifdef WIN32
    if (fp == NULL && !isAbsolutePath(filePath))
    {
        fullPath = __resourcePath;
        fullPath += "../../gameplay/";
        fullPath += filePath;
        
        fp = fopen(fullPath.c_str(), mode);
        if (!fp)
        {
            fullPath = __resourcePath;
            fullPath += "../gameplay/";
            fullPath += filePath;
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
    std::auto_ptr<Stream> stream(open(filePath));
    if (stream.get() == NULL)
    {
        GP_ERROR("Failed to load file: %s", filePath);
        return NULL;
    }
    size_t size = stream->length();

    // Read entire file contents.
    char* buffer = new char[size + 1];
    size_t read = stream->read(buffer, 1, size);
    if (read != size)
    {
        GP_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %u < %u).", filePath, read, size);
        SAFE_DELETE_ARRAY(buffer);
        return NULL;
    }

    // Force the character buffer to be NULL-terminated.
    buffer[size] = '\0';

    if (fileSize)
    {
        *fileSize = (int)size; 
    }
    return buffer;
}

bool FileSystem::isAbsolutePath(const char* filePath)
{
    if (filePath == 0 || filePath[0] == '\0')
        return false;
#ifdef WIN32
    if (filePath[1] != '\0')
    {
        char first = filePath[0];
        return (filePath[1] == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')));
    }
    return false;
#else
    return filePath[0] == '/';
#endif
}

void FileSystem::createFileFromAsset(const char* path)
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

std::string FileSystem::getDirectoryName(const char* path)
{
    if (path == NULL || strlen(path) == 0)
    {
        return "";
    }
#ifdef WIN32
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath(path, drive, dir, NULL, NULL);
    std::string dirname;
    size_t driveLength = strlen(drive);
    if (driveLength > 0)
    {
        dirname.reserve(driveLength + strlen(dir));
        dirname.append(drive);
        dirname.append(dir);
    }
    else
    {
        dirname.assign(dir);
    }
    std::replace(dirname.begin(), dirname.end(), '\\', '/');
    return dirname;
#else
    // dirname() modifies the input string so create a temp string
    std::string dirname;
    char* tempPath = new char[strlen(path) + 1];
    strcpy(tempPath, path);
    char* dir = ::dirname(tempPath);
    if (dir && strlen(dir) > 0)
    {
        dirname.assign(dir);
        // dirname() strips off the trailing '/' so add it back to be consistent with Windows
        dirname.append("/");
    }
    SAFE_DELETE_ARRAY(tempPath);
    return dirname;
#endif
}

std::string FileSystem::getExtension(const char* path)
{
    const char* str = strrchr(path, '.');
    if (str == NULL)
        return "";

    std::string ext;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i)
        ext += std::toupper(str[i]);

    return ext;
}

//////////////////

FileStream::FileStream(FILE* file)
    : _file(file), _canRead(false), _canWrite(false)
{
    
}

FileStream::~FileStream()
{
    if (_file)
    {
        close();
    }
}

FileStream* FileStream::create(const char* filePath, const char* mode)
{
    FILE* file = fopen(filePath, mode);
    if (file)
    {
        FileStream* stream = new FileStream(file);
        const char* s = mode;
        while (s != NULL && *s != '\0')
        {
            if (*s == 'r')
                stream->_canRead = true;
            else if (*s == 'w')
                stream->_canWrite = true;
            ++s;
        }

        return stream;
    }
    return NULL;
}

bool FileStream::canRead()
{
    return _file && _canRead;
}

bool FileStream::canWrite()
{
    return _file && _canWrite;
}

bool FileStream::canSeek()
{
    return _file != NULL;
}

void FileStream::close()
{
    if (_file)
        fclose(_file);
    _file = NULL;
}

size_t FileStream::read(void* ptr, size_t size, size_t count)
{
    if (!_file)
        return 0;
    return fread(ptr, size, count, _file);
}

char* FileStream::readLine(char* str, int num)
{
    if (!_file)
        return 0;
    return fgets(str, num, _file);
}

size_t FileStream::write(const void* ptr, size_t size, size_t count)
{
    if (!_file)
        return 0;
    return fwrite(ptr, size, count, _file);
}

bool FileStream::eof()
{
    if (!_file || feof(_file))
        return true;
    return ((size_t)position()) >= length();
}

size_t FileStream::length()
{
    size_t len = 0;
    if (canSeek())
    {
        long int pos = position();
        if (seek(0, SEEK_END))
        {
            len = position();
        }
        seek(pos, SEEK_SET);
    }
    return len;
}

long int FileStream::position()
{
    if (!_file)
        return -1;
    return ftell(_file);
}

bool FileStream::seek(long int offset, int origin)
{
    if (!_file)
        return false;
    return fseek(_file, offset, origin) == 0;
}

bool FileStream::rewind()
{
    if (canSeek())
    {
        ::rewind(_file);
        return true;
    }
    return false;
}

////////////////////////////////

#ifdef __ANDROID__

FileStreamAndroid::FileStreamAndroid(AAsset* asset)
    : _asset(asset)
{
}

FileStreamAndroid::~FileStreamAndroid()
{
    if (_asset)
        close();
}

FileStreamAndroid* FileStreamAndroid::create(const char* filePath, const char* mode)
{
    AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
    if (asset)
    {
        FileStreamAndroid* stream = new FileStreamAndroid(asset);
        return stream;
    }
    return NULL;
}

bool FileStreamAndroid::canRead()
{
    return true;
}

bool FileStreamAndroid::canWrite()
{
    return false;
}

bool FileStreamAndroid::canSeek()
{
    return true;
}

void FileStreamAndroid::close()
{
    if (_asset)
        AAsset_close(_asset);
    _asset = NULL;
}

size_t FileStreamAndroid::read(void* ptr, size_t size, size_t count)
{
    int result = AAsset_read(_asset, ptr, size * count);
    return result > 0 ? ((size_t)result) / size : 0;
}

char* FileStreamAndroid::readLine(char* str, int num)
{
    if (num <= 0)
        return NULL;
    char c = 0;
    size_t maxCharsToRead = num - 1;
    for (size_t i = 0; i < maxCharsToRead; ++i)
    {
        size_t result = read(&c, 1, 1);
        if (result != 1)
        {
            str[i] = '\0';
            break;
        }
        if (c == '\n')
        {
            str[i] = c;
            str[i + 1] = '\0';
            break;
        }
        else if(c == '\r')
        {
            str[i] = c;
            // next may be '\n'
            size_t pos = position();

            char nextChar = 0;
            if (read(&nextChar, 1, 1) != 1)
            {
                // no more characters
                str[i + 1] = '\0';
                break;
            }
            if (nextChar == '\n')
            {
                if (i == maxCharsToRead - 1)
                {
                    str[i + 1] = '\0';
                    break;
                }
                else
                {
                    str[i + 1] = nextChar;
                    str[i + 2] = '\0';
                    break;
                }
            }
            else
            {
                seek(pos, SEEK_SET);
                str[i + 1] = '\0';
                break;
            }
        }
        str[i] = c;
    }
    return str; // what if first read failed?
}

size_t FileStreamAndroid::write(const void* ptr, size_t size, size_t count)
{
    return 0;
}

bool FileStreamAndroid::eof()
{
    return position() >= length();
}

size_t FileStreamAndroid::length()
{
    return (size_t)AAsset_getLength(_asset);
}

long int FileStreamAndroid::position()
{
    return AAsset_getLength(_asset) - AAsset_getRemainingLength(_asset);
}

bool FileStreamAndroid::seek(long int offset, int origin)
{
    return AAsset_seek(_asset, offset, origin) != -1;
}

bool FileStreamAndroid::rewind()
{
    if (canSeek())
    {
        return AAsset_seek(_asset, 0, SEEK_SET) != -1;
    }
    return false;
}

#endif

}
