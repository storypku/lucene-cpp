#include "segment_manager.h"
#include "index_input.h"
#include "index_output.h"
#include "file_utils.h"
#include "mmap_directory.h"
#include "update_thread.h"
#include "simple_fs_directory.h"

namespace Lucene {

const String& SegmentManager::SEGMENTS_META = "segments.meta";

SegmentManager::SegmentManager()
    : _seg_num_max(0) {
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

        uint64_t msg_id_hitherto = input.read_long();

        int32_t count = input.read_int();
        for (int32_t i = 0; i < count; ++i) { // CAUTION: no mutex protect here !
            String s = input.read_string();
            int32_t segnum = std::stoi(s);
            _seg_num_max = std::max(segnum, _seg_num_max);
            _segments.push_back(s);
        }

        _directory = dir; // UGLY...

        // 通知文档更新线程之前已处理到的消息号
        UpdateThread::get_instance()->set_msg_id(msg_id_hitherto);

        input.close();
        return 0;
    } catch (...) {
        _segments.clear();
        return -1;
    }
}

int SegmentManager::save_to_file() {

    try {
        String segmeta_orig = FileUtils::join_path(_directory, SEGMENTS_META);
        String segmeta_new = segmeta_orig + ".tmp";
        SimpleFSIndexOutput output(segmeta_new);

        uint64_t msg_id_hitherto = UpdateThread::get_instance()->get_msg_id();
        output.write_long(msg_id_hitherto);

        std::vector<String> snapshot = segments_at_work();
        int32_t count = snapshot.size();
        output.write_int(count);

        for (int32_t i = 0; i < count; ++i) {
            output.write_string(snapshot[i]);
        }
        output.close();

        // FileUtils::remove_file(segmeta_orig);
        bool copy_ok = FileUtils::copy_file(segmeta_new, segmeta_orig);
        FileUtils::remove_file(segmeta_new);
        return copy_ok ? 0 : -1;
    } catch (...) {
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

void SegmentManager::add_segment(const String& snew) {
    boost::unique_lock<boost::shared_mutex> lock(_segments_mutex);
    _segments.push_back(snew);
}

} // namespace Lucene

