#include "segment_manager.h"
#include "simple_fs_directory.h"
#include "merge_thread.h"
#include "update_thread.h"

#include <iostream>

using namespace Lucene;

int main() {
    SimpleFSDirectory fsd("index_data");
    IndexOutputPtr output = fsd.create_output("segments.meta");
    output->write_long(132);
    output->write_int(4);
    output->write_string("9");
    output->write_string("13");
    output->write_string("20");
    output->write_string("31");
    output->close();

    int result = SegmentManager::get_instance()->init_from_file("index_data");
    std::cout << result << std::endl;
    std::vector<String> segs = SegmentManager::get_instance()->segments_at_work();
    for (auto i : segs) {
        std::cout << i << std::endl;
    }

    MergeThread::get_instance()->start();
    UpdateThread::get_instance()->start();
    sleep(30);
    UpdateThread::get_instance()->stop();
    MergeThread::get_instance()->stop();

    SegmentManager::get_instance()->save_to_file();

    return 0;
}
