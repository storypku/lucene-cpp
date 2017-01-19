#include "lucene.h"
#include "buffered_index_input.h"
#include "misc_utils.h"
#include "string_utils.h"

namespace Lucene {

const int32_t BufferedIndexInput::BUFFER_SIZE = 1024;

BufferedIndexInput::BufferedIndexInput(int32_t bufferSize)
    : m_bufferSize(bufferSize)
    , m_bufferStart(0)
    , m_bufferLength(0)
    , m_bufferPosition(0) {
    //
}

BufferedIndexInput::~BufferedIndexInput() {
}

uint8_t BufferedIndexInput::read_byte() {
    if (m_bufferPosition >= m_bufferLength) {
        refill();
    }
    return m_buffer[m_bufferPosition++];
}

void BufferedIndexInput::set_buffer_size(int32_t newSize) {
    if (newSize == m_bufferSize) {
        return;
    }

    m_bufferSize = newSize;
    if (m_buffer) {
        /// Resize the existing buffer and carefully save as many bytes as
        /// possible starting from the current buffer position;
        ByteArray _newBuffer(ByteArray::new_instance(newSize));
        int32_t leftInBuffer = m_bufferLength - m_bufferPosition;
        int32_t numToCopy = leftInBuffer  > newSize ? newSize : leftInBuffer;
        MiscUtils::array_copy(m_buffer.get(), m_bufferPosition, _newBuffer.get(), 0, numToCopy);
        m_bufferStart += m_bufferPosition;
        m_bufferPosition = 0;
        m_bufferLength = numToCopy;
        new_buffer(_newBuffer);
    }
}

void BufferedIndexInput::new_buffer(ByteArray newBuffer) {
    m_buffer = newBuffer;
}

int32_t BufferedIndexInput::get_buffer_size() {
    return m_bufferSize;
}

void BufferedIndexInput::read_bytes(uint8_t* b, int32_t offset, int32_t length) {
    read_bytes_ex(b, offset, length);
}

void BufferedIndexInput::read_bytes_ex(uint8_t* b, int32_t offset, int32_t length) {
    if (length <= (m_bufferLength - m_bufferPosition)) {
        // the buffer contains enough data to satisfy this request
        if (length > 0) {
            MiscUtils::array_copy(m_buffer.get(), m_bufferPosition, b, offset, length);
            m_bufferPosition += length;
        }
    } else {
        // the buffer does not have enough data, first serve all we've got
        int32_t available = m_bufferLength - m_bufferPosition;
        if (available > 0) {
            MiscUtils::array_copy(m_buffer.get(), m_bufferPosition, b, offset, available);
            offset += available;
            length -= available;
            m_bufferPosition = m_bufferLength;
        }

        // and now, read the remaining 'length' bytes.
        if (length < m_bufferSize) {
            // if the amount left to read is small enough, do it in the usual
            // buffred way: fill the buffer and copy from it.
            refill();
            if (m_bufferLength < length) {
                // throw an exception when refill() could not read length
                // bytes.
                MiscUtils::array_copy(m_buffer.get(), 0, b, offset, m_bufferLength);
                throw IOException("Read past EOF");
            } else {
                MiscUtils::array_copy(m_buffer.get(), 0, b, offset, length);
                m_bufferPosition += length;
            }
        } else {
            // The amount left to read is larger than the buffer.
            int64_t after = m_bufferStart + m_bufferPosition + length;
            if (after > this->length()) {
                throw IOException("Read past EOF");
            }
            read_internal(b, offset, length);
            m_bufferStart = after;
            m_bufferPosition = 0;
            m_bufferLength = 0; // trigger refill on read
        }
    }
}

void BufferedIndexInput::refill() {
    int64_t start = m_bufferStart + m_bufferPosition;
    int64_t end = start + m_bufferSize;
    int64_t length = this->length();
    if (end > length) { // don't read past EOF
        end = length;
    }

    int32_t newLength = (int32_t)(end - start);
    if (newLength <= 0) {
        throw IOException("Read past EOF");
    }

    if (!m_buffer) {
        // allocate buffer lazily
        new_buffer(ByteArray::new_instance(m_bufferSize));
        seek_internal(start);
    }
    read_internal(m_buffer.get(), 0, newLength);
    m_bufferLength = newLength;
    m_bufferStart = start;
    m_bufferPosition = 0;
}

void BufferedIndexInput::close() {
    m_bufferStart = 0;
    m_bufferLength = 0;
    m_bufferPosition = 0;
}

int64_t BufferedIndexInput::get_file_pointer() {
    return m_bufferStart + m_bufferPosition;
}

void BufferedIndexInput::seek(int64_t pos) {
    if (pos >= m_bufferStart && pos < (m_bufferStart + m_bufferLength)) {
        m_bufferPosition = (int32_t)(pos - m_bufferStart);
        // seek within buffer
    } else {
        m_bufferStart = pos;
        m_bufferPosition = 0;
        m_bufferLength = 0; // trigger refill() on read()
        seek_internal(pos);
    }
}

// 这货有人用么？
LuceneObjectPtr BufferedIndexInput::clone(const LuceneObjectPtr& other) {
    BufferedIndexInputPtr cloneIndexInput(std::dynamic_pointer_cast<BufferedIndexInput>(IndexInput::clone(other)));
    cloneIndexInput->m_bufferSize = m_bufferSize;
    cloneIndexInput->m_buffer.reset();
    cloneIndexInput->m_bufferLength = 0;
    cloneIndexInput->m_bufferPosition = 0;
    cloneIndexInput->m_bufferStart = get_file_pointer();
    return cloneIndexInput;
}

} // namespace Lucene

