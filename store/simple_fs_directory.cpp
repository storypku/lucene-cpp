#include "lucene.h"
#include <fstream>
#include "simple_fs_directory.h"
#include "index_output.h"
#include "file_reader.h"
#include "file_utils.h"

namespace Lucene {

SimpleFSDirectory::SimpleFSDirectory(const String& path, const LockFactoryPtr& lockFactory)
    : FSDirectory(path, lockFactory) {
}

SimpleFSDirectory::~SimpleFSDirectory() {
}

IndexOutputPtr SimpleFSDirectory::create_output(const String& name) {
    init_output(name);
    return new_lucene<SimpleFSIndexOutput>(FileUtils::join_path(m_directory, name));
}

IndexInputPtr SimpleFSDirectory::open_input(const String& name) {
    return FSDirectory::open_input(name);
}

IndexInputPtr SimpleFSDirectory::open_input(const String& name, int32_t bufferSize) {
    ensure_open();
    return new_lucene<SimpleFSIndexInput>(FileUtils::join_path(m_directory, name),
                                          bufferSize, get_read_chunk_size());
}

const int32_t InputFile::FILE_EOF = FileReader::FILE_EOF;
const int32_t InputFile::FILE_ERROR = FileReader::FILE_ERROR;

InputFile::InputFile(const String& path) {
    m_file = new_instance<std::ifstream>(path, std::ios::binary | std::ios::in);
    if (!m_file->is_open()) {
        throw FileNotFoundException(path);
    }

    m_position = 0;
    m_length = FileUtils::file_length(path);
}

InputFile::~InputFile() {
}

void InputFile::set_position(int64_t pos) {
    this->m_position = pos;
    m_file->seekg((std::streamoff)pos);
    if (!m_file->good()) { // it seems seekg wont make m_file go bad...
        throw IOException();
    }
}

int64_t InputFile::get_position() {
    return m_position;
}

int64_t InputFile::get_length() {
    return m_length;
}

int32_t InputFile::read(uint8_t *b, int32_t offset, int32_t length) {
    try {
        if (m_file->eof()) {
            return FILE_EOF;
        }
        m_file->read((char *)b + offset, length);
        int32_t readCount = m_file->gcount();
        m_position += readCount;
        return readCount;
    } catch (...) {
        return FILE_ERROR;
    }
}

void InputFile::close() {
    if (m_file->is_open()) {
        m_file->close();
    }
}

bool InputFile::is_valid() {
    return m_file && m_file->is_open() && m_file->good();
}

SimpleFSIndexInput::SimpleFSIndexInput()
    : m_chunkSize(0)
    , m_isClone(false) {
}

SimpleFSIndexInput::SimpleFSIndexInput(const String& path, int32_t bufferSize, int32_t chunkSize)
    : BufferedIndexInput(bufferSize)
    , m_path(path)
    , m_chunkSize(chunkSize)
    , m_isClone(false) {
        this->m_file = new_lucene<InputFile>(path);
}

SimpleFSIndexInput::~SimpleFSIndexInput() {
}

void SimpleFSIndexInput::read_internal(uint8_t* b, int32_t offset, int32_t length) {
    SyncLock fileLock(m_file);
    int64_t position = get_file_pointer();
    BOOST_ASSERT(position == m_file->get_position());
    // if (position != m_file->get_position()) {
    //    m_file->set_position(position);
    // }

    int32_t total = 0;
    while (total < length) {
        int32_t readLength = total + m_chunkSize > length ? (length - total) : m_chunkSize;
        int32_t i = m_file->read(b, offset + total, readLength);
        if (i == InputFile::FILE_EOF) {
            throw IOException("Read past EOF");
        }
        total += i;
    }
}

void SimpleFSIndexInput::seek_internal(int64_t pos) {
    m_file->set_position(pos);
}

int64_t SimpleFSIndexInput::length() {
    return m_file->get_length();
}

void SimpleFSIndexInput::close() {
    if (!m_isClone) {
        m_file->close();
    }
}

bool SimpleFSIndexInput::is_valid() {
    return m_file->is_valid();
}

LuceneObjectPtr SimpleFSIndexInput::clone(const LuceneObjectPtr& other) {
    LuceneObjectPtr clone = BufferedIndexInput::clone(other ? other : new_lucene<SimpleFSIndexInput>());
    SimpleFSIndexInputPtr cloneIndexInput(std::dynamic_pointer_cast<SimpleFSIndexInput>(clone));
    cloneIndexInput->m_path = m_path;
    cloneIndexInput->m_file = m_file;
    cloneIndexInput->m_chunkSize = m_chunkSize;
    cloneIndexInput->m_isClone = true;
    return cloneIndexInput;
}

OutputFile::OutputFile(const String& path)
    : m_path(path) {
    m_file = new_instance<std::ofstream>(m_path, std::ios::binary | std::ios::out);
}

OutputFile::~OutputFile() {
}

bool OutputFile::write(const uint8_t* b, int32_t offset, int32_t length) {
    if (!m_file->is_open()) {
        return false;
    }
    try {
        m_file->write((char *)b + offset, length);
        return m_file->good();
    } catch (...) {
        return false;
    }
}

void OutputFile::close() {
    m_file.reset();
}

void OutputFile::set_position(int64_t position) {
    m_file->seekp((std::streamoff)position);
    if (!m_file->good()) {
        throw IOException();
    }
}

int64_t OutputFile::get_length() {
    return FileUtils::file_length(m_path);
}

void OutputFile::set_length(int64_t length) {
    FileUtils::set_file_length(m_path, length);
}

void OutputFile::flush() {
    if (m_file->is_open()) {
        m_file->flush();
    }
}

bool OutputFile::is_valid() {
    return m_file && m_file->is_open() && m_file->good();
}

SimpleFSIndexOutput::SimpleFSIndexOutput(const String& path) {
    m_file = new_lucene<OutputFile>(path);
    m_isOpen = true;
}

SimpleFSIndexOutput::~SimpleFSIndexOutput() {
}

void SimpleFSIndexOutput::flush_buffer(const uint8_t* b, int32_t offset, int32_t length) {
    m_file->write(b, offset, length);
    m_file->flush();
}

void SimpleFSIndexOutput::close() {
    if (m_isOpen) {
        BufferedIndexOutput::close();
        m_file.reset();
        m_isOpen = false;
    }
}

void SimpleFSIndexOutput::seek(int64_t pos) {
    BufferedIndexOutput::seek(pos);
    m_file->set_position(pos);
}

int64_t SimpleFSIndexOutput::length() {
    return m_file->get_length();
}

void SimpleFSIndexOutput::set_length(int64_t length) {
    m_file->set_length(length);
}

} // namespace Lucene

