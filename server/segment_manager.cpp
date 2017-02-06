#include "segment_manager.h"
#include "index_input.h"
#include "index_output.h"
#include "file_utils.h"
#include "mmap_directory.h"

namespace Lucene {

const String& SegmentManager::SEGMENTS_META = "segments.meta";
const int32_t SegmentManager::MERGE_THRESHOLD = 3;

SegmentManager::SegmentManager()
    : _seg_num_max(0)
    , _msg_id_max(0) {
}

SegmentManager::~SegmentManager() {
}

SegmentManagerPtr SegmentManager::get_instance() {
    static SegmentManagerPtr instance;
    if (!instance) {
        instance.reset(new SegmentManager());
    }
    return instance;
}

int SegmentManager::init_from_file(const String& dir) {
    try {
        MMapIndexInput input(FileUtils::join_path(dir, SEGMENTS_META));
        int32_t count = input.read_int();

        // TODO: mutex protect
        for (int32_t i = 0; i < count; ++i) {
            String s = input.read_string();
            int32_t segnum = std::stoi(s);
            _seg_num_max = std::max(segnum, _seg_num_max);
            _segments.push_back(s);
        }

        input.close();
        return 0;
    } catch (...) {
        _segments.clear();
        return -1;
    }
}

std::vector<String> SegmentManager::segments_at_work() {
    std::vector<String> result;
    boost::shared_lock<boost::shared_mutex> lock(_segments_mutex);
    for (auto iter = _segments.begin(); iter != _segments.end(); ++iter) {
        result.push_back(*iter);
    }
    return result;
}

String SegmentManager::next_segment_name() {
    int32_t segnum = 0;
    {
        std::lock_guard<std::mutex> lock(_unique_name_mutex);
        segnum = ++_seg_num_max;
    }
    return std::to_string(segnum);
}

void SegmentManager::on_merge_done(const String& s1, const String& s2, const String& snew) {
    boost::unique_lock<boost::shared_mutex> lock(_segments_mutex);
    _segments.erase(std::find(_segments.begin(), _segments.end(), s1));
    _segments.erase(std::find(_segments.begin(), _segments.end(), s2));
    _segments.push_back(snew);
}

void SegmentManager::merge() {
    // TODO signal handler
    while (true) {
        std::vector<String> snapshot = segments_at_work();
        int32_t num_segs = snapshot.size();
        if (num_segs < MERGE_THRESHOLD) {
            LuceneThread::thread_sleep(3000);
        } else {
            std::sort(snapshot.begin(), snapshot.end(), [](const String& s1, const String& s2) {
                return std::stoi(s1) <= std::stoi(s2);
            });
            String seg1 = snapshot[0];
            String seg2 = snapshot[1];
            String newborn = next_segment_name();
            std::cout << "seg1: " << seg1 << ", seg2: " << seg2 << ", merged: " << newborn << "\n";
            LuceneThread::thread_sleep(6000);
        }
    }
}

} // namespace Lucene

