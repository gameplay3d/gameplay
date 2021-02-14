#pragma once

#include "App.h"
#include <string>


namespace gameplay
{
struct File;

enum class FileMode
{
    READ,
    WRITE,
    READ_WRITE,
    APPEND
};

enum class FileWhence
{
    BEGIN,
    CURRENT,
    END
};

enum class FileStreamStatus
{
    STREAM_OK,
    STREAM_EOF,
    STREAM_ERROR
};

enum class DirectoryItemType
{
    FILE,
    DIRECTORY,
};

struct FileInfo
{
    DirectoryItemType type;
    time_t createTime;
    time_t modTime;
    size_t size;
    bool symlink;
};

struct DirectoryInfo : public FileInfo
{
    const char* path;
};

enum class VisitAction : signed char
{
    STOP = -1,
    SKIP,
    CONTINUE
};


/**
 * Defines the file system.
 *
 * Any operation that takes a path can be specified in absolute
 * or relative paths are resolved from the current working directory.
 */
class GP_API FileSystem
{
    friend class App;
    friend class Config;
public:

    /**
     * Changes events for files and directories on the file system. 
     */
    enum class ChangeEvent
    {
        CREATED,
        MODIFIED,
        DELETED,
        RENAMED
    };

    /**
     * Constructor.
     *
     * @see App::get_file_system() instead.
     */
    FileSystem();

    /**
     * Destructor.
     */
    ~FileSystem();

    /**
     * Gets the application executable file path.
     *
     * @return The application executable file path.
     */
    const char* get_app_executable_path() const;

    /**
     * Gets the application directory path.
     *
     * @return The application directory path.
     */
    const char* get_app_directory_path() const;

    /**
     * Sets the current directory path for the filesystem.
     *
     * @param path The current directory path for the filesystem.
     */
    bool set_current_directory_path(const char* path);

    /**
     * Gets the current directory path for the filesystem.
     *
     * @return The current directory path for the filesystem.
     */
    const char* get_current_directory_path();

    /**
     * Checks if a file or directory path exists on the file system.
     *
     * @param path The path to check.
     * @return true if the path exists, false if not.
     */
    bool exists(const char* path);

    /**
     * Checks is a path is a directory.
     *
     * @param path The path to check.
     * @return true if the path is a directory, false if not.
     */
    bool is_directory(const char* path);

     /**
     * Checks is a file is a writable.
     *
     * @param path The path to check.
     * @return true if the file is writable, false if not.
     */
    bool is_writable(const char* path);

    /**
     * Gets the time the file or directory was created.
     *
     * @param path The path to use.
     * @return The time the file or directory was created.
     */
    time_t get_create_time(const char* path);

    /**
     * Gets the last time a file or directory was modified.
     *
     * @param path The path to use.
     * @return The last time a file or directory was modified.
     */
    time_t get_mod_time(const char* path);

    /**
     * Gets an operating system specific canonical path relative to the base.
     * 
     * @param path The absolute or relative path. The path must exist.
     * @param base The base path to resolve relative path against. Can be nullptr.
     * @return The operating system specific canonical path relative to the base.
     */
    std::string get_canonical_path(const char* path, const char* base = nullptr);

    /**
     * Makes a temporary directory on the file system.
     *
     * The directory is created under the system temporary directory area and will have a randomized name.
     *
     * @return The full path to the created directory or if fails and empty string and also logs an error message.
     */
    std::string make_temp_directory();

    /**
     * Makes a directory on the file system.
     *
     * @param path The path to the directory to create. 
     * @param createMissingDirectories if true it will create any missing directories to make the 
     * 
     * @return true if the creation was successful, fails if fails.
     */
    bool make_directory(const char* path, bool createMissingDirectories = true);

    /**
     * Removes (deletes) a directory from the file system.
     * 
     * This will never follow symbolic links. The symbolic link will be removed, but its target will not.
     * To remove the currect directory, you must first change to another directory and then remove it.
     * 
     * @param path The path to the directory to remove. 
     * @return true if the removal was successful, false is not successful.
     */
    bool remove_directory(const char* path);

    /**
     * Removes (deletes) a file from the file system.
     *
     * This can fail by either having the file still open by either the calling process or another process,
     * or by not having sufficient permission to delete the file.
     * 
     * @param path The path of the file to be removed. 
     * @returns true if the file was removed from the file system, false if not successful.
     */
    bool remove_file(const char* path);

    /**
     * Moves (renames) a file or directory on the file system.
     * 
     * @param src The source path to a file or directory to rename. 
     * @param dst The destination path.
     * @return true of the move was successful, false is not successful.
     */
    bool move(const char* src, const char* dst);

    /**
     * Copy a file on the file system to a new destination.
     * 
     * @param src The source path to a file to copy. 
     * @param dst The destination filename and path.
     * @return true of the copy was successful, false is not successful.
     */
    bool copy(const char* src, const char* dst);

    /**
     * Gets file information on a specified file on the file system.
     *
     * @param path The path to the file.
     * @param info The info populated about the file.
     * @return true if information was gathered. false if an error occurs.
     */
    bool get_file_info(const char* path, FileInfo* info);

