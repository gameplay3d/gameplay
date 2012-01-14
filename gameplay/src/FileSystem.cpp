#include "Base.h"
#include "FileSystem.h"

#ifdef WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
#else
    #include <dirent.h>
#endif

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

bool FileSystem::listFiles(const char* dirPath, std::vector<std::string>& files)
{
    // TODO make this method work with absolute and relative paths.
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
    } while(FindNextFile(hFind, &FindFileData) != 0);

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

FILE* FileSystem::openFile(const char* path, const char* mode)
{
    std::string fullPath(__resourcePath);
    fullPath += path;

    FILE* fp = fopen(fullPath.c_str(), mode);

// Win32 doesnt support a asset or bundle definitions.
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
