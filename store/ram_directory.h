#ifndef RAM_DIRECTORY_H
#define RAM_DIRECTORY_H

#include "directory.h"

namespace Lucene {

/// A memory-resident Directory implementation. Locking implementation is by
/// default the SingleInstanceLockFactory but can be changed with
/// #set_lock_factory.
/// Lock acquisition sequence: RAMDirectory, then RAMFile
class RAMDirectory : public Directory {
public:
    /// Constructs an empty Directory.
    RAMDirectory();

    /// Creates a new RAMDirectory instance from a different Directory implementation.
    /// This can be used to load a disk-based index into memory.
    /// This should be used only with indices that can fit into memory.
    /// Note that the resulting RAMDirectory instance is fully independent from the
    /// original Directory (it is a complete copy).  Any subsequent changes to the
    /// original Directory will not be visible in the RAMDirectory instance.
    /// @param dir a Directory value
    RAMDirectory(const DirectoryPtr& dir);

    RAMDirectory(const DirectoryPtr& dir, bool closeDir);

    virtual ~RAMDirectory();

    LUCENE_CLASS(RAMDirectory);

protected:
    int64_t m_sizeInBytes;
    MapStringRAMFile m_fileMap;
protected:
    DirectoryWeakPtr m_dirSource_;
    bool m_copyDirectory;
    bool m_closeDir;
public:
    virtual void initialize();

    virtual HashSet<String> list_all();
    virtual bool file_exists(const String& name);
    virtual uint64_t file_modified(const String& name);
    virtual void touch_file(const String& name);
    virtual int64_t file_length(const String& name);
    int64_t size_in_bytes();
    virtual void delete_file(const String& name);

    virtual IndexOutputPtr create_output(const String& name);
    virtual IndexInputPtr open_input(const String& name);

    virtual void close();

public:
    friend class RAMFile;
};

} // namespace Lucene

#endif // RAM_DIRECTORY_H
