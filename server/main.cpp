#include "lucene.h"
#include <memory>
#include <mutex>
#include <iostream>

namespace Lucene {

class SegmentManager;
typedef std::shared_ptr<SegmentManager> SegmentManagerPtr;

class SegmentManager : public LuceneObject {
public:
    static SegmentManagerPtr get_instance() {
        static SegmentManagerPtr instance;
        if (!instance) {
            instance.reset(new SegmentManager());
        }
        return instance;
    }

protected:
    std::mutex _crud_mutex;
    std::mutex _name_mutex;
    int32_t _segnum_maximum;
    std::vector<String> _segments;

public:
    SegmentManager() : _segnum_maximum(0) {
    }

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
        std::lock_guard<std::mutex> lock(_crud_mutex);
        _segments.push_back(segment);
    }

    void on_merge(const String& segnew, const String& seg1, const String& seg2) {
        std::lock_guard<std::mutex> lock(_crud_mutex);

        _segments.erase(std::find(_segments.begin(), _segments.end(), seg1));
        _segments.erase(std::find(_segments.begin(), _segments.end(), seg2));

        _segments.push_back(segnew);
    }

    void print() {
        for (const auto& i : _segments) {
            std::cout << i << "\n";
        }
    }
};


}

using namespace Lucene;
int main() {
    SegmentManagerPtr instance = SegmentManager::get_instance();
    instance->add_segment("1");
    instance->add_segment("2");
    instance->add_segment("3");
    instance->on_merge("4", "2", "3");
    instance->print();
}
