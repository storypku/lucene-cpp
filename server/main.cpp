#include "lucene.h"
#include "info_stream.h"
#include "file_utils.h"
#include <iostream>

using namespace Lucene;

int main(int argc, char *argv[]) {
    std::cout << "Hello lucene\n";
    InfoStreamFile of("lucene.log");
    of << "hello";
    return 0;
}
