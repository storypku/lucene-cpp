#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "lucene_object.h"

namespace Lucene {
namespace FileUtils {

/// Return true if given file or directory exists.
bool file_exists(const String& path);

/// Return file last modified date and time.
uint64_t file_modified(const String& path);

/// Set file last modified date and time to now.
bool touch_file(const String& path);

/// Return file length in bytes.
int64_t file_length(const String& path);

/// Set new file length, truncating or expanding as required.
bool set_file_length(const String& path, int64_t length);

/// Delete file from file system.
bool remove_file(const String& path);

/// Copy a file to/from file system.
bool copy_file(const String& source, const String& dest);

/// Create new directory under given location.
bool create_directory(const String& path);

/// Delete directory from file system.
bool remove_directory(const String& path);

/// Return true if given path points to a directory.
bool is_directory(const String& path);

/// Return list of files (and/or directories) under given directory.
/// @param path path to list directory.
/// @param filesOnly if true the exclude sub-directories.
/// @param dirList list of files to return.
bool list_directory(const String& path, bool filesOnly, HashSet<String> dirList);

/// Copy a directory to/from file system.
bool copy_directory(const String& source, const String& dest);

/// Return complete path after joining given directory and file name.
String join_path(const String& path, const String& file);

/// Extract parent path from given path.
String extract_path(const String& path);

/// Extract file name from given path.
String extract_file(const String& path);

} // namespace FileUtils
} // namespace Lucene

#endif // FILE_UTILS_H
