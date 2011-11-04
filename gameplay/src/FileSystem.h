/*
 * FileSystem.h
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

namespace gameplay
{

/**
 * Defines a set of functions for interacting with the device filesystem.
 */
class FileSystem
{
public:

    /**
     * Destructor.
     */
    ~FileSystem();

    /**
     * Sets the path to the root of the resources folder for the game.
     *
     * Once set, all resource/file loading will load from the given path.
     * The default resource path is "./".
     * 
     * @param path The path to the root of the resources folder.
     */
    static void setResourcePath(const char* path);

    /**
     * Returns the currently set resource path.
     * 
     * @return The currently set resource path.
     */
    static const char* getResourcePath();

    /**
     * Opens the specified file.
     *
     * The file at the specified location is opened, relative to the currently set
     * resource path.
     *
     * @param path The path to the file to be opened, relative to the currently set resource path.
     * @param mode The mode used to open the file, passed directly to fopen.
     * 
     * @see setResourcePath(const char*)
     */
    static FILE* openFile(const char* path, const char* mode);

    /**
     * Reads the entire contents of the specified file and returns its contents.
     *
     * The returned character array is allocated with new[] and must therefore
     * deleted by the caller using delete[].
     *
     * @param filePath The path to the file to be read.
     * @param fileSize The size of the file in bytes (optional).
     * 
     * @return A newly allocated (NULL-terminated) character array containing the
     *      contents of the file, or NULL if the file could not be read.
     */
    static char* readAll(const char* filePath, int* fileSize = NULL);

private:

    /**
     * Constructor.
     */
    FileSystem();
};

}

#endif
