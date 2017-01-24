#ifndef SIMPLE_FS_DIRECTORY_H
#define SIMPLE_FS_DIRECTORY_H

#include "fs_directory.h"
#include "buffered_index_input.h"
#include "buffered_index_output.h"

namespace Lucene {

/// A straightforward implementation of FSDirectory using std::ofstream and
/// std::ifstream.
class SimpleFSDirectory : public FSDirectory {
public:
    /// Create a new SimpleFSDirectory for the named location and
    /// NativeFSLockFactory.
    /// @param path the path of the directory
    /// @param lockFactory the lockFactory to use or null for the default
    /// (NativeFSLockFactory)
    SimpleFSDirectory(const String& path, const LockFactoryPtr& lockFactory = LockFactoryPtr());

    virtual ~SimpleFSDirectory();

    LUCENE_CLASS(SimpleFSDirectory);

public:
    virtual IndexOutputPtr create_output(const String& name);

    virtual IndexInputPtr open_input(const String& name);

    virtual IndexInputPtr open_input(const String& name, int32_t bufferSize);
};


class InputFile : public LuceneObject {
public:
    InputFile(const String& path);
    virtual ~InputFile();

    LUCENE_CLASS(InputFile);

public:
    static const int32_t FILE_EOF;
    static const int32_t FILE_ERROR;

protected:
    IfstreamPtr m_file;
    int64_t m_position;
    int64_t m_length;

public:
    void set_position(int64_t position);
    int64_t get_position();
    int64_t get_length();
    int32_t read(uint8_t* b, int32_t offset, int32_t length);
    void close();
    bool is_valid();
};

class SimpleFSIndexInput : public BufferedIndexInput {
public:
    SimpleFSIndexInput();
    SimpleFSIndexInput(const String& path, int32_t bufferSize, int32_t chunkSize);
    virtual ~SimpleFSIndexInput();

    LUCENE_CLASS(SimpleFSIndexInput);

protected:
    String m_path;
    InputFilePtr m_file;
    int32_t m_chunkSize;
    bool m_isClone;

protected:
    virtual void read_internal(uint8_t* b, int32_t offset, int32_t length);
    virtual void seek_internal(int64_t pos);

public:
    virtual int64_t length();
    virtual void close();

    /// Method used for testing.
    bool is_valid();

    /// Returns a clone of this stream.
    virtual LuceneObjectPtr clone(const LuceneObjectPtr& other = LuceneObjectPtr());
};

class OutputFile : public LuceneObject {
public:
    OutputFile(const String& path);
    virtual ~OutputFile();

    LUCENE_CLASS(OutputFile);

protected:
    OfstreamPtr m_file;
    String m_path;

public:
    bool write(const uint8_t* b, int32_t offset, int32_t length);
    void close();
    void set_position(int64_t position);
    int64_t get_length();
    void set_length(int64_t length);
    void flush();
    bool is_valid();
};

class SimpleFSIndexOutput : public BufferedIndexOutput {
public:
    SimpleFSIndexOutput(const String& path);
    virtual ~SimpleFSIndexOutput();

    LUCENE_CLASS(SimpleFSIndexOutput);

protected:
    OutputFilePtr m_file;
    bool m_isOpen;

public:
    virtual void flush_buffer(const uint8_t* b, int32_t offset, int32_t length);
    virtual void close();
    virtual void seek(int64_t pos);
    virtual int64_t length();
    virtual void set_length(int64_t length);
};

} // namespace Lucene

#endif // SIMPLE_FS_DIRECTORY_H
