#include "FileSystem.h"
#include "Logging.h"
#include "Path.h"
#include "Unicode.h"
#include <sys/stat.h>
#if GP_PLATFORM_WINDOWS
#   define NOMINMAX
#   include <Windows.h>
#   include <PathCch.h>
#   include <Shlwapi.h>
#   include <shellapi.h>
#   include <cwctype>
#   include <io.h>
#   include <winternl.h>
#else
#   include <sys/sendfile.h>
#   include <sys/types.h>
#   include <cerrno>
#   include <climits>
#   include <dirent.h>
#   include <fcntl.h>
#   include <libgen.h>
#   include <sstream>
#   include <unistd.h>
#   include <vector>
#endif
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#if GP_PLATFORM_WINDOWS
#   define strncasecmp(x, y, z) _strnicmp(x, y, z)
#   define MY_FILENO _fileno
#else
#   define MY_FILENO fileno
#endif

namespace gameplay
{

enum class FileOp
{
    NONE,
    READ,
    WRITE,
};

struct File
{
    FILE* handle;
    FileMode mode;
    FileStreamStatus streamStatus;
    FileOp lastOp;
};

typedef uint32_t WalkFlags;
constexpr WalkFlags WALK_FLAGS_RECURSIVE = (1 << 0);
constexpr WalkFlags WALK_FLAGS_SYMLINKS_ARE_FILES = (1 << 1);

// utility functions
#if GP_PLATFORM_WINDOWS
static const size_t PATH_BUFFER_LEN = 32768;
static void __convert_To_lower(std::wstring& str);
static std::string __winapi_errorcode_to_string(DWORD errorCode);
static time_t __filetime_to_timet(FILETIME const& ft);
typedef VisitAction (*OnVisitDirectoryItemFnWindows)(const std::wstring& path, DirectoryInfo* info, void* userPtr);
static VisitAction __walk_directory_windows(const std::wstring& pathAbsW, const std::wstring& parentW, OnVisitDirectoryItemFnWindows fn, void* userPtr,
                                           WalkFlags flags, std::list<std::wstring>* files, std::list<std::wstring>* directories);
#elif GP_PLATFORM_LINUX
static const size_t PATH_BUFFER_LEN = PATH_MAX + 1;;
std::vector<std::string> __split_and_fix_linux_path(const std::string& path);
static WalkAction __walk_directory_linux(const std::string& pathAbs, const std::string& parent, OnDirectoryItemFn fn , void* userPtr,
                                          WalkFlags flags, std::list<std::string>* files, std::list<std::string>* directories);
#endif
static std::string __resolve_path(FileSystem* fileSystem, const char* relativeOrAbsolutePath, const char* base);
static void __remove_duplicated_slashes(std::string& path);

struct FileSystem::Impl
{
    std::string appExecutablePath = "";
    std::string appDirectoryPath = "";
    void update_cwd(const char* cwd);
    char* cwd = nullptr;
};

//////////////////////////////////////////////////////////////////////////////
// impl.

FileSystem::FileSystem()
{
    _impl = new FileSystem::Impl();
}

FileSystem::~FileSystem()
{
    GP_SAFE_DELETE(_impl);
}

void FileSystem::set_app_executable_path(const char* path)
{
    _impl->appExecutablePath = path;
    _impl->appDirectoryPath = Path(path).get_parent();
}

const char* FileSystem::get_app_executable_path() const
{
    return _impl->appExecutablePath.c_str();
}

const char* FileSystem::get_app_directory_path() const
{
    return _impl->appDirectoryPath.c_str();
}

void FileSystem::Impl::update_cwd(const char* path)
{
    if (cwd == nullptr)
    {
        cwd = static_cast<char*>(GP_MALLOC(PATH_BUFFER_LEN * sizeof(char)));
        if (cwd == nullptr)
        {
            GP_LOG_ERROR("Failed to allocate a buffer to hold the current working directory.");
            return;
        }
    }
    size_t cwdLen = strlen(path);
    memcpy(cwd, path, (cwdLen + 1) * sizeof(char));
}

bool FileSystem::set_current_directory_path(const char* path)
{
#if GP_PLATFORM_WINDOWS
    std::string pathAbs;
    std::wstring winPath;
    BOOL success;
    pathAbs = __resolve_path(this, path, nullptr);
    winPath = Path::convert_utf8_to_windows_path(pathAbs);
    success = ::SetCurrentDirectoryW(winPath.c_str());
    if (!success)
    {
        GP_LOG_ERROR("Failed to set the current working directory to '{}'. error = " PRIu32 "{}", pathAbs.c_str(),::GetLastError());
        return false;
    }
    _impl->update_cwd(const_cast<char*>(pathAbs.c_str()));
    return true;
#elif GP_PLATFORM_LINUX
    int result;
    result = chdir(path);
    if (result == 0)
    {
        _update_cwd(path);
    }
    else
    {
        GP_LOG_ERROR("Failed to set the current working directory to '{}'.", path);
        return false;
    }
    return true;
#endif
}

const char* FileSystem::get_current_directory_path()
{
#if GP_PLATFORM_WINDOWS
    size_t lengthNeeded;
    std::vector<wchar_t> buf;
    lengthNeeded = MAX_PATH;
    do
    {
        buf.resize(lengthNeeded);
        lengthNeeded = ::GetCurrentDirectoryW(static_cast<DWORD>(buf.size()), buf.data());
        if (lengthNeeded == 0)
        {
            GP_LOG_ERROR("Failed to retrieve the working directory.");
            return 0;
        }
    } while (lengthNeeded > buf.size());

    std::string pathStr = Path::convert_windows_to_utf8_path(buf.data());
    _impl->update_cwd(pathStr.c_str());
    return _impl->cwd;

#elif GP_PLATFORM_LINUX
    char pathBuffer[PATH_MAX + 1];
    if (getcwd(pathBuffer, PATH_MAX) == nullptr)
    {
        GP_LOG_ERROR("Failed to retrieve the working directory.");
        return nullptr;
    }
    _update_cwd(pathBuffer);
    return _cwd;
#endif
}

bool FileSystem::exists(const char* path)
{
    if (!path)
    {
        GP_LOG_ERROR("Invalid path with nullptr");
        return false;
    }
    bool exists = false;
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    if (pathAbsW.length() == 2 && pathAbsW[1] == L':')
    {
        pathAbsW += L"\\";
    }
    if (pathAbsW.length() == 3 && pathAbsW[1] == L':' && pathAbsW[2] == L'\\')
    {
        // check for drive existence (e.g. D: or D:\)
        auto ret = GetDriveTypeW(pathAbsW.c_str());
        exists = ret != DRIVE_NO_ROOT_DIR && ret != DRIVE_UNKNOWN;
    }
    else
    {
        // check for directory for file existence
        exists = GetFileAttributesW(pathAbsW.c_str()) != INVALID_FILE_ATTRIBUTES;
    }
#else
    exists = access(pathAbs.c_str(), F_OK) == 0;
#endif
    return exists;
}

bool FileSystem::is_directory(const char* path)
{
    if (!path)
    {
        GP_LOG_ERROR("Invalid path with nullptr");
        return false;
    }
    bool isDir = false;
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    const DWORD attribs = ::GetFileAttributesW(pathAbsW.c_str());
    isDir = (attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat fileStat;
    if (stat(pathAbs.c_str(), &fileStat) == 0)
    {
        isDir = fileStat.st_mode & S_IFDIR;
    }
#endif
    return isDir;
}

bool FileSystem::is_writable(const char* path)
{
    if (!path)
    {
        GP_LOG_ERROR("Invalid path with nullptr");
        return false;
    }
    if (strncasecmp("file:", path, 5) == 0)
    {
        path += 5;
    }
    std::string pathAbs = __resolve_path(this, path, nullptr);
    bool isDirectory = is_directory(pathAbs.c_str());
    if (!isDirectory && !exists(pathAbs.c_str()))
    {
        // the file doesn't exist, we need to check the folder.
        pathAbs = Path(pathAbs).get_parent();
        // path was just the basename, so there will be no parent
        if (pathAbs.empty())
        {
            pathAbs = ".";
        }
        isDirectory = is_directory(pathAbs.c_str());
        if (!isDirectory)
        {
            // parent should be a directory. If it's not a directory, so we can't write file to it.
            return false;
        }
    }
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    {
        const SECURITY_INFORMATION securityInfo =
            OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
        DWORD length = 0;
        if (!::GetFileSecurityW(pathAbsW.c_str(), securityInfo, nullptr, 0, &length))
        {
            if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            {
                return false;
            }
        }
        std::unique_ptr<char[]> buffer(new (std::nothrow) char[length]);
        PSECURITY_DESCRIPTOR security = (PSECURITY_DESCRIPTOR)buffer.get();
        if (!security)
        {
            GP_LOG_CRITICAL("Failed memory allocation.");
            return false;
        }
        if (!::GetFileSecurity(pathAbsW.c_str(), securityInfo, security, length, &length))
        {
            return false;
        }
        HANDLE token;
        DWORD desiredAccess = TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ;
        if (!::OpenThreadToken(::GetCurrentThread(), desiredAccess, TRUE, &token))
        {
            if (!::OpenProcessToken(::GetCurrentProcess(), desiredAccess, &token))
            {
                ::CloseHandle(token);
                return false;
            }
        }
        bool result = false;
        HANDLE duplicateToken;
        if (::DuplicateToken(token, SecurityImpersonation, &duplicateToken))
        {
            PRIVILEGE_SET privileges = {};
            DWORD grantedAccess = 0;
            DWORD privilegesLength = sizeof(privileges);
            BOOL accessStatus = FALSE;
            GENERIC_MAPPING mapping;
            mapping.GenericRead = FILE_GENERIC_READ;
            mapping.GenericWrite = FILE_GENERIC_WRITE;
            mapping.GenericExecute = FILE_GENERIC_EXECUTE;
            mapping.GenericAll = FILE_ALL_ACCESS;
            DWORD accessMask = FILE_GENERIC_WRITE;
            ::MapGenericMask(&accessMask, &mapping);
            if (::AccessCheck(security, duplicateToken, accessMask, &mapping, &privileges, &privilegesLength, &grantedAccess, &accessStatus))
            {
                if (accessStatus)
                {
                    result = true;
                }
            }
            ::CloseHandle(duplicateToken);
        }
        ::CloseHandle(token);
        if (!result)
        {
            return false;
        }
    }
    if (!isDirectory)
    {
        DWORD attr = ::GetFileAttributesW(pathAbsW.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES)
        {
            if (attr & FILE_ATTRIBUTE_READONLY)
            {
                return false;
            }
        }
    }
#else
    return access(pathAbs.c_str(), W_OK) == 0;
#endif
    return true;
}

time_t FileSystem::get_create_time(const char* path)
{
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(pathAbsW.c_str(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &data))
    {
        DWORD err = GetLastError();
        GP_LOG_ERROR("Unable to get_create_time() for '%s' (GetFileAttributesExW error: %d/%s)", pathAbs.c_str(), err,
                       __winapi_errorcode_to_string(err).c_str());
        return 0;
    }
    SYSTEMTIME st;
    FileTimeToSystemTime(&data.ftCreationTime, &st);
    std::tm tm;
    tm.tm_sec = st.wSecond;
    tm.tm_min = st.wMinute;
    tm.tm_hour = st.wHour;
    tm.tm_mday = st.wDay;
    tm.tm_mon = st.wMonth - 1;
    tm.tm_year = st.wYear - 1900;
    tm.tm_isdst = -1;
    return std::mktime(&tm);
#elif GP_PLATFORM_LINUX
    struct stat info;
    stat(pathAbs.c_str(), &info);
    return info.st_ctime;
#endif
}

time_t FileSystem::get_mod_time(const char* path)
{
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExW(pathAbsW.c_str(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &data))
    {
        DWORD errorcode = GetLastError();
        GP_LOG_ERROR("Unable to get_mod_time() for: {} (GetFileAttributesExW failed: {}-{})", path, errorcode,
                       __winapi_errorcode_to_string(errorcode).c_str());
        return 0;
    }
    SYSTEMTIME st;
    FileTimeToSystemTime(&data.ftLastWriteTime, &st);
    std::tm tm;
    tm.tm_sec = st.wSecond;
    tm.tm_min = st.wMinute;
    tm.tm_hour = st.wHour;
    tm.tm_mday = st.wDay;
    tm.tm_mon = st.wMonth - 1;
    tm.tm_year = st.wYear - 1900;
    tm.tm_isdst = -1;
    return std::mktime(&tm);
#elif GP_PLATFORM_LINUX
    struct stat info;
    stat(path.c_str(), &info);
    return info.st_mtime;
#endif
}

std::string FileSystem::get_canonical_path(const char* path, const char* base)
{
    if (!path)
    {
        return "";
    }
    const std::string resolvedPath = __resolve_path(this, path, base);
    std::string canonicalPath;
#if GP_PLATFORM_WINDOWS
    const std::wstring pathNormW = Path::convert_utf8_to_windows_path(path);
    std::wstring pathCanonicalW = Path::get_windows_canonical_path(pathNormW);
    __convert_To_lower(pathCanonicalW);
    if (::GetFileAttributesW(pathCanonicalW.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        canonicalPath = Path::convert_windows_to_utf8_path(pathCanonicalW);
    }
#else
    char buffer[PATH_MAX];
    if (::realpath(path.c_str(), buffer) != nullptr)
    {
        canonicalPath = buffer;
    }
#endif
    __remove_duplicated_slashes(canonicalPath);
    return canonicalPath;
}

std::string FileSystem::make_temp_directory()
{
    std::string tempDir;
#if GP_PLATFORM_WINDOWS
    wchar_t buffer[L_tmpnam_s];
    _wtmpnam_s(buffer, L_tmpnam_s);
    bool success = ::CreateDirectoryW(buffer, nullptr);
    tempDir = Path::convert_windows_to_utf8_path(buffer);
#else
    char buffer[] = "/tmp/gameplay.XXXXXX";
    char* tempName = mkdtemp(buffer);
    bool success = (tempName != nullptr);
    tempDir = tempName;
#endif
    if (!success)
    {
        GP_LOG_ERROR("Failed to create temporary directory '{}'.", tempDir.c_str());
        return "";
    }
    else
    {
        return tempDir;
    }
}

bool FileSystem::make_directory(const char* path, bool createMissingDirectories)
{
    if (!createMissingDirectories)
    {
        std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
        std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
        bool success = ::CreateDirectoryW(pathAbsW.c_str(), nullptr);
#else
        bool success = mkdir(pathAbs.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#endif
        if (!success)
        {
            GP_LOG_ERROR("Failed to make directory '%s'.", path);
        }
        return success;
    }
    else
    {
        std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
        std::wstring pathAbsW = Path::convert_utf8_to_windows_path(get_canonical_path(pathAbs.c_str()));
        wchar_t* slash = &pathAbsW[0];
        if (pathAbsW.size() > 4 && wcsncmp(slash, L"\\\\?\\", 4) == 0)
        {
            // long path
            slash += 4;
        }
        if (wcslen(slash) > 3 && slash[1] == L':' && slash[2] == L'\\')
        {
            // absolute path with drive letter
            slash += 3;
        }
        bool done = false;
        while (!done)
        {
            // get path component
            slash += wcsspn(slash, L"\\");
            slash += wcscspn(slash, L"\\");
            done = (*slash == L'\0');
            *slash = L'\0';

            DWORD dwAttrib = ::GetFileAttributesW(pathAbsW.c_str());
            if (dwAttrib == INVALID_FILE_ATTRIBUTES)
            {
                // there is no such directory, try to create
                if (!::CreateDirectoryW(pathAbsW.c_str(), nullptr))
                {
                    GP_LOG_ERROR("Failed to make directory'{}'.",
                        Path::convert_windows_to_utf8_path(pathAbsW).c_str());
                    return false;
                }
            }
            else if (!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
            {
                // there is a file with the same name
                GP_LOG_ERROR("Failed to make directory '%s'. File already exists on this path.",
                    Path::convert_windows_to_utf8_path(pathAbsW).c_str());
                return false;
            }
            *slash = L'\\';
        }
    #else
        // posix realpath doesn't work for paths that don't exist, so the path has to be canonicalized manually
        std::string currentPath;
        for (auto& it : __split_and_fix_linux_path(pathAbs))
        {
            currentPath += it + '/';
            struct stat fileStat;
            if (stat(currentPath.c_str(), &fileStat) == -1)
            {
                // There is no such directory, try to create
                if (mkdir(currentPath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1)
                {
                    GP_LOG_ERROR("Failed to make directory '%s'", currentPath.c_str());
                    return false;
                }
            }
            else if (!(fileStat.st_mode & S_IFDIR))
            {
                GP_LOG_ERROR("Failed to make directory '%s' File already exists on this path", currentPath.c_str());
                return false;
            }
        }
    #endif
        return true;
    }
}

bool FileSystem::remove_directory(const char* path)
{
#if GP_PLATFORM_WINDOWS
    std::string pathAbs = __resolve_path(this, path, nullptr);
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    std::list<std::wstring> files;
    std::list<std::wstring> directories;
    __walk_directory_windows(
        pathAbsW, pathAbsW, nullptr, nullptr, WALK_FLAGS_RECURSIVE | WALK_FLAGS_SYMLINKS_ARE_FILES, &files, &directories);

    for (std::wstring& file : files)
    {
        if (::DeleteFileW(file.c_str()) == 0 && ::RemoveDirectoryW(file.c_str()) == 0)
        {
            GP_LOG_ERROR("Failed to delete file '%s'.", Path::convert_windows_to_utf8_path(file).c_str());
            return false;
        }
    }
    directories.emplace_front(pathAbsW);
    directories.reverse();
    for (std::wstring& directory : directories)
    {
        if (::RemoveDirectoryW(directory.c_str()) == 0)
        {
            GP_LOG_ERROR("Failed to delete directory '%s'.", Path::convert_windows_to_utf8_path(directory).c_str());
            return false;
        }
    }
    return true;

#elif GP_PLATFORM_LINUX
    std::list<std::string> files;
    std::list<std::string> directories;
    __walk_directory_linux(
        path, path, nullptr, nullptr, WALK_FLAGS_RECURSIVE | WALK_FLAGS_SYMLINKS_ARE_FILES, &files, &directories);

    for (std::string& file : files)
    {
        if (unlink(file.c_str()) != 0)
        {
            GP_LOG_ERROR("Failed to delete the file '%s'.", file.c_str());
            return false;
        }
    }
    directories.emplace_front(path);
    directories.reverse();
    for (std::string& directory : directories)
    {
        if (rmdir(directory.c_str()) != 0)
        {
            GP_LOG_ERROR("Failed to remove the directory '%s'.", directory.c_str());
            return false;
        }
    }
    return true;
#endif
}

bool FileSystem::remove_file(const char* path)
{
    bool success = false;
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    pathAbsW.push_back(L'\0');
    pathAbsW.push_back(L'\0');

    SHFILEOPSTRUCTW fileOperation;
    fileOperation.wFunc = FO_DELETE;
    fileOperation.pFrom = pathAbsW.c_str();
    fileOperation.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION;
    int result = ::SHFileOperationW(&fileOperation);
    if (result != 0)
    {
        GP_LOG_ERROR("Failed to delete file: '%s' SHFileOperationW failed (error code: %d)", path, result);
    }
    else
    {
        success = true;
    }
#elif GP_PLATFORM_LINUX
    success = remove(pathAbs.c_str()) == 0;
#endif
    return success;
}

bool FileSystem::move(const char* src, const char* dst)
{
    std::string srcAbs = __resolve_path(this, src, nullptr);
    std::string dstAbs = __resolve_path(this, dst, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring srcAbsW = Path::convert_utf8_to_windows_path(srcAbs);
    std::wstring dstAbsW = Path::convert_utf8_to_windows_path(dstAbs);
    if (!::MoveFileW(srcAbsW.c_str(), dstAbsW.c_str()))
    {
        GP_LOG_ERROR("Failed to move file: '%s' -> '%s' (error code %" PRIu32 ")", src, dst, ::GetLastError());
        return false;
    }
    return true;
#elif GP_PLATFORM_LINUX
    if (rename(srcAbs.c_str(), dstAbs.c_str()) < 0)
    {
        GP_LOG_ERROR("Failed to move file: '%s' -> '%s' (%s)", srcAbs.c_str(), dstAbs.c_str(), strerror(errno));
        return false;
    }
    return true;
#else
    return false;
#endif
}

bool FileSystem::copy(const char* src, const char* dst)
{
    std::string srcAbs = __resolve_path(this, src, nullptr);
    std::string dstAbs = __resolve_path(this, dst, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring srcAbsW = Path::convert_utf8_to_windows_path(srcAbs);
    std::wstring dstAbsW = Path::convert_utf8_to_windows_path(dstAbs);
    bool ok = ::CopyFileW(srcAbsW.c_str(), dstAbsW.c_str(), true);
    if (!ok)
    {
        GP_LOG_ERROR("Failed to copy file: '%s' -> '%s' (error code %" PRIu32 ")", src, dst, ::GetLastError());
    }
    return ok;
#elif GP_PLATFORM_LINUX
    int input = open(srcAbs.c_str(), O_RDONLY);
    if (input == -1)
    {
        GP_LOG_ERROR("Failed to copy file. Failed opening file '%s' for reading (%s)", srcAbs.c_str(), strerror(errno));
        return false;
    }
    bool success = true;
    off_t offset = 0;
    struct stat fileinfo = {};
    if (fstat(input, &fileinfo) < 0)
    {
        GP_LOG_ERROR("Failed to copy file. Failed to get file size for file '%s' (%s)", srcAbs.c_str(), strerror(errno));
    }
    int output = open(dstAbs.c_str(), O_WRONLY | O_CREAT | O_EXCL, fileinfo.st_mode);
    if (output == -1)
    {
        GP_LOG_ERROR("Failed to copy file. Failed to create file '%s' for writing (%s)", dstAbs.c_str(), strerror(errno));
        close(input);
        return false;
    }
    while (offset < fileinfo.st_size)
    {
        ssize_t result = sendfile(output, input, &offset, fileinfo.st_size);

        if (result == -1)
        {
            GP_LOG_ERROR("Failed to copy file: '%s' -> '%s' (%s)", srcAbs.c_str(), dstAbs.c_str(), strerror(errno));
            success = false;
            break;
        }
    }
    close(input);
    close(output);
    return success;
#else
    return false;
#endif
}

bool FileSystem::get_file_info(const char* path, FileInfo* info)
{
    std::string pathAbs = __resolve_path(this, path, nullptr);
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    WIN32_FILE_ATTRIBUTE_DATA winInfo = {};
    if (!::GetFileAttributesExW(pathAbsW.c_str(), GetFileExInfoStandard, &winInfo))
    {
        return false;
    }
    info->type = !(winInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? DirectoryItemType::FILE :
                                                                          DirectoryItemType::DIRECTORY;
    info->modTime = __filetime_to_timet(winInfo.ftLastWriteTime);
    info->createTime = __filetime_to_timet(winInfo.ftCreationTime);
    info->size = (size_t(winInfo.nFileSizeHigh) << 32) + winInfo.nFileSizeLow;
    info->symlink = !!(winInfo.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
    return true;
#elif GP_PLATFORM_LINUX
    struct stat buf;
    if (stat(path, &buf) != 0)
    {
        return false;
    }
    info->type = !S_ISDIR(buf.st_mode) ? DirectoryItemType::eFile : DirectoryItemType::eDirectory;
    info->modifiedTimestamp = buf.st_mtime;
    info->createdTimestamp = buf.st_ctime;
    info->size = size_t(buf.st_size);
    // Use lstat to determine if it's a link
    if (lstat(path, &buf) != 0)
    {
        info->isSymlink = false;
    }
    else
    {
        info->isSymlink = !!S_ISLNK(buf.st_mode);
    }
    return true;
#endif
}

File* FileSystem::open_file(const char* path, FileMode mode)
{
    if (!path)
    {
        GP_LOG_ERROR("Invalid path with nullptr.");
        return nullptr;
    }

    std::string pathAbs = __resolve_path(this, path, nullptr);
    const char* modeStr;
    switch (mode)
    {
    case FileMode::READ:
        modeStr = "rb";
        break;
    case FileMode::WRITE:
        modeStr = "wb";
        break;
    case FileMode::APPEND:
        modeStr = "ab";
        break;
    case FileMode::READ_WRITE:
        modeStr = exists(pathAbs.c_str()) ? "rb+" : "wb+";
        break;
    default:
        GP_LOG_ERROR("Unknown file mode %d.", static_cast<int>(mode));
        return nullptr;
    }
#if GP_PLATFORM_WINDOWS
    int shareMode = _SH_DENYNO;
    wchar_t modeW[64];
    MultiByteToWideChar(CP_UTF8, 0, modeStr, -1, modeW, GP_COUNTOF32(modeW));
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    FILE* handle = _wfsopen(pathAbsW.c_str(), modeW, shareMode);
#else
    FILE* handle = fopen(pathAbs.c_str(), modeStr);
#endif
    if (handle == nullptr)
    {
        if (errno != ENOENT)
            GP_LOG_WARN("Failed to open file '{}'. errno {}", pathAbs.c_str(), errno);
        else
            GP_LOG_WARN("File '{}' does not exist", pathAbs.c_str());
        return nullptr;
    }
    return new File{ handle, mode, FileStreamStatus::STREAM_OK, FileOp::NONE };
}

void FileSystem::close_file(File* file)
{
    GP_ASSERT(file);
    int res = fclose(file->handle);
    if (res)
    {
        GP_LOG_ERROR("Failed to close file");
    }
    delete file;
}

static void __get_file_stat(File* file, struct stat* info)
{
    // flush the stream so that the most up-to-date information is queried.  Without doing this,
    // the file size and modification time will not be returned correctly if there is still
    // buffered data on the stream.
    fflush(file->handle);
    fstat(MY_FILENO(file->handle), info);
}

size_t FileSystem::get_file_size(File* file)
{
    struct stat info;
    __get_file_stat(file, &info);
    return info.st_size;
}

time_t FileSystem::get_file_create_time(File* file)
{
    struct stat info;
    __get_file_stat(file, &info);
    return info.st_ctime;
}

time_t FileSystem::get_file_mod_time(File* file)
{
    struct stat info;
    __get_file_stat(file, &info);
    return info.st_mtime;
}

static void __resync_file_stream(FileSystem* fileSystem, File* file, FileOp newOp)
{
    FileOp lastOp = file->lastOp;
    // not in read/write mode -> nothing to do => succeed.
    if (file->mode != FileMode::READ_WRITE)
        return;

    file->lastOp = newOp;

    // the new operation does not need a resync given the previous operation => succeed.
    if (lastOp == FileOp::NONE || lastOp == newOp)
        return;

    // in read/write mode, each mix between a read and write operation must have an intervening
    // seek operation to prevent immediate failure.  The seek operation will perform a sync
    // operation on the file stream and cause the buffered state to be revalidated even if the
    // seek is just a no-op.  If the last operation was different from the new operation and
    // we're in read/write mode, we'll just do a no-op seek here to force the stream sync.
    fileSystem->set_file_pos(file, 0, FileWhence::CURRENT);
}

static void __update_file_operation_status(File* file, size_t result)
{
    // the read or write operation didn't fail => succeed.  Note that the operation may still have
    // reached a condition such as end-of-file by returning a short item count, but the actual
    // end-of-file condition won't be set or returned until the next operation.
    if (result != 0)
    {
        file->streamStatus = FileStreamStatus::STREAM_OK;
        return;
    }
    if (ferror(file->handle))
        file->streamStatus = FileStreamStatus::STREAM_ERROR;

    else if (feof(file->handle))
        file->streamStatus = FileStreamStatus::STREAM_EOF;

    clearerr(file->handle);
}

size_t FileSystem::read_file_chunk(File* file, void* chunk, size_t chunkSize)
{
    size_t bytesRead;
    __resync_file_stream(this, file, FileOp::READ);
    bytesRead = fread(chunk, 1, chunkSize, file->handle);
    __update_file_operation_status(file, bytesRead);
    return bytesRead;
}

size_t FileSystem::write_file_chunk(File* file, void* chunk, size_t chunkSize)
{
    size_t bytesWrote;
    __resync_file_stream(this, file, FileOp::WRITE);
    bytesWrote = fwrite(chunk, 1, chunkSize, file->handle);
    __update_file_operation_status(file, bytesWrote);
    return bytesWrote;
}

char* FileSystem::read_file_line(File* file, char* line, size_t maxLineSize)
{
    GP_ASSERT(file);
    GP_ASSERT(line);
    GP_ASSERT(maxLineSize);
    __resync_file_stream(this, file, FileOp::READ);
    char* stringRead = fgets(line, int(maxLineSize), file->handle);
    __update_file_operation_status(file, static_cast<size_t>(reinterpret_cast<uintptr_t>(stringRead)));
    // likely line termination will be located in the end of string
    if (stringRead)
    {
        size_t end = strlen(stringRead) - 1;
        if ('\n' == stringRead[end])
        {
            stringRead[end] = '\0';
            if (end > 0 && '\r' == stringRead[end - 1])
            {
                stringRead[end - 1] = '\0';
            }
        }
    }
    return stringRead;
}

bool FileSystem::write_file_line(File* file, const char* line)
{
    size_t lineLen = strlen(line);
    __resync_file_stream(this, file, FileOp::WRITE);
    size_t bytesWrote = fwrite(line, 1, lineLen, file->handle);
    __update_file_operation_status(file, bytesWrote);
    // failed the bulk of the write => fail and don't write the newline.
    if (bytesWrote != lineLen)
    {
        return false;
    }
    const int LF = '\n';
    int ret = fputc(LF, file->handle);
    if (ret != LF)
    {
        file->streamStatus = FileStreamStatus::STREAM_ERROR;
        clearerr(file->handle);
        return false;
    }
    return true;
}

void FileSystem::flush_file(File* file)
{
    GP_ASSERT(file);
    fflush(file->handle);
}

int64_t FileSystem::get_file_pos(File* file)
{
    GP_ASSERT(file);
#if GP_PLATFORM_WINDOWS
    // ftell() only returns a 32-bit value on Windows even in a 64-bit build since the 'long'
    // type remains 32-bit.  To get the file's actual size (if large), we need to call the
    // Windows specific 64-bit variant instead.
    return _ftelli64(file->handle);
#elif GP_PLATFORM_LINUX

    return static_cast<int64_t>(ftello(file->handle));
#else
    return static_cast<int64_t>(ftell(file->handle));
#endif
}

bool FileSystem::set_file_pos(File* file, int64_t offsetFromWhence, FileWhence whence)
{
    GP_ASSERT(file);
    int location;
    int result;
    switch (whence)
    {
    default:
    case FileWhence::BEGIN:
        location = SEEK_SET;
        if (offsetFromWhence < 0)
        {
            return false;
        }
        break;

    case FileWhence::CURRENT:
        location = SEEK_CUR;
        break;

    case FileWhence::END:
        location = SEEK_END;
        break;
    }
#if GP_PLATFORM_WINDOWS
    result = _fseeki64(file->handle, static_cast<long long>(offsetFromWhence), location);
#elif GP_PLATFORM_LINUX
    result = fseeko(file->handle, static_cast<off_t>(offsetFromWhence), location);
#else
    result = fseek(file->handle, offsetFromWhence, location);
#endif
    if (result != 0)
    {
        file->streamStatus = FileStreamStatus::STREAM_ERROR;
        return false;
    }
    clearerr(file->handle);
    file->streamStatus = FileStreamStatus::STREAM_OK;
    return true;
}

bool FileSystem::set_file_pos_begin(File* file)
{
    return set_file_pos(file, 0, FileWhence::BEGIN);
}

bool FileSystem::set_file_pos_end(File* file)
{
    return set_file_pos(file, 0, FileWhence::END);
}

bool FileSystem::truncate_file_at_current_pos(File* file)
{
    int result;
    int64_t pos = get_file_pos(file);
    if (pos == -1)
    {
        return false;
    }
#if GP_PLATFORM_WINDOWS
    result = _chsize_s(_fileno(file->handle), pos);
#else
    result = ftruncate(fileno(file->handle), pos);
#endif
    file->streamStatus = (result == 0 ? FileStreamStatus::STREAM_OK : FileStreamStatus::STREAM_ERROR);
    clearerr(file->handle);
    return (result == 0);
}

FileStreamStatus FileSystem::get_file_stream_status(File* file)
{
    return file->streamStatus;
}

void FileSystem::for_each_directory_item(const char* path, OnVisitDirectoryItemFn fn, void* userPtr, bool recursive)
{
    std::string pathAbs = __resolve_path(this, path, nullptr);
    std::string parent = pathAbs;
    WalkFlags flags = recursive ? WALK_FLAGS_RECURSIVE : 0;
#if GP_PLATFORM_WINDOWS
    std::wstring pathAbsW = Path::convert_utf8_to_windows_path(pathAbs);
    struct WindowsUserData
    {
        OnVisitDirectoryItemFn fn;
        void* userPtr;
    } windowsUserData{ fn, userPtr };

    auto windowsFn = [](const std::wstring& pathW, DirectoryInfo* info, void* userPtr) -> VisitAction {
        auto windowsUserData = static_cast<WindowsUserData*>(userPtr);
        auto path = Path::convert_windows_to_utf8_path(pathW);
        info->path = path.c_str();
        return windowsUserData->fn(info, windowsUserData->userPtr);
    };
    __walk_directory_windows(pathAbsW, pathAbsW, windowsFn, &windowsUserData, flags, nullptr, nullptr);
#elif GP_PLATFORM_LINUX
    __walk_directory_linux(pathAbs, pathAbs, fn, userPtr, flags, nullptr, nullptr);
#endif
}

//////////////////////////////////////////////////////////////////////////////
// platform utility impl.

#if GP_PLATFORM_WINDOWS
void __convert_To_lower(std::wstring& str)
{
    std::transform(str.begin(), str.end(), str.begin(), std::towlower);
}
std::string __winapi_errorcode_to_string(DWORD errorCode)
{
    if (errorCode == 0)
    {
        return std::string();
    }
    LPWSTR resultMessageBuffer = nullptr;
    const DWORD kFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                               FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_IGNORE_INSERTS;
    const DWORD dwFormatResultCode = ::FormatMessageW(kFormatFlags, nullptr, errorCode,
                                                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                                    reinterpret_cast<LPWSTR>(&resultMessageBuffer), 0, nullptr);
    if (dwFormatResultCode == 0)
    {
        const DWORD operationErrorCode = ::GetLastError();
        GP_LOG_ERROR("{} couldn't translate error code {" PRIu32 "}, `FormatMessage` error code is '{" PRIu32 "}'",
                       __func__, errorCode, operationErrorCode);
        return std::to_string(errorCode);
    }
    assert(resultMessageBuffer);
    const auto localMemDeleter = [](LPWSTR str)
    {
        if (str)
        {
            ::LocalFree(str);
        }
    };
    std::unique_ptr<WCHAR, decltype(localMemDeleter)> systemBuffKeeper(resultMessageBuffer, localMemDeleter);
    const std::string result = Unicode::convert_wide_to_utf8(resultMessageBuffer);
    return result;
}

std::wstring __get_windows_canonical_path(const std::wstring& pathW)
{
    wchar_t* canonical = nullptr;
    if (::PathAllocCanonicalize(pathW.c_str(), PATHCCH_ALLOW_LONG_PATHS, &canonical) == S_OK)
    {
        std::wstring result = canonical;
        LocalFree(canonical);
        return result;
    }
    GP_LOG_WARN("path '{}' could not be canonicalized!", Path::convert_windows_to_utf8_path(pathW).c_str());
    return pathW;
}

time_t __filetime_to_timet(FILETIME const& ft)
{
    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;
    // "11644473600" is the number of seconds between the Windows epoch of midnight January 1,
    // 1601 and the Unix epoch of midnight January 1, 1970 (including 89 leap days).  Note that
    // this does not include any leap seconds since they were not introduced into the UTC
    // calendar until January 1, 1972 (despite them being introduced at that time with 10 leap
    // seconds already retroactively applied).
    return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

VisitAction __walk_directory_windows(const std::wstring& pathAbsW, const std::wstring& parentW, OnVisitDirectoryItemFnWindows fn, void* userPtr,
                                    WalkFlags flags, std::list<std::wstring>* files, std::list<std::wstring>* directories)
{
    WIN32_FIND_DATA findData;
    std::wstring searchPathW = pathAbsW + L"\\*";
    HANDLE handle = ::FindFirstFileW(searchPathW.c_str(), &findData);
    BOOL success;
    VisitAction action = VisitAction::CONTINUE;

    // failed to start the directory enumeration -> nothing to do => fail.
    if (handle == INVALID_HANDLE_VALUE)
    {
        return action;
    }
    do
    {
        std::wstring fileNameW = findData.cFileName;
        std::wstring pathW = fileNameW;
        if (!parentW.empty())
        {
            pathW = parentW;
            pathW.append(L"\\");
            pathW.append(fileNameW);
        }
        pathW = Path::fix_windows_path_prefixes(pathW);

        bool const isDirectory = !!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
        bool const isSymlink = !!(findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT);
        if (isDirectory && ((flags & WALK_FLAGS_SYMLINKS_ARE_FILES) == 0 || !isSymlink))
        {
            if (fileNameW != L"." && fileNameW != L"..")
            {
                if (directories)
                {
                    directories->emplace_back(pathW);
                }
                action = VisitAction::CONTINUE;
                if (fn)
                {
                    DirectoryInfo info;
                    info.path = nullptr;
                    info.type = DirectoryItemType::DIRECTORY;
                    info.modTime = __filetime_to_timet(findData.ftLastWriteTime);
                    info.createTime = __filetime_to_timet(findData.ftCreationTime);
                    info.size = (findData.nFileSizeHigh * ((size_t)MAXDWORD + 1)) + findData.nFileSizeLow;
                    info.symlink = isSymlink;
                    if ((action = fn(pathW, &info, userPtr)) == VisitAction::STOP)
                    {
                        break;
                    }
                }
                if ((flags & WALK_FLAGS_RECURSIVE) != 0 && action == VisitAction::CONTINUE)
                {
                    std::wstring childPathAbsW = pathAbsW;
                    childPathAbsW.append(L"\\");
                    childPathAbsW.append(fileNameW);
                    childPathAbsW = Path::fix_windows_path_prefixes(childPathAbsW);
                    if ((action = __walk_directory_windows(childPathAbsW, pathW, fn, userPtr, flags, files, directories)) == VisitAction::STOP)
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            if (files)
            {
                files->emplace_back(pathW);
            }
            if (fn)
            {
                DirectoryInfo info;
                info.path = nullptr;
                info.type = DirectoryItemType::FILE;
                info.modTime = __filetime_to_timet(findData.ftLastWriteTime);
                info.createTime = __filetime_to_timet(findData.ftCreationTime);
                info.size = (findData.nFileSizeHigh * ((size_t)MAXDWORD + 1)) + findData.nFileSizeLow;
                info.symlink = isSymlink;
                if ((action = fn(pathW, &info, userPtr)) != VisitAction::CONTINUE)
                {
                    break;
                }
            }
        }
        success = ::FindNextFileW(handle, &findData);
        if (!success)
        {
            DWORD errorCode = GetLastError();
            if (errorCode != ERROR_NO_MORE_FILES)
            {
                GP_LOG_ERROR("FindNextFileW returned error code" PRIu32"{} inside '{}'", errorCode,
                               Path::convert_windows_to_utf8_path(pathAbsW).c_str());
            }
        }
    } while (success);

    // When the search handle is no longer needed, close it by using the FindClose function, not CloseHandle.
    if (!::FindClose(handle))
    {
        GP_LOG_ERROR("FindClose returned error code " PRIu32"{} inside '{}'", ::GetLastError(),
                       Path::convert_windows_to_utf8_path(pathAbsW).c_str());
    }
    return action;
}

#endif

#if GP_PLATFORM_LINUX
std::vector<std::string> __split_and_fix_linux_path(const std::string& path)
{
    std::vector<std::string> components;
    std::istringstream iss(path);
    std::string s;
    while (getline(iss, s, '/'))
    {
        // remove extra slashes in the middle or end of the path
        if (s.empty() && !components.empty())
        {
            continue;
        }
        // skip current directory
        if (s == ".")
        {
            continue;
        }
        // go to the parent directory
        if (s == "..")
        {
            // only if there is one
            if (components.empty())
            {
                components.push_back(s);
            }
            else
            {
                components.pop_back();
            }
        }
        else
        {
            components.push_back(s);
        }
    }
    return components;
}

WalkAction __walk_directory_linux(const std::string& pathAbs,
                                     const std::string& parent,
                                     OnDirectoryItemFn fn,
                                     void* userPtr,
                                     WalkFlags flags,
                                     std::list<std::string>* files = nullptr,
                                     std::list<std::string>* directories = nullptr)
{
    struct dirent* entry;
    DirectoryInfo info;
    DIR* dir;
    dir = opendir(pathAbs.c_str());
    if (dir == nullptr)
    {
        switch (errno)
        {
        case ENOENT:
            GP_LOG_INFO("Directory '{}' does not exist", pathAbs.c_str());
            break;
        case EACCES:
            GP_LOG_INFO("Insufficient permissions for directory '{}'", pathAbs.c_str());
            break;
        default:
            GP_LOG_ERROR("Failed to opendir() on '{}'. errno = {}", pathAbs.c_str(), errno);
            break;
        }
        return WalkAction::CONTINUE;
    }
    WalkAction action = WalkAction::CONTINUE;
    while ((entry = readdir(dir)) != nullptr)
    {
        std::string fileName = entry->d_name;
        std::string path = fileName;
        if (!parent.empty())
        {
            path = parent;
            path.append("/");
            path.append(fileName);
        }
        bool const isSymlink = (entry->d_type == DT_LNK);
        if (!(flags & WALK_FLAGS_SYMLINKS_ARE_FILES) && isSymlink)
        {
            struct stat s;
            int r = stat(path.c_str(), &s);
            if (r != 0)
            {
                switch (errno)
                {
                case ENOENT:
                    GP_LOG_WARN("Broken symlink '{}'", path.c_str());
                    break;
                case EACCES:
                    GP_LOG_INFO("Insufficient permissions for symlink '{}'", path.c_str());
                    break;
                default:
                    GP_LOG_ERROR("Failed stat() on '{}'. errno = {}", path.c_str(), errno);
                    break;
                }
            }
            else if (S_ISDIR(s.st_mode))
            {
                entry->d_type = DT_DIR;
            }
        }
        if (entry->d_type == DT_DIR)
        {
            if (fileName != "." && fileName != "..")
            {
                if (directories != nullptr)
                {
                    directories->emplace_back(path);
                }
                action = WalkAction::CONTINIUE;
                if (fn != nullptr)
                {
                    // retrieve the directory's info.
                    if (!get_file_info(&info, path))
                    {
                        continue;
                    }
                    info.symlink = isSymlink;

                    // perform the callback for the directory.
                    if ((action = fn(&info, userData)) == WalkAction::STOP)
                    {
                        break;
                    }
                }
                if ((flags & WALK_FLAGS_RECURSIVE) != 0 && action == WalkAction::CONTINUE)
                {
                    std::string childPathAbs = pathAbs;
                    childPathAbs.append("/");
                    childPathAbs.append(fileName);
                    if ((action = __walk_directory_linux(childPathAbs, path, fn, userData, flags, files, directories)) == WalkAction::STOP)
                    {
                        break;
                    }
                }
            }
        }
        else if (entry->d_type == DT_REG || entry->d_type == DT_CHR || entry->d_type == DT_LNK)
        {
            if (files != nullptr)
            {
                files->emplace_back(path);
            }
            if (fn != nullptr)
            {
                // retrieve the file's info.
                if (!get_file_info(&info, path))
                    continue;

                info.symlink = isSymlink;

                // perform the callback for the file.
                if ((action = fn(&info, userData)) != WalkAction::CONTINUE)
                {
                    break;
                }
            }
        }
        else
        {
            GP_LOG_WARN("Unknown type for the object '{}' d_type = {:08x}", path.c_str(), entry->d_type);
        }
    }
    if (closedir(dir) != 0)
    {
        GP_LOG_ERROR("Failed to close the directory '{}'. errno = {}", pathAbs.c_str(), errno);
    }
    return action;
}
#endif

std::string __resolve_path(FileSystem* fileSystem, const char* relativeOrAbsolutePath, const char* base)
{
    std::string path = std::string(relativeOrAbsolutePath);
    if (base != nullptr && base[0] == 0)
    {
        return path;
    }
    if (path.empty())
    {
        if (base != nullptr)
        {
            return std::string(base);
        }
        return fileSystem->get_current_directory_path();
    }
#if GP_PLATFORM_WINDOWS
    if (Path(path.c_str()).is_relative())
    {
        if (base != nullptr)
        {
            return std::string(base) + "/" + path;
        }
        std::string cwd = fileSystem->get_current_directory_path();
        return cwd + "/" + path;
    }
#else
    if (base != nullptr && Path(path.c_str()).is_relative())
    {
        return std::string(base) + "/" + path;
    }
#endif
    return path;
}

void __remove_duplicated_slashes(std::string& path)
{
    path.erase(std::unique(path.begin(), path.end(), [](char a, char b) { return a == '/' && b == '/'; }), path.end());
}

uint32_t FileSystem::subscribe_to_change_events(const char* path, OnChangeEventFn fn, void* userPtr)
{
    // todo
    return 0;
}

   
void FileSystem::unsubscribe_to_change_events(uint32_t id)
{
    // todo
}
}
