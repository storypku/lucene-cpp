#include "merge_thread.h"
#include "segment_manager.h"

namespace Lucene {

const int32_t MergeThread::MERGE_THRESHOLD = 3;

MergeThread::MergeThread() {
}

MergeThread::~MergeThread() {
}

void MergeThread::start() {
    LuceneThread::start();
}

void MergeThread::run() {
    while (m_running) {
        std::vector<String> snapshot = SegmentManager::get_instance()->segments_at_work();
        int32_t num_segs = snapshot.size();
        if (num_segs < MERGE_THRESHOLD) {
            LuceneThread::thread_sleep(5000); // 5 secs
            continue;
        }

        std::sort(snapshot.begin(), snapshot.end(), [](const String& s1, const String& s2) {
                    return std::stoi(s1) <= std::stoi(s2);
                });
        String snew = SegmentManager::get_instance()->next_segment_name();

        const String& s1 = snapshot[0];
        const String& s2 = snapshot[1];

        std::cout << "seg1: " << s1 << ", seg2: " << s2 << ", snew: " << snew << "\n";

        // OneMerge(snapshot[0], snapshot[1], snew);

        SegmentManager::get_instance()->on_merge_done(s1, s2, snew);
        LuceneThread::thread_sleep(2000);
    }
}

void MergeThread::stop() {
    if (this->is_alive()) {
        this->set_running(false);
        this->join();
    }
}

MergeThreadPtr MergeThread::get_instance() {
    static MergeThreadPtr merge_thread;
    if (!merge_thread) {
        merge_thread = new_lucene<MergeThread>();
    }
    // if (!merge_thread->is_alive()) {
    //    merge_thread->start();
    // }
    return merge_thread;
}

};
