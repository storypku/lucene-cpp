#ifndef NO_LOCK_FACTORY_H
#define NO_LOCK_FACTORY_H

#include "lock.h"
#include "lock_factory.h"

namespace Lucene {

/// Use this LockFactory to disable locking entirely. Only one instance of this
/// lock is created.
/// You should call #get_no_lock_factory() to get the instance.
class NoLockFactory : public LockFactory {
public:
    virtual ~NoLockFactory() = default;

    LUCENE_CLASS(NoLockFactory);

private:
    static NoLockPtr get_singleton_lock();

public:
    static NoLockFactoryPtr get_no_lock_factory();

public:
    virtual LockPtr make_lock(const String& lockName);

    virtual void clear_lock(const String& lockName);
};

class NoLock : public Lock {
public:
    virtual ~NoLock();

    LUCENE_CLASS(NoLock);

public:
    virtual bool obtain();
    virtual void release();
    virtual bool is_locked();
    virtual String to_string();
};

} // namespace Lucene

#endif // NO_LOCK_FACTORY_H
