#include "Base.h"
#include "FileSystem.h"
#include "Stream.h"
#include <sys/types.h>
#include <sys/stat.h>
#if GP_PLATFORM_WINDOWS
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
#if GP_PLATFORM_ANDROID
#include <android/asset_manager.h>
#include <unistd.h>
extern AAssetManager* __assetManager;
#endif

namespace gameplay
{

#if GP_PLATFORM_ANDROID
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
    for (size_t i = 0; i < dirs.size(); i++)
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


static bool androidFileExists(const char* filePath)
{
    AAsset* asset = AAssetManager_open(__assetManager, filePath, AASSET_MODE_RANDOM);
    if (asset)
    {
        int length = AAsset_getLength(asset);
        AAsset_close(asset);
        return length > 0;
    }
    return false;
}

#endif

static std::string __homePath(GP_ENGINE_HOME_PATH);
static std::map<std::string, std::string> __aliases;

static void getFullPath(const std::string& path, std::string& fullPath)
{
    if (FileSystem::isAbsolutePath(path))
    {
        fullPath.assign(path);
    }
    else
    {
        fullPath.assign(__homePath);
        fullPath += FileSystem::resolvePath(path);
    }
}

/**
 *  File stream.
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

#if GP_PLATFORM_ANDROID

/**
 * Android file stream.
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


FileSystem::FileSystem()
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::setHomePath(const std::string& path)
{
    if (path.length() > 0)
        __homePath =  path;
}

std::string FileSystem::getHomePath()
{
    return __homePath;
}

std::string FileSystem::getAbsolutePath(const std::string& path)
{
    #if GP_PLATFORM_WINDOWS
        char fullPath[_MAX_PATH]; 
        if (_fullpath(fullPath, path.c_str(), _MAX_PATH) != nullptr)
        {
            return std::string(fullPath);
        }
    #else
        char fullPath[PATH_MAX + 1];
        if (realpath(path.c_str(), fullPath))
        {
            return std::string(fullPath);
        }
    #endif
    return "";
}

std::string FileSystem::resolvePath(const std::string& path)
{
    if (path.length() > 1 && path.at(0) == '@')
    {
        std::string alias = path.substr(1);
        std::map<std::string, std::string>::const_iterator itr = __aliases.find(alias);
        if (itr == __aliases.end())
            return path; 
        return itr->second.c_str();
    }
    return __homePath + path;
}

bool FileSystem::listFiles(const std::string& dirPath, std::vector<std::string>& files)
{
#if GP_PLATFORM_WINDOWS
    std::string path(FileSystem::getHomePath());
    if (dirPath.length() > 0)
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
    std::string path(FileSystem::getHomePath());
    if (dirPath.length() > 0)
    {
        path.append(dirPath);
    }
    path.append("/.");
    bool result = false;

    struct dirent* dp;
    DIR* dir = opendir(path.c_str());
    if (dir != nullptr)
    {
        while ((dp = readdir(dir)) != nullptr)
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

#if GP_PLATFORM_ANDROID
    // List the files that are in the android APK at this path
    AAssetDir* assetDir = AAssetManager_openDir(__assetManager, dirPath.c_str());
    if (assetDir != nullptr)
    {
        AAssetDir_rewind(assetDir);
        const char* file = nullptr;
        while ((file = AAssetDir_getNextFileName(assetDir)) != nullptr)
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

bool FileSystem::fileExists(const std::string& filePath)
{
    std::string fullPath;
#if GP_PLATFORM_ANDROID
    fullPath = resolvePath(filePath);

    if (androidFileExists(fullPath.c_str()))
    {
        return true;
    }
#endif
    getFullPath(filePath, fullPath);
    gp_stat_struct s;
    return stat(fullPath.c_str(), &s) == 0;
}

Stream* FileSystem::open(const std::string& path, AccessFlags accessFlags)
{
    char modeStr[] = "rb";
    if (bool(accessFlags & AccessFlags::eWrite))
    {
        modeStr[0] = 'w';
    }
#if GP_PLATFORM_ANDROID
    std::string fullPath(__assetPath);
    fullPath += resolvePath(path);

    if (bool(accessFlags & AccessFlags::eWrite))
    {
        // Open a file on the SD card
        size_t index = fullPath.rfind('/');
        if (index != std::string::npos)
        {
            std::string directoryPath = fullPath.substr(0, index);
            gp_stat_struct s;
            if (stat(directoryPath.c_str(), &s) != 0)
                makepath(directoryPath, 0777);
        }
        return FileStream::create(fullPath.c_str(), modeStr);
    }
    else
    {
        // First try the SD card
        Stream* stream = FileStream::create(fullPath.c_str(), modeStr);
        if (!stream)
        {
            // Otherwise fall-back to assets loaded via the AssetManager
            fullPath = resolvePath(path);

            stream = FileStreamAndroid::create(fullPath.c_str(), modeStr);
        }
        return stream;
    }
#else
    std::string fullPath;
    getFullPath(path, fullPath);
    FileStream* stream = FileStream::create(fullPath.c_str(), modeStr);
    return stream;
#endif
}

std::string FileSystem::readAll(const std::string& filePath)
{
    // Open file for reading.
    std::unique_ptr<Stream> stream(open(filePath));
    if (stream.get() == nullptr)
    {
        GP_ERROR("Failed to load file: %s", filePath.c_str());
    }
    size_t size = stream->length();
    std::string result;
    result.resize(size);
    size_t read = stream->read(&result[0], 1, size);
    if (read != size)
    {
        GP_ERROR("Failed to read complete contents of file '%s' (amount read vs. file size: %u < %u).", filePath.c_str(), read, size);
    }
    return result;
}

bool FileSystem::isAbsolutePath(const std::string& filePath)
{
    char first = filePath.at(0);
#if GP_PLATFORM_WINDOWS
    char second = filePath.at(1);
    return (second == ':' && ((first >= 'a' && first <= 'z') || (first >= 'A' && first <= 'Z')));
#else
    return first == '/';
#endif
}

std::string FileSystem::getDirectoryPath(const std::string& path)
{
    if (path.length() == 0)
        return "";
#if GP_PLATFORM_WINDOWS
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    _splitpath(path.c_str(), drive, dir, nullptr, nullptr);
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
    char* tempPath = new char[strlen(path.c_str()) + 1];
    strcpy(tempPath, path.c_str());
    char* dir = ::dirname(tempPath);
    if (dir && strlen(dir) > 0)
    {
        dirname.assign(dir);
        // dirname() strips off the trailing '/' so add it back to be consistent with Windows
        dirname.append("/");
    }
    GP_SAFE_DELETE_ARRAY(tempPath);
    return dirname;
#endif
}

std::string FileSystem::getExtension(const std::string& path)
{
    std::string str = path.substr(path.find_last_of('.') + 1);
    if (str.length() == 0)
        return "";

    std::string ext;
    for (size_t i = 0; i < str.length(); ++i)
        ext += std::toupper(str.at(i));

    return ext;
}


FileStream::FileStream(FILE* file)
    : _file(file), _canRead(false), _canWrite(false)
{
}

FileStream::~FileStream()
{
    if (_file)
        close();
}

FileStream* FileStream::create(const char* filePath, const char* mode)
{
    FILE* file = fopen(filePath, mode);
    if (file)
    {
        FileStream* stream = new FileStream(file);
        const char* s = mode;
        while (s != nullptr && *s != '\0')
        {
            if (*s == 'r')
                stream->_canRead = true;
            else if (*s == 'w')
                stream->_canWrite = true;
            ++s;
        }

        return stream;
    }
    return nullptr;
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
    return _file != nullptr;
}

void FileStream::close()
{
    if (_file)
        fclose(_file);
    _file = nullptr;
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

#if GP_PLATFORM_ANDROID

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
    return nullptr;
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
    _asset = nullptr;
}

size_t FileStreamAndroid::read(void* ptr, size_t size, size_t count)
{
    int result = AAsset_read(_asset, ptr, size * count);
    return result > 0 ? ((size_t)result) / size : 0;
}

char* FileStreamAndroid::readLine(char* str, int num)
{
    if (num <= 0)
        return nullptr;
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
