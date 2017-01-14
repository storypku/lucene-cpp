#include "lucene.h"
#include "single_instance_lock_factory.h"

namespace Lucene {

SingleInstanceLockFactory::SingleInstanceLockFactory()
    : m_locks(HashSet<String>::new_instance()) {
}

LockPtr SingleInstanceLockFactory::make_lock(const String& name) {
    // We do not use the LockPrefix at all, because the private HashSet instance
    // effectively scopes the locking to this single Directory instance.
    return LockPtr();
    // return new_lucene<SingleInstanceLock>(m_locks, lockName);
}

void SingleInstanceLockFactory::clear_lock(const String& name) {

}

} // namespace Lucene

