#include "lucene.h"
#include "misc_utils.h"
#include "simple_fs_lock_factory.h"
#include "directory.h"
#include "index_input.h"
#include "info_stream.h"
#include "ram_output_stream.h"
#include "ram_directory.h"
#include "file_utils.h"
#include <iostream>
#include <time.h>

using namespace Lucene;

int main(UNUSED int argc, UNUSED char* argv[]) {
    std::cout << "Hello lucene\n";
    RAMDirectoryPtr directory(new_instance<RAMDirectory>());
    IndexOutputPtr out = directory->create_output("hello");
    out->write_byte('a');
    std::cout << directory->to_string() << "\n";
    IndexInputPtr in = directory->open_input("hello");
    uint8_t byte = in->read_byte();
    std::cout << byte << "\n";
    std::cout << MiscUtils::current_time_millis() << " " << time(NULL) << "\n";

    return 0;
}
