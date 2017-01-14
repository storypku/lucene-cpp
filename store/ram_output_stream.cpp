#include "lucene.h"
#include "ram_output_stream.h"
#include "ram_file.h"
#include "ram_directory.h"
#include "misc_utils.h"

namespace Lucene {

const int32_t RAMOutputStream::BUFFER_SIZE = 1024;

RAMOutputStream::RAMOutputStream()
    : m_file(new_lucene<RAMFile>(RAMDirectoryPtr()))
    , m_currentBufferIndex(-1)
    , m_bufferPosition(0)
    , m_bufferStart(0)
    , m_bufferLength(0) {
}

RAMOutputStream::RAMOutputStream(const RAMFilePtr& f)
    : m_file(f)
    , m_currentBufferIndex(-1)
    , m_bufferPosition(0)
    , m_bufferStart(0)
    , m_bufferLength(0) {
    // make sure that we switch to the first needed buffer lazily.
}

void RAMOutputStream::write_to(const IndexOutputPtr& out) {
    this->flush();
    int64_t end = m_file->get_length();
    int64_t pos = 0;
    int32_t buffer = 0;
    while (pos < end) {
        int32_t length = BUFFER_SIZE;
        int64_t nextPos = pos + length;
        if (nextPos > end) {
            length = (int32_t)(end - pos);
        }
        out->write_bytes(m_file->get_buffer(buffer++).get(), length);
        pos = nextPos;
    }
}

void RAMOutputStream::reset() {
    m_currentBuffer.reset();
    m_currentBufferIndex = -1;
    m_bufferPosition = 0;
    m_bufferStart = 0;
    m_bufferLength = 0;
    m_file->set_length(0);
}

void RAMOutputStream::close() {
    this->flush();
}

int64_t RAMOutputStream::length() {
    return m_file->get_length();
}

void RAMOutputStream::seek(int64_t pos) {
    // set the file length in case we seek back and flush() has not been called yet
    set_file_length();

    if (pos < m_bufferStart || pos >= m_bufferStart + m_bufferLength) {
        m_currentBufferIndex = (int32_t)(pos / BUFFER_SIZE);
        switch_current_buffer();
    }
    m_bufferPosition = (int32_t)(pos % BUFFER_SIZE);
}

void RAMOutputStream::write_byte(uint8_t b) {
    if (m_bufferPosition == m_bufferLength) {
        ++ m_currentBufferIndex;
        switch_current_buffer();
    }
    m_currentBuffer[m_bufferPosition] = b;
}

void RAMOutputStream::write_bytes(const uint8_t *b, int32_t offset, int32_t length) {
    BOOST_ASSERT(b != NULL);
    while (length > 0) {
        if (m_bufferPosition == m_bufferLength) {
            ++ m_currentBufferIndex;
            switch_current_buffer();
        }
        int32_t remainInBuffer = m_currentBuffer.size() - m_bufferPosition;
        int32_t bytesToCopy = length < remainInBuffer ? length : remainInBuffer;
        MiscUtils::array_copy(b, offset, m_currentBuffer.get(), m_bufferPosition, bytesToCopy);
        offset += bytesToCopy;
        length -= bytesToCopy;
        m_bufferPosition += bytesToCopy;
    }
}

void RAMOutputStream::switch_current_buffer() {
    if (m_currentBufferIndex == m_file->num_buffers()) {
        m_currentBuffer = m_file->add_buffer(BUFFER_SIZE);
    } else {
        m_currentBuffer = m_file->get_buffer(m_currentBufferIndex);
    }
    m_bufferPosition = 0;
    m_bufferStart = (int64_t)BUFFER_SIZE * (int64_t)m_currentBufferIndex;
    m_bufferLength = m_currentBuffer.size();
}

void RAMOutputStream::set_file_length() {
    int64_t pointer = m_bufferStart + m_bufferPosition;
    if (pointer > m_file->get_length()) {
        m_file->set_length(pointer);
    }
}

void RAMOutputStream::flush() {
    m_file->set_last_modified(MiscUtils::current_time_millis());
    set_file_length();
}

int64_t RAMOutputStream::get_file_pointer() {
    return m_currentBufferIndex < 0 ? 0 : m_bufferStart + m_bufferPosition;
}

int64_t RAMOutputStream::size_in_bytes() {
    return m_file->num_buffers() * BUFFER_SIZE;
}

} // namespace Lucene

