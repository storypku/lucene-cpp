#ifndef MMAP_DIRECTORY_H
#define MMAP_DIRECTORY_H

#include <boost/iostreams/device/mapped_file.hpp>
#include "index_input.h"
#include "fs_directory.h"

namespace Lucene {

/// File-based Directory implementation that uses mmap for reading, and
/// SimpleFSIndexOutput for writing.
///
/// NOTE: memory mapping uses up a portion of the virtual memory address space
/// in your process equal to the size of the file being mapped.  Before using
/// this class, be sure your have plenty of virtual address space.
///
/// 不懂...555
/// NOTE: Accessing this class either directly or indirectly from a thread
/// while it's interrupted can close the underlying channel immediately if at
/// the same time the thread is blocked on IO.  The channel will remain closed
/// and subsequent access to MMapDirectory will throw an exception.
///
class MMapDirectory : public FSDirectory {
public:
    /// Create a new MMapDirectory for the named location.
    /// @param path the path of the directory.
    /// @param lockFactory the lock factory to use, or null for the default (NativeFSLockFactory)
    MMapDirectory(const String& path, const LockFactoryPtr& lockFactory = LockFactoryPtr());
    virtual ~MMapDirectory();

    LUCENE_CLASS(MMapDirectory);

public:
    using FSDirectory::open_input;

    /// Creates an IndexInput for the file with the given name.
    virtual IndexInputPtr open_input(const String& name, int32_t bufferSize);

    /// Creates an IndexOutput for the file with the given name.
    virtual IndexOutputPtr create_output(const String& name);
};

class MMapIndexInput : public IndexInput {
public:
    MMapIndexInput(const String& path = EmptyString);
    virtual ~MMapIndexInput();

    LUCENE_CLASS(MMapIndexInput);

protected:
    int32_t m_length;
    bool m_isClone;
    boost::iostreams::mapped_file_source m_file;
    int32_t m_bufferPosition;

public:
    virtual uint8_t read_byte();

    virtual void read_bytes(uint8_t* b, int32_t offset, int32_t length);

    virtual int64_t get_file_pointer();

    virtual void seek(int64_t pos);

    virtual int64_t length();

    virtual void close();

    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());
};

} // namespace Lucene
#endif // MMAP_DIRECTORY_H
