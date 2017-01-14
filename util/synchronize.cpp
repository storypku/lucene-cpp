#include <mutex>
#include <chrono>
#include "lucene_factory.h"
#include "synchronize.h"
#include "lucene_thread.h"

namespace Lucene {

Synchronize::Synchronize() : m_lockThread(0), m_recursionCount(0) {
}

Synchronize::~Synchronize() {
}

void Synchronize::create_sync(SynchronizePtr& sync) {
    static std::mutex lockMutex;
    std::lock_guard<std::mutex> syncLock(lockMutex);

    if (!sync) {
        sync = new_instance<Synchronize>();
    }
}

bool Synchronize::lock(int32_t timeout) {
    if (timeout > 0) {
        bool success = m_mutexSynchronize.try_lock_for(std::chrono::milliseconds(timeout));

        if (!success) {
            return false;
        }
    } else {
        m_mutexSynchronize.lock();
    }

    m_lockThread = LuceneThread::current_id();
    ++m_recursionCount;
    return true;
}

void Synchronize::unlock() {
    if (m_recursionCount == 0) {
        return;
    } else if (--m_recursionCount == 0) {
        m_lockThread = 0;
    }

    m_mutexSynchronize.unlock();
}

int32_t Synchronize::unlock_all() {
    int32_t count = m_recursionCount;

    for (int32_t unlock = 0; unlock < count; ++unlock) {
        this->unlock();
    }

    return count;
}

bool Synchronize::holds_lock() {
    return (m_lockThread == LuceneThread::current_id() && m_recursionCount > 0);
}

SyncLock::SyncLock(const SynchronizePtr& sync, int32_t timeout)
    : m_sync(sync) {
    this->lock(timeout);
}

SyncLock::~SyncLock() {
    if (m_sync) {
        m_sync->unlock();
    }
}

void SyncLock::lock(int32_t timeout) {
    if (m_sync) {
        m_sync->lock(timeout); // 未检查返回值
    }
}

} // namespace Lucene

