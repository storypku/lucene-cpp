#ifndef FS_DIRECTORY_H
#define FS_DIRECTORY_H

#include "directory.h"

namespace Lucene {


/// Base class for Directory implementations that store index files in the
/// file system.  There are currently 2 core subclasses:
///
/// SimpleFSDirectory is a straightforward implementation using std::ofstream
/// and std::ifstream.
///
/// MMapDirectory uses memory-mapped IO when reading. This is a good choice if
/// you have plenty of virtual
/// memory relative to your index size, eg if you are running on a 64 bit op-
/// erating system, oryour index sizes are small enough to fit into the virtual
/// memory space.
///
/// The locking implementation is by default NativeFSLockFactory, but can be
/// changed by passing in a custom LockFactory instance.
class FSDirectory : public Directory {
protected:
    /// Create a new FSDirectory for the named location (ctor for subclasses).
    /// @param path the path of the directory.
    /// @param lockFactory the lock factory to use, or null for the default
    /// (NativeFSLockFactory)
    FSDirectory(const String& path, const LockFactoryPtr& lockFactory);

public:
    virtual ~FSDirectory();

    LUCENE_CLASS(FSDirectory);

public:
    /// Default read chunk size. This is a conditional default based on
    /// operating system.
    static const int32_t DEFAULT_READ_CHUNK_SIZE;

protected:
    bool m_checked;
    /// The underlying filesystem directory.
    String m_directory;
    int32_t m_chunkSize;

public:
    /// Create an FSDirectory instance.
    static FSDirectoryPtr open(const String& path);

    /// Just like #open(File), but allows you to also specify a custom
    /// LockFactory.
    static FSDirectoryPtr open(const String& path, const LockFactoryPtr& lockFactory);

    /// Lists all files (not subdirectories) in the directory.
    /// @throws NoSuchDirectoryException if the directory does not exist, or
    /// does exist but is not a directory.
    static HashSet<String> list_all(const String& dir);

    static uint64_t file_modified(const String& directory, const String& name);

    /// Create file system directory.
    void create_dir();

    /// Return file system directory.
    String get_file();

    /// Sets the maximum number of bytes read at once from the underlying file
    /// during IndexInput#readBytes. The default value is #DEFAULT_READ_CHUNK_SIZE.
    /// Changes to this value will not impact any already-opened IndexInputs.
    /// You should call this before attempting to open an index on the directory.
    /// This value should be as large as possible to reduce any possible per-
    /// formance impact.
    void set_read_chunk_size(int32_t chunkSize);

    /// The maximum number of bytes to read at once from the underlying file
    /// during IndexInput#readBytes.
    int32_t get_read_chunk_size();

    /// Lists all files (not subdirectories) in the directory.
    virtual HashSet<String> list_all();

    /// Returns true if a file with the given name exists.
    virtual bool file_exists(const String& name);

    /// Returns the time the named file was last modified.
    virtual uint64_t file_modified(const String& name);

    /// Set the modified time of an existing file to now.
    virtual void touch_file(const String& name);

    /// Removes an existing file in the directory.
    virtual void delete_file(const String& name);

    /// Returns the length in bytes of a file in the directory.
    virtual int64_t file_length(const String& name);

    /// Ensure that any writes to this file are moved to stable storage.  Lucene uses
    /// this to properly commit changes to the index, to prevent a machine/OS crash
    /// from corrupting the index.
    virtual void sync(const String& name);

    virtual IndexInputPtr open_input(const String& name);
    virtual IndexInputPtr open_input(const String& name, int32_t bufferSize);

    /// Return a string identifier that uniquely differentiates this Directory
    /// instance from other Directory instances.
    virtual String get_lock_id();

    /// Closes the store.
    virtual void close();

    /// For debug output.
    virtual String to_string();

protected:
    /// Initializes the directory to create a new file with the given name.
    /// This method should be used in #createOutput.
    void init_output(const String& name);
};

} // namespace Lucene

#endif // FS_DIRECTORY_H
