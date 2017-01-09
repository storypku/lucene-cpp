#ifndef LOCK_H
#define LOCK_H

#include "lucene_object.h"

namespace Lucene {

/// An interprocess mutex lock.
class Lock : public LuceneObject {
public:
    virtual ~Lock();
    LUCENE_CLASS(Lock);

public:
    /// How long #obtain(int64_t) waits, in milliseconds, in between attempts
    //to acquire the lock.
    static const int32_t LOCK_POLL_INTERVAL;

    /// Pass this value to #obtain(int64_t) to try forever to obtain the lock.
    static const int32_t LOCK_OBTAIN_WAIT_FOREVER;

public:
    /// Attempts to obtain exclusive access and immediately return upon success
    // or failure.
    /// @return true if exclusive access is obtained.
    virtual bool obtain() = 0;

    /// Releases exclusive access.
    virtual void release() = 0;

    /// Returns true if the resource is currently locked.
    virtual bool is_locked() = 0;

    /// Attempts to obtain an exclusive lock within amount of time given. Polls
    // once per LOCK_POLL_INTERVAL(currently 1000) milliseconds until
    // lockWaitTimeout is passed.
    // @param lockWaitTimeout length of time to wait in milliseconds or
    // #LOCK_OBTAIN_WAIT_FOREVER to retry forever.
    // @return true if lock was obtained.
    bool obtain(int32_t lockWaitTimeout);
};

} // namespace Lucene
#endif // LOCK_H
