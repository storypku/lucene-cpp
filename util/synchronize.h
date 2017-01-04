#ifndef SYNCHRONIZE_H
#define SYNCHRONIZE_H

#include <mutex>
#include "lucene.h"

namespace Lucene {

/// Utility class to support locking via a mutex.
class Synchronize {
public:
    Synchronize();
    virtual ~Synchronize();

protected:
    std::recursive_timed_mutex m_mutexSynchronize;
    int64_t m_lockThread;
    int32_t m_recursionCount;

public:
    /// Create a new Synchronize instance atomically.
    static void create_sync(SynchronizePtr& sync);

    /// Lock mutex using an optional timeout.
    bool lock(int32_t timeout = 0);

    /// Unlock mutex.
    void unlock();

    /// Unlock all recursive mutex.
    int32_t unlock_all();

    /// Returns true if mutex is currently locked by current thread.
    bool holds_lock();
};

/// Utility class to support scope locking.
class SyncLock {
public:
    SyncLock(const SynchronizePtr& sync, int32_t timeout = 0);

    template <class OBJECT>
    SyncLock(OBJECT object, int32_t timeout = 0) {
        this->m_sync = object->get_sync();
        lock(timeout);
    }

    virtual ~SyncLock();

protected:
    SynchronizePtr m_sync;

protected:
    void lock(int32_t timeout);
};

} // namespace Synchronize

#endif // SYNCHRONIZE_H
