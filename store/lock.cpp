#include "lucene.h"
#include "lock.h"
#include "lucene_thread.h"

namespace Lucene {
const int32_t Lock::LOCK_OBTAIN_WAIT_FOREVER = -1;
const int32_t Lock::LOCK_POLL_INTERVAL = 1000;

Lock::~Lock() {
}

bool Lock::obtain(int32_t lockWaitTimeout) {
    bool locked = obtain();
    int32_t maxSleepCount = lockWaitTimeout / LOCK_POLL_INTERVAL;
    int32_t sleepCount = 0;

    while (!locked) {
        if (lockWaitTimeout != LOCK_OBTAIN_WAIT_FOREVER && sleepCount++ >= maxSleepCount) {
            throw LockObtainFailedException("Lock obtain timed out");
        }

        LuceneThread::thread_sleep(LOCK_POLL_INTERVAL);
        locked = obtain();
    }

    return locked;
}

} // namespace Lucene
