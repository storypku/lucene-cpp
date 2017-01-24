#ifndef SEGMENT_MANAGER_H
#define SEGMENT_MANAGER_H

#include "lucene.h"
#include <boost/thread/shared_mutex.hpp>

namespace Lucene {

class SegmentManager;
typedef std::shared_ptr<SegmentManager> SegmentManagerPtr;

class SegmentManager : public LuceneObject {
protected:
    SegmentManager();

public:
    virtual ~SegmentManager();
    static SegmentManagerPtr get_instance();

protected:
    std::vector<String> _segments;
    boost::shared_mutex _crud_mutex;

    std::mutex _name_mutex;

    int32_t _segnum_maximum;
    int32_t _msg_id_maximum;

    String _filepath;

public:
    int load_from_file(const String& dir, const String& name);
    int save_to_file();

    void read_from(IndexInputPtr input);
    void write_to(IndexOutputPtr output);

    String next_avail_name() {
        int32_t segnum = 0;
        {
            std::lock_guard<std::mutex> lock(_name_mutex);
            segnum = ++_segnum_maximum;
        }
        std::cout << "[Retrieval] " << segnum << "\n";
        return std::to_string(segnum);
    }

    void add_segment(const String& segment) {
        std::unique_lock<boost::shared_mutex> lock(_crud_mutex);
        _segments.push_back(segment);
    }

    void on_merge(const String& segnew, const String& seg1, const String& seg2) {
        boost::unique_lock<boost::shared_mutex> lock(_crud_mutex);

        _segments.erase(std::find(_segments.begin(), _segments.end(), seg1));
        _segments.erase(std::find(_segments.begin(), _segments.end(), seg2));

        _segments.push_back(segnew);
    }

    void get_segments(std::vector<String>& result) {
        boost::shared_lock<boost::shared_mutex> lock(_crud_mutex);
        for (const auto& i : _segments) {
            result.push_back(i);
        }
    }
};

} // namespace Lucene

#endif // SEGMENT_MANAGER_H
