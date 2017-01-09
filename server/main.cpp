#include "lucene.h"
#include "simple_fs_lock_factory.h"
#include "info_stream.h"
#include "file_utils.h"
#include <iostream>

using namespace Lucene;

int main(UNUSED int argc, UNUSED char *argv[]) {
    std::cout << "Hello lucene\n";
    SimpleFSLockFactory simpLockFactory("indexDir");
    LockPtr lock = simpLockFactory.make_lock("write.lock");
    std::cout << lock->obtain() << "\n";
    std::cout << lock->to_string() << "\n";
    // lock->release();
    simpLockFactory.clear_lock("write.lock");
    return 0;
}
