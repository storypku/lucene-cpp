#ifndef INDEX_INPUT_H
#define INDEX_INPUT_H

#include "lucene_object.h"

namespace Lucene {

/// Abstract base class for input from a file in a Directory.
/// A random-access input stream. Used for all Lucene index input operations.
class IndexInput : public LuceneObject {
public:
    IndexInput() = default;
    virtual ~IndexInput() = default;

public:
    /// Reads and returns a single byte.
    virtual uint8_t read_byte() = 0;

    /// Reads a specified number of bytes into an array at the specified offset.
    /// @param b the array to read bytes into.
    /// @param offset the offset in the array to start storing bytes.
    /// @param length the number of bytes to read.
    virtual void read_bytes(uint8_t* b, int32_t offset, int32_t length);

    /// Reads four bytes and returns an int.
    virtual int32_t read_int();

    /// Reads an int stored in variable-length format.  Reads between one and five
    /// bytes.  Smaller values take fewer bytes.  Negative numbers are not supported.
    virtual int32_t read_vint();

    /// Reads eight bytes and returns a int64.
    virtual int64_t read_long();


    /// Reads a int64 stored in variable-length format.  Reads between one and nine
    /// bytes.  Smaller values take fewer bytes.  Negative numbers are not supported.
    virtual int64_t read_vlong();

    /// Reads a string.
    virtual String read_string();

    /// Close the stream.
    virtual void close() = 0;

    /// Returns the current position in this file, where next read will occur.
    virtual int64_t get_file_pointer() = 0;

    /// Set current position in this file, where the next read will occur.
    virtual void seek(int64_t pos) = 0;

    /// The number of bytes in the file.
    virtual int64_t length() = 0;

    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());

    /// Read string map as a series of key/value pairs.
    virtual MapStringString read_string_string_map();
};

} // namespace Lucene
#endif
