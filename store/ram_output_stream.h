#ifndef RAM_OUTPUT_STREAM_H
#define RAM_OUTPUT_STREAM_H

#include "index_output.h"
#include "ram_file.h"

namespace Lucene {

class RAMOutputStream : public IndexOutput {
public:
    RAMOutputStream(const RAMFilePtr& f) {}
};

} // namespace Lucene
#endif // RAM_OUTPUT_STREAM_H
