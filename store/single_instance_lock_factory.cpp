#include "lucene.h"
#include "single_instance_lock_factory.h"

namespace Lucene {

SingleInstanceLockFactory::SingleInstanceLockFactory()
    : m_locks(HashSet<String>::new_instance()) {
}

LockPtr SingleInstanceLockFactory::make_lock(const String& lockName) {
    // We do not use the LockPrefix at all, because the private HashSet instance
    // effectively scopes the locking to this single Directory instance.
    return new_lucene<SingleInstanceLock>(m_locks, lockName);
}

void SingleInstanceLockFactory::clear_lock(const String& lockName) {
    SyncLock syncLock(&m_locks);
    m_locks.remove(lockName);
}


SingleInstanceLock::SingleInstanceLock(HashSet<String> locks, const String& lockName)
    : m_locks(locks), m_lockName(lockName) {

}

SingleInstanceLock::~SingleInstanceLock() {
}

bool SingleInstanceLock::obtain() {
    SyncLock syncLock(&m_locks);
    return m_locks.add(m_lockName);
}

void SingleInstanceLock::release() {
    SyncLock syncLock(&m_locks);
    m_locks.remove(m_lockName);
}

bool SingleInstanceLock::is_locked() {
    SyncLock syncLock(&m_locks);
    return m_locks.contains(m_lockName);
}

String SingleInstanceLock::to_string() {
    return m_lockName;
}

} // namespace Lucene

