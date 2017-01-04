#include "lucene_sync.h"
#include "synchronize.h"
#include "lucene_signal.h"

namespace Lucene {

LuceneSync::~LuceneSync() {
}

SynchronizePtr LuceneSync::get_sync() {
    Synchronize::create_sync(m_objectLock);
    return m_objectLock;
}

LuceneSignalPtr LuceneSync::get_signal() {
    LuceneSignal::create_signal(m_objectSignal, get_sync());
    return m_objectSignal;
}

void LuceneSync::lock() {
    this->get_sync()->lock();
}

void LuceneSync::unlock() {
    this->get_sync()->unlock();
}

bool LuceneSync::holds_lock() {
    return this->get_sync()->holds_lock();
}

void LuceneSync::wait(int32_t timeout) {
    this->get_signal()->wait(timeout);
}

void LuceneSync::notify_all() {
    this->get_signal()->notify_all();
}

} // namespace Lucene
