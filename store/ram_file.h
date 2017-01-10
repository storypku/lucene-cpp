#ifndef RAM_FILE_H
#define RAM_FILE_H

#include "lucene_object.h"

namespace Lucene {

/// File used as buffer in RAMDirectory
class RAMFile : public LuceneObject {
public:
    RAMFile(); // File used as buffer, in no RAMDirectory.
    RAMFile(const RAMDirectoryPtr& directory);
    virtual ~RAMFile();

    LUCENE_CLASS(RAMFile);

private:
    int64_t m_length;
    RAMDirectoryWeakPtr m_directory_;

protected:
    Collection<ByteArray> m_buffers;
    int64_t m_sizeInBytes;

    /// This is publicly modifiable vir Directory.touchFile(), so direct access
    /// not supported.
    int64_t m_lastModified;

public:
    int64_t get_length();
    void set_length(int64_t length);

    /// For non-stream access from thread that might be concurrent with writing
    int64_t get_last_modified();
    void set_last_modified(int64_t lastModified);

    int64_t get_size_in_bytes();

    ByteArray add_buffer(int32_t size);
    ByteArray get_buffer(int32_t index);
    int32_t num_buffers();

protected:
    /// Allocate a new buffer. Subclasses can allocate differently.
    virtual ByteArray new_buffer(int32_t size);

};

} // namespace Lucene
#endif // RAM_FILE_H
