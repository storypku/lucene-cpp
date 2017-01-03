#include "lucene.h"
#include <boost/thread/mutex.hpp>
#include "synchronize.h"
#include "lucene_thread.h"

namespace Lucene {

Synchronize::Synchronize() : m_lockThread(0), m_recursionCount(0) {
}

void Synchronize::create_sync(SynchronizePtr& sync) {
    static boost::mutex lockMutex;
    boost::mutex::scoped_lock syncLock(lockMutex);
    if (!sync) {
        sync = new_instance<
    }
}

} // namespace Lucene

