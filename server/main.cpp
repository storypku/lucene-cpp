#include "lucene.h"
#include "misc_utils.h"
#include "simple_fs_lock_factory.h"
#include "directory.h"
#include "info_stream.h"
#include "ram_output_stream.h"
#include "ram_directory.h"
#include "file_utils.h"
#include <iostream>
#include <time.h>

using namespace Lucene;

int main(UNUSED int argc, UNUSED char *argv[]) {
    std::cout << "Hello lucene\n";
    RAMDirectory directory;
    IndexOutputPtr out = directory.create_output("hello");

    out->write_byte('a');
    std::cout << out->length() << "\n";
    std::cout << directory.to_string() << "\n";
    std::cout << MiscUtils::current_time_millis() << " " << time(NULL) << "\n";

    return 0;
}
