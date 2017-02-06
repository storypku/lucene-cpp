#ifndef UPDATE_THREAD_H
#define UPDATE_THREAD_H

#include "lucene_thread.h"

namespace Lucene {

class UpdateThread;
typedef std::shared_ptr<UpdateThread> UpdateThreadPtr;

class UpdateThread : public LuceneThread {
public:
    UpdateThread();
    virtual ~UpdateThread();

    virtual void start();
    virtual void run();
    virtual void stop();

protected:
    // mock real scenario
    bool has_next();
    uint64_t next();

protected:
    uint64_t _msg_id_hitherto;

public:
    void set_msg_id(uint64_t msgid);
    uint64_t get_msg_id();

public:
    // TODO: thread_group
    static UpdateThreadPtr get_instance();
};

} // namespace Lucene
#endif // UPDATE_THREAD_H