    /**
     * Opens a file on the file system for binary mode with the specifed file mode access.
     *
     * The file opened must closed with close_file() when it is no longer needed.
     * 
     * Any files opened with FileMode::WRITE, FileMode::READ_WRITE or FileMode::APPEND
     * that do not exist will be create if the file does not already exist.
     *
     * Files opened with FileMode::READ, FileMode::WRITE or FileMode::READ_WRITE will be initially positioned
     * to the beginning of the file. Files opened with FileMode::APPEND will be initiallly positioned to the
     * end of the file.
     * 
     * @param path The path to the file to open.
     * @param The file mode for which to access the file.
     * @param The handle for the file opened if successful or nullptr if not successful.
     */
    File* open_file(const char* path, FileMode mode);

    /**
     * Closes a file on the file system.
     *
     * @param file The handle to the file to be closed.
     */
    void close_file(File* file);

    /**
     * Gets the total size of the file in bytes.
     *
     * @param file The handle to the file.
     * @param The total size of the file in bytes.
     */
    size_t get_file_size(File* file);

    /**
     * Gets the time the file was created.
     *
     * @param file The handle to the file.
     * @return The time this file was created.
     */
    time_t get_file_create_time(File* file);

    /**
     * Gets the last time the file was modified.
     *
     * @param file The handle to the file.
     * @return The time this file was created.
     */
    time_t get_file_mod_time(File* file);

    /**
     * Reads a chunk of binary data from a file.
     *
     * @param file The handle to the file to read from in binary mode.
     * @param chunk Memory to read the binary data to, at least chunkSize bytes large.
     * @param chunkSize Number of bytes to read from file into 'chunk' memory area.
     * @return The number of bytes read. This can be less than requested 'chunkSize' when reading the last bytes of
     *  data. This will return 0 when all data has been read from the file.
     */
    size_t read_file_chunk(File* file, void* chunk, size_t chunkSize);

    /**
     * Writes a chunk of binary data to a file.
     *
     * @param file An handle to the file to write to in binary mode.
     * @param chunk The memory buffer to write to the file.
     * @param chunkSize Number of bytes from 'chunk' to write to the file.
     * @returns The number of bytes successfully written to the file. This can be less than the
     *  requested chunkSize if an error occurs such as disk full.
     * @returns 0 bytes if no data could be written to the file.
     */
    size_t write_file_chunk(File* file, void* chunk, size_t chunkSize);

    /**
     * Reads a line of character data from a text file.
     *
     * A read line will not include any line termination characters.
     * 
     * @param file The handle of the file to read.
     * @param line The string that will receive the read line. 
     * @param maxLineSize The maximum number of characters that can be read into 'line', including null termination.
     *  If the buffer is exhausted before end-of-line is reached the buffer will be null terminated and thus still a
     *  proper string but won't necessarily contain the full line from the file.
     * @return The read string, 'line', on each successful read but nullptr when end of file is reached.
     */
    char* read_file_line(File* file, char* line, size_t maxLineSize);

    /**
     * Writes a line of characters to a text file.
     *
     * A newline will always be appended to the string in the file if it is successfully written.
     *
     * @param file The handle of the file to read.
     * @param line The null-terminated string to write. 
     * @returns true if the full line is successfully written to the file, false if the full string could not be written to the file.
     */
    bool write_file_line(File* file, const char* line);

    /**
     * Flushes any unwritten data to the file.
     *
     * When a file is closed, either by calling close_file or during program termination,
     * all the associated buffers are automatically flushed.
     *
     * @param  The handle of the file opened for writing or appending.
     */
    void flush_file(File* file);

    /**
     * Gets the current file cursor position for an open file.
     *
     * The file may have been opened for read, write or append. Files that were opened for
     * append will always write at the end of the file regardless of the current file position.
     * The file current position is typically unused or undefined in the append case.
     *
     * This retrieves the current location of the file in a file that has been
     * opened for read, write, or append. The offset is always returned in bytes.
     * The current file position may be beyond the end of the file if the file pointer was
     * recently placed beyond the end of the file.  However, this does not actually reflect the size of the file
     * until at least one byte is written into it at the new position beyond the file's end.
     *
     * @param file The handle of the file to retrieve the current position for.
     * @return The current position in the file in bytes relative to the beginning or -1
     *  if the position could not be retrieved.
     */
    int64_t get_file_pos(File* file);

    /**
     * Sets the new file cursor position for an open file.
     *
     * The file may have been opened for read, write or append. Files that were opened for
     * append will always write at the end of the file regardless of the current file position.
     * The file current position is typically unused or undefined in the append case.
     *
     * This attempts to reposition the file cursor in an open file. The new absolute position may
     * not be negative once combined with whence. If the new absolute position is beyond the current
     * end of the file, the file will not be extended until at least one byte is written into the
     * file at that new position or the file is truncated at the current position with truncate_file_at_current_pos().
     * When it is written to or truncated with a larger size than previous, the new space  will be filled with zeros.
     * However, that if the file pointer is set beyond the end of the file, the get_file_pos() call will return that
     * same position even though it is larger than the file currently is. 
     * 
     * @param The handle of the file to set the current position for.
     * @param offsetFromWhence The new position for the file cursor relative to the location specified in whence.
     *  This value may be negative only if whence is not FileWhence::BEGIN. This may specify an index beyond the
     *  current end of the file when combined with whence.
     * @return true if the file position was successfully set,
     *  false if the file position could not be set or was invalid.
     */
    bool set_file_pos(File* file, int64_t offsetFromWhence, FileWhence whence);

