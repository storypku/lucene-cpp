#include "segment_manager.h"
#include "index_input.h"
#include "index_output.h"
#include "file_utils.h"

namespace Lucene {

SegmentManager::SegmentManager()
    : _segnum_maximum(0)
    , _msg_id_maximum(0) {
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

int load_from_file(const String& dir, const String& name) {
    _filepath = FileUtils::join_path(dir, name);
    return 0;
}

} // namespace Lucene

