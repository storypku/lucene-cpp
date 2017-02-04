#include "segment_manager.h"
#include <iostream>

using namespace Lucene;

int main() {
    int result = SegmentManager::get_instance()->init_from_file("index_data");
    std::cout << result << std::endl;
    std::vector<String> segs = SegmentManager::get_instance()->segments_at_work();
    for (auto i : segs) {
        std::cout << i << std::endl;
    }
    return 0;
}
