#include "lucene.h"
#include "file_utils.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Hello lucene\n";
    Lucene::FileUtils::create_directory("helloworld");
    return 0;
}
