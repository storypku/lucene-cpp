#include "merge_thread.h"
#include "segment_manager.h"

namespace Lucene {

MergeThread::MergeThread() {
}

MergeThread::~MergeThread() {
}

void MergeThread::start() {
    LuceneThread::start();
}

void MergeThread::run() {
    while (
}

void MergeThread::stop() {
    if (this->is_alive()) {
        this->join();
    }
}

MergeThreadPtr MergeThread::get_instance() {
    static MergeThreadPtr merge_thread;
    if (!merge_thread) {
        merge_thread = new_lucene<MergeThread>();
    }
    if (!merge_thread->is_alive()) {
        merge_thread->start();
    }
    return merge_thread;
}

};
