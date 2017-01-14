#ifndef RAM_INPUT_STREAM_H
#define RAM_INPUT_STREAM_H

#include "index_input.h"

namespace Lucene {

/// A memory-resident IndexInput implementation.
class RAMInputStream : public IndexInput {
public:
    RAMInputStream();
    RAMInputStream(const RAMFilePtr& f);
    virtual ~RAMInputStream() = default;

    LUCENE_CLASS(RAMInputStream);

public:
    static const int32_t BUFFER_SIZE;

protected:
    RAMFilePtr m_file;
    int64_t m_length;
    ByteArray m_currentBuffer;
    int32_t m_currentBufferIndex;
    int32_t m_bufferPosition;
    int64_t m_bufferStart;
    int32_t m_bufferLength;

public:
    virtual void close();

    virtual int64_t length();

    virtual uint8_t read_byte();

    virtual void read_bytes(uint8_t* b, int32_t offset, int32_t length);

    virtual int64_t get_file_pointer();

    virtual void seek(int64_t pos);

    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());

protected:
    void switch_current_buffer(bool enforceEOF);
};

} // namespace Lucene

#endif // RAM_INPUT_STREAM_H
