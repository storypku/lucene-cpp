#include "segment_manager.h"
#include "simple_fs_directory.h"
#include <iostream>

using namespace Lucene;

int main(int argc, char *argv[]) {
    SimpleFSDirectory dir("index_data");
    IndexOutputPtr output = dir.create_output("segments.gen");
    output->write_int(3);
    output->write_string("1");
    output->write_string("13");
    output->write_string("135");
    output->close();

    IndexInputPtr input = dir.open_input("segments.gen");
    int32_t count = input->read_int();
    std::vector<String> segments;
    for (int32_t i = 0; i < count; ++i) {
        String s = input->read_string();
        segments.push_back(s);
    }

    std::cout << "Count: " << count << "\n";
    for (const auto& s : segments) {
        std::cout << s << "\t";
    }
    std::cout << "\n";
    return 0;
}
