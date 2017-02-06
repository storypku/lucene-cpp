#include <cstdlib>
#include <iostream>
#include <random>

#include "update_thread.h"

namespace Lucene {

UpdateThread::UpdateThread() : _msg_id_hitherto(0) {
    srand(time(NULL));
}

UpdateThread::~UpdateThread() {
}

void UpdateThread::set_msg_id(uint64_t msgid) {
    _msg_id_hitherto = msgid;
}

uint64_t UpdateThread::get_msg_id() {
    return _msg_id_hitherto;
}

void UpdateThread::start() {
    LuceneThread::start();
}

bool UpdateThread::has_next() {
    int32_t randint = rand() % 100 + rand() % 100;

    if ((uint64_t)randint > _msg_id_hitherto) {
        _msg_id_hitherto = (uint64_t)randint;
        return true;
    }
    return false;
}

uint64_t UpdateThread::next() {
    return _msg_id_hitherto;
}

void UpdateThread::run() {
    while (m_running) {
        if (has_next()) {
            uint64_t msg_id = next();
            // process the message
            std::cout << "process  msg: " << msg_id << "\n";
            // MemoryIndex
            LuceneThread::thread_sleep(1000);
        } else {
            LuceneThread::thread_sleep(3000);
        }
    }
}

void UpdateThread::stop() {
    if (this->is_alive()) {
        this->set_running(false);
        this->join();
    }
}

UpdateThreadPtr UpdateThread::get_instance() {
    static UpdateThreadPtr update_thread;
    if (!update_thread) {
        update_thread = new_lucene<UpdateThread>();
    }

    // if (!update_thread->is_alive()) {
    //     update_thread->start();
    // }
    return update_thread;
}

} // namespace Lucene
