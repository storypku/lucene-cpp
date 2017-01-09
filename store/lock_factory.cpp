#include "lucene.h"
#include "lock_factory.h"

namespace Lucene {

LockFactory::~LockFactory() {
}

void LockFactory::set_lock_prefix(const String& lockPrefix) {
    this->m_lockPrefix = lockPrefix;
}

String LockFactory::get_lock_prefix() {
    return m_lockPrefix;
}

} // namespace Lucene
