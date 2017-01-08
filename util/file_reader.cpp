#include "lucene.h"
#include "file_reader.h"
#include "misc_utils.h"
#include "file_utils.h"

namespace Lucene {

const int32_t FileReader::FILE_EOF = Reader::READER_EOF;
const int32_t FileReader::FILE_ERROR = -1;

FileReader::FileReader(const String& fileName) {
    this->m_file = new_instance<std::ifstream>(fileName, std::ios::binary | std::ios::in);

    if (!m_file->is_open()) {
        throw FileNotFoundException(fileName);
    }

    m_length_ = FileUtils::file_length(fileName);
}

FileReader::~FileReader() {
}

int32_t FileReader::read() {
    char buffer;
    return read(&buffer, 0, 1) == FILE_EOF ? FILE_EOF : buffer;
}

int32_t FileReader::read(char *buffer, int32_t offset, int32_t length) {
    try {
        if (m_file->eof()) {
            return FILE_EOF;
        }
        if (!m_fileBuffer) {
            m_fileBuffer = ByteArray::new_instance(length);
        }
        if (length > m_fileBuffer.size()) {
            m_fileBuffer.resize(length);
        }

        m_file->read((char *)m_fileBuffer.get(), length);
        int32_t readLength = m_file->gcount();
        MiscUtils::array_copy(m_fileBuffer.get(), 0, buffer, offset, readLength);

        return readLength == 0 ? FILE_EOF : readLength;
    } catch (...) {
        return FILE_ERROR;
    }
}

void FileReader::close() {
    m_file->close();
}

bool FileReader::mark_supported() {
    return false;
}

void FileReader::reset() {
    m_file->clear();
    m_file->seekg((std::streamoff)0);
}

int64_t FileReader::length() {
    return m_length_;
}

} // namespace Lucene


