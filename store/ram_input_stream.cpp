#include "lucene.h"
#include "ram_input_stream.h"
#include "ram_file.h"
#include "ram_output_stream.h"
#include "misc_utils.h"

namespace Lucene {

const int32_t RAMInputStream::BUFFER_SIZE = RAMOutputStream::BUFFER_SIZE;

RAMInputStream::RAMInputStream()
    : m_length(0)
    , m_currentBufferIndex(-1)
    , m_bufferPosition(0)
    , m_bufferStart(0)
    , m_bufferLength(0) {
}

RAMInputStream::RAMInputStream(const RAMFilePtr& f)
    : m_file(f)
    , m_currentBufferIndex(-1)
    , m_bufferPosition(0)
    , m_bufferStart(0)
    , m_bufferLength(0) {

    m_length = f->get_length();

    if (m_length / BUFFER_SIZE >= std::numeric_limits<int>::max()) {
        throw IOException("Too large RAMFile: " + StringUtils::to_string(m_length));
    }
}

void RAMInputStream::close() {
}

int64_t RAMInputStream::length() {
    return m_length;
}

uint8_t RAMInputStream::read_byte() {
    if (m_bufferPosition >= m_bufferLength) {
        ++ m_currentBufferIndex;
        switch_current_buffer(true);
    }

    return m_currentBuffer[m_bufferPosition++];
}

void RAMInputStream::read_bytes(uint8_t* b, int32_t offset, int32_t length) {
    while (length > 0) {
        if (m_bufferPosition >= m_bufferLength) {
            ++ m_currentBufferIndex;
            switch_current_buffer(true);
        }

        int32_t remainInBuffer = m_bufferLength - m_bufferPosition;
        int32_t bytesToCopy = length < remainInBuffer ? length : remainInBuffer;
        MiscUtils::array_copy(m_currentBuffer.get(), m_bufferPosition, b, offset, bytesToCopy);
        offset += bytesToCopy;
        length -= bytesToCopy;

        m_bufferPosition += bytesToCopy;
    }
}

// enforceEOF的作用，使seek的时候不抛异常，而read的时候抛异常。
void RAMInputStream::switch_current_buffer(bool enforceEOF) {
    if (m_currentBufferIndex >= m_file->num_buffers()) {
        // end of file reached, no more buffers left
        if (enforceEOF) {
            throw IOException("Read past EOF");
        } else {
            // force eof if a read takes place at this position.
            -- m_currentBufferIndex;
            m_bufferPosition = BUFFER_SIZE; // 狐狸尾巴
        }
    } else {
        m_currentBuffer = m_file->get_buffer(m_currentBufferIndex);
        m_bufferPosition = 0;
        m_bufferStart = (int64_t)BUFFER_SIZE * (int64_t)m_currentBufferIndex;
        int64_t lengthLeft = m_length - m_bufferStart;
        m_bufferLength = lengthLeft > BUFFER_SIZE ? BUFFER_SIZE : (int32_t)lengthLeft;
    }
}

int64_t RAMInputStream::get_file_pointer() {
    return m_currentBufferIndex < 0 ? 0 : m_bufferStart + m_bufferPosition;
}

void RAMInputStream::seek(int64_t pos) {
    if (!m_currentBuffer || pos < m_bufferStart || pos >= m_bufferStart + BUFFER_SIZE) {
        m_currentBufferIndex = (int32_t)(pos / BUFFER_SIZE);
        switch_current_buffer(false);
    }

    m_bufferPosition = (int32_t)(pos % BUFFER_SIZE);
}

LuceneObjectPtr RAMInputStream::clone(const LuceneObjectPtr& other) {
    LuceneObjectPtr clone = IndexInput::clone(other) ? other : new_lucene<RAMInputStream>();
    RAMInputStreamPtr cloneInputStream(std::dynamic_pointer_cast<RAMInputStream>(clone));
    cloneInputStream->m_file = m_file;
    cloneInputStream->m_length = m_length;
    cloneInputStream->m_currentBuffer = m_currentBuffer;
    cloneInputStream->m_currentBufferIndex = m_currentBufferIndex;
    cloneInputStream->m_bufferPosition = m_bufferPosition;
    cloneInputStream->m_bufferStart = m_bufferStart;
    cloneInputStream->m_bufferLength = m_bufferLength;
    return cloneInputStream;
}

} // namespace Lucene

