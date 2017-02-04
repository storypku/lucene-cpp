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

            _segments.push_back(input.read_string());
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
            
        }
    }
}

} // namespace Lucene

