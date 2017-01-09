#include "lucene.h"
#include "no_lock_factory.h"

namespace Lucene {

NoLockFactoryPtr NoLockFactory::get_no_lock_factory() {
    static NoLockFactoryPtr singleton;
    if (!singleton) {
        singleton = new_lucene<NoLockFactory>();
    }
    return singleton;
}

NoLockPtr NoLockFactory::get_singleton_lock() {
    // Single instance returned whenever make_lock is called.
    static NoLockPtr singletonLock;
    if (!singletonLock) {
        singletonLock = new_lucene<NoLock>();
    }

    return singletonLock;
}

LockPtr NoLockFactory::make_lock(UNUSED const String& lockName) {
    return get_singleton_lock();
}

void NoLockFactory::clear_lock(UNUSED const String& lockName) {
    // Do nothing.
}

NoLock::~NoLock() {
}

bool NoLock::obtain() {
    return true;
}

void NoLock::release() {
}

bool NoLock::is_locked() {
    return false;
}

String NoLock::to_string() {
    return get_class_name();
}

} // namespace Lucene
