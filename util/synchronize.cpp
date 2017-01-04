#include "lucene.h"
#include <boost/thread/mutex.hpp>
#include "synchronize.h"
#include "lucene_thread.h"

namespace Lucene {

Synchronize::Synchronize() : m_lockThread(0), m_recursionCount(0) {
}

Synchronize::~Synchronize() {
}

void Synchronize::create_sync(SynchronizePtr& sync) {
    static boost::mutex lockMutex;
    boost::mutex::scoped_lock syncLock(lockMutex);
    if (!sync) {
        sync = new_instance<Synchronize>();
    }
}

void Synchronize::lock(int32_t timeout) {
    if (timeout > 0) {
        m_mutexSynchronize.timed_lock(boost::posix_time::milliseconds(timeout));
    } else {
        m_mutexSynchronize.lock();
    }
    m_lockThread = LuceneThread::current_id();
    ++m_recursionCount;
}

} // namespace Lucene

