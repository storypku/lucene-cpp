#ifndef FILE_READER_H
#define FILE_READER_H

#include "reader.h"

namespace Lucene {

/// Convenience class for reading character files.
class FileReader : public Reader {
public:
    /// Create a new FileReader, given the file name to read from.
    FileReader(const String& fileName);
    virtual ~FileReader();

    LUCENE_CLASS(FileReader);

protected:
    IfstreamPtr m_file;
    int64_t m_length_;
    ByteArray m_fileBuffer;

public:
    static const int32_t FILE_EOF;
    static const int32_t FILE_ERROR;

public:
    /// Read a single character.
    virtual int32_t read();

    /// Read characters into a portion of an array.
    virtual int32_t read(char* buffer, int32_t offset, int32_t length);

    /// Close the stream.
    virtual void close();

    /// Tell whether this stream supports the mark() operation.
    virtual bool mark_supported();

    /// Reset the stream.
    virtual void reset();

    /// The number of bytes in the file.
    virtual int64_t length();
};

} // namespace Lucene

#endif // FILE_READER_H
