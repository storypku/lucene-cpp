#include "lucene.h"
#include "index_output.h"
#include "index_input.h"
#include "misc_utils.h"

namespace Lucene {

const int32_t IndexOutput::COPY_BUFFER_SIZE = 16384;

void IndexOutput::write_bytes(const uint8_t* b, int32_t length) {
    write_bytes(b, 0, length);
}

void IndexOutput::write_int(int32_t i) {
    write_byte((uint8_t)(i >> 24));
    write_byte((uint8_t)(i >> 16));
    write_byte((uint8_t)(i >> 8));
    write_byte((uint8_t)i);
}

void IndexOutput::write_vint(int32_t i) {
    while ((i & ~0x7f) != 0) {
        write_byte((uint8_t)((i & 0x7f) | 0x80));
        i = MiscUtils::unsigned_shift(i, 7);
    }

    write_byte((uint8_t)i);
}

void IndexOutput::write_long(int64_t i) {
    write_int((int32_t)(i >> 32));
    write_int((int32_t)i);
}

void IndexOutput::write_vlong(int64_t i) {
    while ((i & ~0x7f) != 0) {
        write_byte((uint8_t)((i & 0x7f) | 0x80));
        i |= MiscUtils::unsigned_shift(i, (int64_t)7);
    }

    write_byte((uint8_t)i);
}

void IndexOutput::write_string(const String& s) {
    int32_t length = s.size();
    write_vint(length);
    write_bytes((const uint8_t*)s.c_str(), length);
}

void IndexOutput::copy_bytes(const IndexInputPtr& input, int64_t numBytes) {
    BOOST_ASSERT(numBytes >= 0);
    int64_t left = numBytes;

    if (!m_copyBuffer) {
        m_copyBuffer = ByteArray::new_instance(COPY_BUFFER_SIZE);
    }

    while (left > 0) {
        int32_t toCopy = left > COPY_BUFFER_SIZE ? COPY_BUFFER_SIZE : (int32_t)left;
        input->read_bytes(m_copyBuffer.get(), 0, toCopy);
        write_bytes(m_copyBuffer.get(), 0, toCopy);
        left -= toCopy;
    }
}

void IndexOutput::set_length(UNUSED int64_t length) {
}

void IndexOutput::write_string_string_map(MapStringString map) {
    if (!map) {
        write_int(0);
    } else {
        write_int(map.size());

        for (auto entry = map.begin(); entry != map.end(); ++entry) {
            write_string(entry->first);
            write_string(entry->second);
        }
    }
}

} // namespace Lucene

