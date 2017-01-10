#ifndef BUFFERED_INDEX_OUTPUT_H
#define BUFFERED_INDEX_OUTPUT_H

#include "index_output.h"
namespace Lucene {

class BufferedIndexOutput : public LuceneObject {
public:
    static const int32_t BUFFER_SIZE;
};

}
#endif // BUFFERED_INDEX_OUTPUT_H
