#ifndef INDEX_OUTPUT_H
#define INDEX_OUTPUT_H

#include "lucene.h"

namespace Lucene {

/// Abstract base class for output to a file in a Directory. A random-access
/// output stream. Used for all Lucene index output operations.
class IndexOutput : public LuceneObject {
public:
    virtual ~IndexOutput() = default;

    LUCENE_CLASS(IndexOutput);

protected:
    static const int32_t COPY_BUFFER_SIZE;
    ByteArray m_copyBuffer;

public:
    /// Writes a single byte.
    virtual void write_byte(uint8_t b) = 0;

    /// Writes an array of bytes.
    /// @param b the bytes to write.
    /// @param length the number of bytes to write.
    virtual void write_bytes(const uint8_t* b, int32_t offset, int32_t length) = 0;

    /// Forces any buffered output to be written.
    virtual void flush() = 0;

    /// Close this stream.
    virtual void close() = 0;

    /// Returns the current position in this file, where the next write will
    /// occur.
    virtual int64_t get_file_pointer() = 0;

    /// Sets current position in this file, where the next write will occur.
    virtual void seek(int64_t pos) = 0;

    /// The number of bytes in the file.
    virtual int64_t length() = 0;

public:
    /// Writes an array of bytes.
    /// @param b the bytes to write.
    /// @param length the number of bytes to write.
    void write_bytes(const uint8_t* b, int32_t length);

    /// Writes an int as four bytes.
    void write_int(int32_t i);

    /// Writes an int in a variable-length format.  Writes between one and five bytes.  Smaller values take fewer bytes.
    /// Negative numbers are not supported.
    void write_vint(int32_t i);

    /// Writes a int64 as eight bytes.
    void write_long(int64_t i);

    /// Writes an int64 in a variable-length format.  Writes between one and
    /// five bytes.  Smaller values take fewer bytes. Negative numbers are not
    /// supported.
    void write_vlong(int64_t i);

    /// Write a string.
    void write_string(const String& s);

    /// Copy numBytes bytes from input to ourself.
    void copy_bytes(const IndexInputPtr& input, int64_t numBytes);

    /// Set the file length. By default, this method does nothing (it's
    /// optional for a Directory to implement it). But, certain Directory
    /// implementations (for example FSDirectory) can use this to inform
    /// the underlying IO system to pre-allocate the file to the specified
    /// size.  If the length is longer than the current file length, the
    /// bytes added to the file are undefined.  Otherwise the file is truncated.
    /// @param length file length.
    void set_length(int64_t length);

    /// Write string map as a series of key/value pairs.
    /// @param map map of string-string key-values.
    void write_string_string_map(MapStringString map);
};

} // namespace Lucene
#endif // INDEX_OUTPUT_H
