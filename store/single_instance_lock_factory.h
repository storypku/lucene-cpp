#ifndef SINGLE_INSTANCE_LOCK_FACTORY_H
#define SINGLE_INSTANCE_LOCK_FACTORY_H

#include "lock_factory.h"

namespace Lucene {

/// Implements LockFactory for a single in-process instance, meaning all
/// locking will take place through this one instance.  Only use this LockFactory
/// when you are certain all IndexReaders and IndexWriters for a given index are running
/// against a single shared in-process Directory instance.  This is currently the
/// default locking for RAMDirectory.
class SingleInstanceLockFactory : public LockFactory {
public:
    SingleInstanceLockFactory();
    virtual ~SingleInstanceLockFactory() = default;

    LUCENE_CLASS(SingleInstanceLockFactory);

protected:
    HashSet<String> m_locks;

public:
    /// Return a new Lock instance identified by lockName.
    /// @param lockName name of the lock to be created.
    virtual LockPtr make_lock(const String& lockName);

    /// Attempt to clear (forcefully unlock and remove) the
    /// specified lock.  Only call this at a time when you are
    /// certain this lock is no longer in use.
    /// @param lockName name of the lock to be cleared.
    virtual void clear_lock(const String& lockName);
};

} // namespace Lucene
#endif // SINGLE_INSTANCE_LOCK_FACTORY_H
