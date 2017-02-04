#include "lucene.h"
#include "mmap_directory.h"
#include "simple_fs_directory.h"
#include "misc_utils.h"
#include "file_utils.h"

namespace Lucene {

MMapDirectory::MMapDirectory(const String& path, const LockFactoryPtr& lockFactory)
    : FSDirectory(path, lockFactory) {
}

MMapDirectory::~MMapDirectory() {
}

IndexInputPtr MMapDirectory::open_input(const String& name, UNUSED int32_t bufferSize) {
    ensure_open();
    return new_lucene<MMapIndexInput>(FileUtils::join_path(m_directory, name));
}

IndexOutputPtr MMapDirectory::create_output(const String& name) {
    init_output(name);
    return new_lucene<SimpleFSIndexOutput>(FileUtils::join_path(m_directory, name));
}

MMapIndexInput::MMapIndexInput(const String& path) {
    m_length = path.empty() ? 0 : (int32_t)FileUtils::file_length(path);
    m_bufferPosition = 0;
    if (!path.empty()) {
        try {
            m_file.open(path, m_length);
        } catch (...) {
            throw FileNotFoundException("Read past EOF");
        }
    }
    m_isClone = false;
}

MMapIndexInput::~MMapIndexInput() {
}

uint8_t MMapIndexInput::read_byte() {
    try {
        return m_file.data()[m_bufferPosition++];
    } catch (...) {
        throw IOException("Read past EOF");
        return 0;
    }
}

void MMapIndexInput::read_bytes(uint8_t* b, int32_t offset, int32_t length) {
    try {
        MiscUtils::array_copy(m_file.data(), m_bufferPosition, b, offset, length);
        m_bufferPosition += length;
    } catch (...) {
        throw IOException("Read past EOF");
    }
}

int64_t MMapIndexInput::get_file_pointer() {
    return m_bufferPosition;
}

void MMapIndexInput::seek(int64_t pos) {
    m_bufferPosition = (int32_t)pos;
}

int64_t MMapIndexInput::length() {
    return (int64_t)m_length;
}

void MMapIndexInput::close() {
    if (m_isClone || !m_file.is_open()) {
        return;
    }
    m_length = 0;
    m_bufferPosition = 0;
    m_file.close();
}

LuceneObjectPtr MMapIndexInput::clone(const LuceneObjectPtr& other) {
    if (!m_file.is_open()) {
        throw AlreadyClosedException("MMapIndexInput already closed");
    }
    LuceneObjectPtr clone = IndexInput::clone(other ? other : new_lucene<MMapIndexInput>());
    MMapIndexInputPtr cloneIndexInput(std::dynamic_pointer_cast<MMapIndexInput>(clone));
    cloneIndexInput->m_length = m_length;
    cloneIndexInput->m_file = m_file;
    cloneIndexInput->m_bufferPosition = m_bufferPosition;
    cloneIndexInput->m_isClone = true;
    return cloneIndexInput;
}

} // namespace Lucene

