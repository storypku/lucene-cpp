#ifndef BUFFERED_INDEX_OUTPUT_H
#define BUFFERED_INDEX_OUTPUT_H

#include "index_output.h"

namespace Lucene {

/// Base class implementation for buffered IndexOutput
class BufferedIndexOutput : public IndexOutput {
public:
    BufferedIndexOutput();
    virtual ~BufferedIndexOutput();

    LUCENE_CLASS(BufferedIndexOutput);

public:
    static const int32_t BUFFER_SIZE;

protected:
    int64_t m_bufferStart;
    int32_t m_bufferPosition;
    ByteArray m_buffer;

public:
    /// Writes a single byte.
    virtual void write_byte(uint8_t b);

    /// Writes an array of bytes.
    /// @param b the bytes to write.
    /// @param length the number of bytes to write.
    virtual void write_bytes(const uint8_t* b, int32_t offset, int32_t length);

    /// Forces any buffered output to be written.
    virtual void flush();

    /// Implements buffer write. Writes bytes at the current position in the
    /// output.
    /// @param b the bytes to write.
    /// @param offset the offset in the byte array.
    /// @param length the number of bytes to write.
    virtual void flush_buffer(const uint8_t* b, int32_t offset, int32_t length);

    /// Close this stream.
    virtual void close();

    /// Returns the current position in this file, where the next write will
    /// occur.
    virtual int64_t get_file_pointer();

    /// Set current position in this file, where the next write will occur.
    virtual void seek(int64_t pos);

    /// The number of bytes in the file.
    virtual int64_t length() = 0;

protected:
    /// Implements buffer write. Writes bytes at the current position in the
    /// output.
    /// @param b the bytes to write.
    /// @param length the number of bytes to write.
    void flush_buffer(const uint8_t* b, int32_t length);

};

}
#endif // BUFFERED_INDEX_OUTPUT_H
