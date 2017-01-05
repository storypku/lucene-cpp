#include "reader.h"
#include "lucene.h"

namespace Lucene {

const int32_t Reader::READER_EOF = -1;

Reader::Reader() {
}

Reader::~Reader() {
}

int32_t Reader::read() {
    char buffer;
    return read(&buffer, 0, 1) == READER_EOF ? READER_EOF : buffer;
}

int64_t Reader::skip(UNUSED int64_t n) {
    return 0; // override
}

bool Reader::mark_supported() {
    return false; // override
}

void Reader::mark(UNUSED int32_t readAheadLimit) {
    // override
}

void Reader::reset() {
    // override
}

int64_t Reader::length() {
    return 0; // override
}

} // namespace Lucene
