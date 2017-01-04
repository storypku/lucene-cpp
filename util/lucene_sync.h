#ifndef LUCENE_SYNC_H
#define LUCENE_SYNC_H

#include "lucene_types.h"

namespace Lucene {

/// Base class for all Lucene synchronize classes
class LuceneSync {
public:
    virtual ~LuceneSync();

protected:
    SynchronizePtr m_objectLock;
    LuceneSignalPtr m_objectSignal;

public:
    /// Return this object synchronize lock.
    virtual SynchronizePtr get_sync();

    /// Return this object signal.
    virtual LuceneSignalPtr get_signal();

    /// Lock this object.
    virtual void lock();

    /// Unlock this object.
    virtual void unlock();

    /// Returns true if this object is currently locked by current thread.
    virtual bool holds_lock();

    /// Wait for signal using an optional timeout.
    virtual void wait(int32_t timeout = 0);

    /// Notify all threads waiting for signal.
    virtual void notify_all();
};

} // namespace Lucene

#endif // LUCENE_SYNC_H
