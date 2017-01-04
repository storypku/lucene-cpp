#ifndef LUCENE_SIGNAL_H
#define LUCENE_SIGNAL_H

#include <condition_variable>
#include <mutex>
#include "lucene_types.h"

namespace Lucene {

/// Utility class to support signaling notifications.
class LuceneSignal {
public:
    LuceneSignal(const SynchronizePtr& objectLock = SynchronizePtr());

    virtual ~LuceneSignal();

protected:
    std::mutex m_waitMutex;
    std::condition_variable m_signalCondition;
    SynchronizePtr m_objectLock;

public:
    /// Create a new LuceneSignal instance atomically.
    static void create_signal(LuceneSignalPtr& signal, const SynchronizePtr& objectLock);

    /// Wait for signal using an optional timeout.
    void wait(int32_t timeout = 0);

    /// Notify all threads waiting for signal.
    void notify_all();
};

} // namespace Lucene

#endif // LUCENE_SIGNAL_H

