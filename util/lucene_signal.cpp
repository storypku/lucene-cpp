#include "lucene_factory.h"
#include "lucene_types.h"
#include "lucene_signal.h"
#include "synchronize.h"

namespace Lucene {

LuceneSignal::LuceneSignal(const SynchronizePtr& objectLock)
    : m_objectLock(objectLock) {
}

LuceneSignal::~LuceneSignal() {
}

void LuceneSignal::create_signal(LuceneSignalPtr& signal, const SynchronizePtr& objectLock) {
    static std::mutex lockMutex;
    std::lock_guard<std::mutex> syncLock(lockMutex);

    if (!signal) {
        signal = new_instance<LuceneSignal>(objectLock);
    }
}

void LuceneSignal::wait(int32_t timeout) {
    using Ms = std::chrono::milliseconds;

    int32_t relockCount = m_objectLock ? m_objectLock->unlock_all() : 0;
    std::unique_lock<std::mutex> waitLock(m_waitMutex);

    while (std::cv_status::timeout == m_signalCondition.wait_for(waitLock, Ms(timeout))) {
        if (timeout != 0 || std::cv_status::no_timeout == m_signalCondition.wait_for(waitLock, Ms(10))) {
            break;
        }
    }

    for (int32_t relock = 0; relock < relockCount; ++relock) {
        m_objectLock->lock();
    }
}

void LuceneSignal::notify_all() {
    // 此处的逻辑是难点。
    m_signalCondition.notify_all();
}

} // namespace Lucene
