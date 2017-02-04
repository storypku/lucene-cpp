#include "segment_manager.h"
#include "index_input.h"
#include "index_output.h"
#include "file_utils.h"

namespace Lucene {

const String& SegmentManager::SEGMENTS_META = "segments.meta";

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

int SegmentManager::init_from_file(const String& name) {
    return 0;
}

} // namespace Lucene

