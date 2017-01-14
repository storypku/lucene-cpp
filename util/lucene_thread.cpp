#include "lucene_thread.h"
#include "lucene_factory.h"
#include "synchronize.h"

namespace Lucene {

LuceneThread::LuceneThread() : m_running(false) {
}

LuceneThread::~LuceneThread() {
}

void LuceneThread::start() {
    set_running(false);
    m_thread = new_instance<std::thread>(LuceneThread::run_thread, this);
    set_running(true);
}

void LuceneThread::run_thread(LuceneThread* thread) {
    LuceneThreadPtr thread_object(thread->shared_from_this());

    try {
        thread_object->run();
    } catch (...) {
    }

    thread_object->set_running(false);
    thread_object.reset();
}

void LuceneThread::set_running(bool running) {
    SyncLock sync_lock(this);
    m_running = running;
}

bool LuceneThread::is_running() {
    SyncLock sync_lock(this);
    return m_running;
}

bool LuceneThread::is_alive() {
    return m_thread && is_running();
}

// void LuceneThread::yield() {
//    if (m_thread) {
//        m_thread->yield();
//    }
// }

void LuceneThread::join() {
    if (is_alive() && m_thread->joinable()) {
        m_thread->join();
    }
}

int64_t LuceneThread::current_id() {
    return (int64_t)pthread_self();
}

void LuceneThread::thread_sleep(int32_t time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void LuceneThread::thread_yield() {
    std::this_thread::yield();
}

} // namespace Lucene
