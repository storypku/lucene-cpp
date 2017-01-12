#ifndef RAM_INPUT_STREAM_H
#define RAM_INPUT_STREAM_H

#include "index_input.h"
#include "ram_file.h"

namespace Lucene {

class RAMInputStream : public IndexInput {
public:
    RAMInputStream(const RAMFilePtr& f) {
    }
    virtual ~RAMInputStream() = default;
};

} // namespace Lucene

#endif // RAM_INPUT_STREAM_H
