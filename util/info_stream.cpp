#include <iostream>
#include "lucene.h"
#include "info_stream.h"

namespace Lucene {

InfoStream::InfoStream() {
}

InfoStream::~InfoStream() {
}

InfoStreamFile::InfoStreamFile(const String& path)
    : m_file(path) {
}

InfoStreamFile::~InfoStreamFile() {
    // Added by storypku
    m_file.close();
}

InfoStreamFile& InfoStreamFile::operator<< (const String& t) {
    m_file << t;
    return *this;
}

InfoStreamOut::~InfoStreamOut() {
}

InfoStreamOut& InfoStreamOut::operator<< (const String& t) {
    std::cout << t;
    return *this;
}

InfoStreamNull::~InfoStreamNull() {
}

InfoStreamNull& InfoStreamNull::operator<< (UNUSED const String& t) {
    return *this;
}

} // namespace Lucene

