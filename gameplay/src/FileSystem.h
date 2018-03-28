#pragma once

#include "Stream.h"
#include <string>

namespace gameplay
{

/**
 * Defines the device file system api.
 */
class FileSystem
{
public:

    enum class AccessFlags : uint32_t
    {
        eRead = 1,
        eWrite = 2
    };


    /**
     * Dialog mode flags when displaying dialogs.
     */
    enum class DialogMode
    {
        eOpen,
        eSave
    };

    /**
     * Destructor.
     */
    ~FileSystem();

    /**
     * Sets the home path for the game.
     *
     * Once set, all file loading will load from the given home path.
     *
     * The default path is "./".
     * 
     * @param path The path to the game home folder.
     */
    static void setHomePath(const std::string& path);

    /**
     * Gets the currently set asset path.
     * 
     * @return The currently set asset path.
     */
    static std::string getHomePath();

    /**
     * Gets the absolute directory for a relative path or "" if not resolved.
     *
     * @param relativePath The relative path to be resolved.
     * @return The absolute directory for a relative path or "" if not resolved.
     */
    static std::string getAbsolutePath(const std::string& relativePath);

    /**
     * Resolves a filesystem path.
     *
     * If the specified path is a file system alias, the alias will be
     * resolved and the physical file will be returned.
     *
     * @param path The path to resolve.
     * @return The resolved file path.
     */
    static std::string resolvePath(const std::string& path);

    /**
     * Lists the files in the specified directory and adds the files to the vector. Excludes directories.
     * 
     * @param dirPath Directory path relative to the path set in <code>setHomePath(const char*)</code>.
     * @param files The vector to append the files to.
     * @return True if successful, false if error.
     */
    static bool listFiles(const std::string& dirPath, std::vector<std::string>& files);

    /**
     * Checks if the file at the given path exists.
     * 
     * @param dropFile The path to the file.
     * @return <code>true</code> if the file exists; <code>false</code> otherwise.
     */
    static bool fileExists(const std::string& dropFile);

    /**
     * Opens a byte stream for the given asset path.
     *
     * If <code>path</code> is a file path, the file at the specified location is opened relative to the currently set
     * asset path.
     *
     * @param path The path to the asset to be opened, relative to the currently set asset path.
     * @param accessFlags The access flags used to open the file.
     * @return A stream that can be used to read or write to the file depending on the mode, 
     *         nullptr if there was an error. (Request mode not supported).
     */
    static Stream* open(const std::string& path, AccessFlags accessFlags = AccessFlags::eRead);

    /**
     * Reads the entire contents of the specified file and returns its contents.
     *
     * The returned character array is allocated with new[] and must therefore
     * deleted by the caller using delete[].
     *
     * @param dropFile The path to the file to be read.
     * @return A string containing the contents of the file or empty string if
     */
    static std::string readAll(const std::string& dropFile);

    /**
     * Determines if the file path is an absolute path for the current platform.
     * 
     * @param dropFile The file path to test.
     * @return true if the path is an absolute path or false otherwise.
     */
    static bool isAbsolutePath(const std::string& dropFile);

    /**
     * Gets the directory name up to and including the trailing '/'.
     * 
     * This is a lexical method so it does not verify that the directory exists.
     * Back slashes will be converted to forward slashes.
     * 
     * - "res/image.png" will return "res/"
     * - "image.png" will return ""
     * - "c:/foo/bar/image.png" will output "c:/foo/bar/"
     * 
     * @param path The file path. May be relative or absolute, forward or back slashes. May be nullptr.
     * @return The directory name with the trailing '/' or emptry string if invalid string or directory.
     */
    static std::string getDirectoryPath(const std::string& path);

    /**
     * Gets the extension of the given file path.
     *
     * The extension returned includes all character after and including the last '.'
     * in the file path. The extension is returned as all uppercase.
     *
     * If the path does not contain an extension, an empty string is returned.
     * 
     * @param path File path.
     * @return The file extension, all uppercase, including the '.'.
     */
    static std::string getExtension(const std::string& path);

private:

    FileSystem();
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::FileSystem::AccessFlags);
