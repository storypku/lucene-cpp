#include "lucene.h"
#include "buffered_index_output.h"
#include "misc_utils.h"

namespace Lucene {

const int32_t BufferedIndexOutput::BUFFER_SIZE = 16384;

BufferedIndexOutput::BufferedIndexOutput()
    : m_bufferStart(0)
    , m_bufferPosition(0)
    , m_buffer(ByteArray::new_instance(BUFFER_SIZE)) {

}

BufferedIndexOutput::~BufferedIndexOutput() {
}

void BufferedIndexOutput::write_byte(uint8_t b) {
    if (m_bufferPosition >= BUFFER_SIZE) {
        flush();
    }
    m_buffer[m_bufferPosition++] = b;
}

void BufferedIndexOutput::write_bytes(const uint8_t* b, int32_t offset, int32_t length) {
    int32_t bytesLeft = BUFFER_SIZE - m_bufferPosition;
    if (bytesLeft >= length) {
        // we add the data to the end of the buffer.
        MiscUtils::array_copy(b, offset, m_buffer.get(), m_bufferPosition, length);
        m_bufferPosition += length;
        // if the buffer is full, flush it.
        if (BUFFER_SIZE == m_bufferPosition) {
            flush();
        }
    } else if (length > BUFFER_SIZE) {
        // we flush the buffer.
        if (m_bufferPosition > 0) {
            flush();
        }
        // and write data at once
        flush_buffer(b, offset, length);
        m_bufferStart += length;
    } else { // bytesLeft < lenth <= BUFFER_SIZE
        // we fill/flush the buffer (until the input is written);
        int32_t pos = 0; // position in the input data.
        int32_t pieceLength;
        while (pos < length) {
            pieceLength = (length - pos < bytesLeft) ?  (length - pos) : bytesLeft;
            MiscUtils::array_copy(b, pos + offset, m_buffer.get(), m_bufferPosition, pieceLength);
            pos += pieceLength;
            m_bufferPosition += pieceLength;
            bytesLeft = BUFFER_SIZE - m_bufferPosition;
            if (bytesLeft == 0) {
                flush();
                bytesLeft = BUFFER_SIZE;
            }
        }
    }
}

void BufferedIndexOutput::flush() {
    flush_buffer(m_buffer.get(), m_bufferPosition);
    m_bufferStart += m_bufferPosition;
    m_bufferPosition = 0;
}

void BufferedIndexOutput::flush_buffer(const uint8_t* b, int32_t length) {
    flush_buffer(b, 0, length);
}

void BufferedIndexOutput::flush_buffer(UNUSED const uint8_t* b, UNUSED int32_t offset, UNUSED int32_t length) {
    // override
}

void BufferedIndexOutput::close() {
    flush();
}

int64_t BufferedIndexOutput::get_file_pointer() {
    return m_bufferStart + m_bufferPosition;
}

void BufferedIndexOutput::seek(int64_t pos) {
    flush();
    m_bufferStart = pos;
}

}
