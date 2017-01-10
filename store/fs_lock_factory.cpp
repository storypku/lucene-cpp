#include "lucene.h"
#include "fs_lock_factory.h"

namespace Lucene {

void FSLockFactory::set_lock_dir(const String& lockDir) {
    if (!this->m_lockDir.empty()) {
        throw IllegalStateException("You can set the lock directory for this factory only once.");
    }

    this->m_lockDir = lockDir;
}

String FSLockFactory::get_lock_dir() {
    return m_lockDir;
}

} // namespace Lucene
