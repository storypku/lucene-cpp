#ifndef BUFFERED_INDEX_INPUT_H
#define BUFFERED_INDEX_INPUT_H

#include "index_input.h"

namespace Lucene {
/// Base implementation class for buffered IndexInput.

class BufferedIndexInput : public IndexInput {
public:
    BufferedIndexInput(int32_t bufferSize = BUFFER_SIZE);
    virtual ~BufferedIndexInput();

    LUCENE_CLASS(BufferedIndexInput);

public:
    static const int32_t BUFFER_SIZE;

protected:
    int32_t m_bufferSize;
    int64_t m_bufferStart; // position in file of buffer
    int32_t m_bufferLength; // end of valid bytes.
    int32_t m_bufferPosition; // next byte to read.
    ByteArray m_buffer;

protected:
    virtual void read_bytes_ex(uint8_t* b, int32_t offset, int32_t length);

public:
    /// Reads and returns a single byte.
    virtual uint8_t read_byte();

    /// Change the buffer size used by this IndexInput.
    void set_buffer_size(int32_t newSize);

    /// Returns buffer size.
    int32_t get_buffer_size();

    /// Reads a specified number of bytes into an array at the specified offset.
    /// @param b the array to read bytes into.
    /// @param offset the offset in the array to start storing bytes.
    /// @param length the number of bytes to read.
    virtual void read_bytes(uint8_t *b, int32_t offset, int32_t length);

    /// Close the stream.
    virtual void close();

    /// Returns the current position in this file, where the next read will occur.
    virtual int64_t get_file_pointer();

    /// Sets current position in this file, where the next read will occur.
    virtual void seek(int64_t pos);

    /// Returns a clone of this stream.
    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());

protected:
    virtual void new_buffer(ByteArray newBuffer);

    /// Refill buffer in preparation for read.
    virtual void refill();

    /// Implements buffer refill. Reads bytes from the current position in the
    /// input.
    /// @param b the array to read bytes into.
    /// @param offset the offset in the array to start storing bytes.
    /// @param length the number of bytes to read.
    virtual void read_internal(uint8_t *b, int32_t offset, int32_t length) = 0;

    /// Implements seek. Set current position in this file, where the next
    /// #read_internal(uint8_t*, int32_t, int32_t) will occur.
    virtual void seek_internal(int64_t pos) = 0;
};

} // namespace Lucene

#endif // BUFFERED_INDEX_INPUT_H

