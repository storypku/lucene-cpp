#ifndef RAM_OUTPUT_STREAM_H
#define RAM_OUTPUT_STREAM_H

#include "index_output.h"
#include "ram_file.h"

namespace Lucene {

class RAMOutputStream : public IndexOutput {
public:
    RAMOutputStream();
    RAMOutputStream(const RAMFilePtr& f);

    virtual ~RAMOutputStream() = default;
    LUCENE_CLASS(RAMOutputStream);

public:
    static const int32_t BUFFER_SIZE;

protected:
    RAMFilePtr m_file;
    ByteArray m_currentBuffer;
    int32_t m_currentBufferIndex;
    int32_t m_bufferPosition;
    int64_t m_bufferStart;
    int32_t m_bufferLength;

public:
    /// Copy the current contents of this buffer to the named output.
    void write_to(const IndexOutputPtr& output);

    /// Resets this to an empty file.
    void reset();

    virtual void close();

    /// Sets current position in this file, where the next write will occur.
    virtual void seek(int64_t pos);

    virtual int64_t get_file_pointer();

    /// The number of bytes in this file.
    virtual int64_t length();

    virtual void write_byte(uint8_t b);

    virtual void write_bytes(const uint8_t* b, int32_t offset, int32_t length);

    /// Forces any buffered output to be written.
    virtual void flush();

    /// Returns byte usage of all buffers.
    int64_t size_in_bytes();

protected:
    void switch_current_buffer();
    void set_file_length();
};

} // namespace Lucene
#endif // RAM_OUTPUT_STREAM_H
