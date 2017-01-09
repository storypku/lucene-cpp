#ifndef LOCK_FACTORY_H
#define LOCK_FACTORY_H

#include "lucene_object.h"

namespace Lucene {

/// Base class for Locking implementation.  Directory uses
/// instances of this class to implement locking.
class LockFactory : public LuceneObject {
public:
    virtual ~LockFactory();

    LUCENE_CLASS(LockFactory);

protected:
    String m_lockPrefix;

public:
    /// Set the prefix in use for all locks created in this LockFactory.  This is normally called once, when a
    /// Directory gets this LockFactory instance.
    virtual void set_lock_prefix(const String& lockPrefix);

    /// Get the prefix in use for all locks created in this LockFactory.
    virtual String get_lock_prefix();

    /// Return a new Lock instance identified by lockName.
    /// @param lockName name of the lock to be created.
    virtual LockPtr make_lock(const String& lockName) = 0;

    /// Attempt to clear (forcefully unlock and remove) the specified lock.  Only call this at a time when you
    /// are certain this lock is no longer in use.
    /// @param lockName name of the lock to be cleared.
    virtual void clear_lock(const String& lockName) = 0;
};

}

#endif