    /**
     * Sets the file position to the beginning of the open file.
     *
     * Equivalent to: set_file_pos(file, 0, FileWhence::BEGIN);
     *
     * @param file The handle of the file.
     * @return true if the file position was successfully set,
     *  false if the file position could not be set or was invalid.
     */
    bool set_file_pos_begin(File* file);

    /**
     * Sets the file position to the end of the open file.
     *
     * Equivalent to: set_file_pos(file, 0, FileWhence::END);
     *
     * @param file The handle of the file.
     * @return true if the file position was successfully set,
     *  false if the file position could not be set or was invalid.
     */
    bool set_file_pos_end(File* file);

    /**
     * Truncates a file at the current file position.
     *
     * The file must have been opened for write or append.
     *
     * This truncates a file at the current file pointer position.  This can be used
     * to extend a file without needing to write anything to it by opening the file,
     * setting the file position to the desired size with set_file_pos(), then calling
     * this function to set the new end of the file.  The new area of the file will be
     * filled with zeros if it was extended. If the file is being shortened, all data
     * in the file beyond the current file pointer will be removed.
     * 
     * @param file The handle to the file to be truncated.
     * @return true if the file was successfully truncated,
     *  false if the file could not be truncated for any reason.
     */
    bool truncate_file_at_current_pos(File* file);

    /**
     * Gets the current file status of a stream.
     * 
     * FileStreamStatus::STREAM_OK Is the file stream is still in a valid state and more read or
     *  write operation may potentially succeed.
     * FileStatus::STREAM_ERROR Is if the file stream has encountered an error of any kind.
     *  This may include a partial write due to a full disk or a disk quota being reached.
     * FileStreamStatus::STREAM_OEF Is if a file stream opened for read has already read the last
     *  bytes in the file.  A future call to read_file_*() will simply return 0 or nullptr
     * from the same file position.
     *
     * This retrieves the current status of a file stream object. The status allows
     * the caller to differentiate an error from an end-of-file condition for the
     * last file operation.  The error condition on the file will be reset after
     * each operation after being stored for later retrieval. The file stream status
     * value will remain valid until the next operation is performed on the file.
     *
     * As with all other file operations, retrieving this status is not thread safe
     * and could change if another thread performs an unprotected operation on the
     * same stream. It is the caller's responsibility to ensure operations on the
     * file stream are appropriately protected.
     *
     * The file status will not be modified by calls to get_file_size(), get_file_mod_time(),
     * flush_file(), or get_file_pos().
     *
     * @param file The handle of an open file to check the status of.
     */
    FileStreamStatus get_file_stream_status(File* file);

    /**
     * Callback function that is called for each file or directory visited when calling
     * the function for_each_directory_item(..).
     *
     * @param info The directory information for the item visited.
     * @param The user pointer specified by the call to for_each_directory_item()
     * @return The action to perform after the visited item is completed.
     */
    typedef VisitAction (*OnVisitDirectoryItemFn)(const DirectoryInfo* info, void* userPtr);

    /**
     * Iterate through each item in the directory from the specified directory path.
     *
     * @param path The path to the directory
     * @param fn The visitor function to be called for each directory item encountered.
     * @param userPtr The user pointer data passed to the callback function for each item.
     * @param recursive true to recursively traverse through each sub-directory too,
     * false for only the specified path and not sub-directories.
     */
    void for_each_directory_item(const char* path, OnVisitDirectoryItemFn fn, void* userPtr, bool recursive = false);

    /**
     * Callback function to use when listening to changes on file system.
     *
     * @param path The path for file system change.
     * @param evt The change event that occurred.
     * @param userPtr The user pointer associated with the subscription to the change event.
     * @param newPath The path for the new name of the file. Used only for RENAMED event, otherwise it's nullptr
     */
    typedef void (*OnChangeEventFn)(const char* path, ChangeEvent evt, void* userPtr, const char* newPath);

    /**
     * Subscribes to listen on change events on a path.
     *
     * @param path The path on which to subscribe to change events.
     * @param onChangeEvent The callback function to be called when the events are fired.
     * @param userData The user pointer passed to the callback function for each item.
     * @return The subscription id if the path was successfully subscribed to or nullptr otherwise.
     */
    uint32_t subscribe_to_change_events(const char* path, OnChangeEventFn fn, void* userPtr);

    /**
     * Unsubscribes from listening to change events based on subscription path.
     * 
     * @param id The subscription id to unsubscribe from.
     */
    void unsubscribe_to_change_events(uint32_t id);

private:
    void set_app_executable_path(const char* path);
    struct Impl;
    std::unique_ptr<Impl> _impl;
};
}
