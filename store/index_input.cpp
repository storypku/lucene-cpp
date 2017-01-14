#include "lucene.h"
#include "index_input.h"

namespace Lucene {

int32_t IndexInput::read_int() {
    int32_t i = (read_byte() & 0xff) << 24;
    i |= (read_byte() & 0xff) << 16;
    i |= (read_byte() & 0xff) << 8;
    i |= (read_byte() & 0xff);
    return i;
}

int32_t IndexInput::read_vint() {
    uint8_t b = read_byte();
    int32_t i = (b & 0x7f);

    for (int32_t shift = 7; (b & 0x80) != 0; shift += 7) {
        b = read_byte();
        i |= (b & 0x7f) << shift;
    }

    return i;
}

int64_t IndexInput::read_long() {
    int64_t i = (int64_t)read_int() << 32;
    i |= (read_int() & 0xffffffffLL);
    return i;
}

int64_t IndexInput::read_vlong() {
    uint8_t b = read_byte();
    int64_t i = (b & 0x7f);

    for (int32_t shift = 7; (b & 0x80) != 0; shift += 7) {
        b = read_byte();
        i |= (int64_t)(b & 0x7f) << shift;
    }

    return i;
}

String IndexInput::read_string() {
    int32_t  length = read_vint();
    ByteArray bytes(ByteArray::new_instance(length));
    read_bytes(bytes.get(), 0, length);

    // 目前不管unicode的问题
    return String((const char*)bytes.get(), length);
}

MapStringString IndexInput::read_string_string_map() {
    MapStringString map(MapStringString::new_instance());
    int32_t count = read_int();

    for (int32_t i = 0; i < count; ++i) {
        String key(read_string());
        String val(read_string());

        map[key] = val;
    }

    return map;
}

LuceneObjectPtr IndexInput::clone(const LuceneObjectPtr& other) {
    IndexInputPtr cloneIndexInput(std::dynamic_pointer_cast<IndexInput>(LuceneObject::clone(other)));
    return cloneIndexInput;
}

} // namespace Lucene
