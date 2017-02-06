#ifndef MERGE_THREAD_H
#define MERGE_THREAD_H

#include "lucene_thread.h"

namespace Lucene {
class MergeThread;
typedef std::shared_ptr<MergeThread> MergeThreadPtr;

class MergeThread: public LuceneThread {
public:
    MergeThread();
    virtual ~MergeThread();

public:
    virtual void start();

    virtual void run();

    virtual void stop();

public:
    static MergeThreadPtr get_instance();
};

} // namespace Lucene
#endif // MERGE_THREAD_H
