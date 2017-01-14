#ifndef LUCENE_THREAD_H
#define LUCENE_THREAD_H

#include <thread>
#include "lucene_object.h"

namespace Lucene {

typedef std::shared_ptr<std::thread> ThreadPtr;

class LuceneThread : public LuceneObject {
public:
    LuceneThread();
    virtual ~LuceneThread();

    LUCENE_CLASS(LuceneThread);

protected:
    ThreadPtr m_thread;

    /// Flag to indicate running thread.
    bool m_running;

public:
    /// start thread.
    virtual void start();

    /// return whether thread is currently running.
    virtual bool is_alive();

    /// wait for thread to finish.
    virtual void join();

    /// causes the currently running thread object to temporarily pause and
    /// allow other threads to execute.
    /// Note: C++11 only have a std::this_thread::yield().
    /// virtual void yield();

    /// override to provide the body of the thread.
    virtual void run() = 0;

    /// return representation of current execution thread.
    static int64_t current_id();

    /// Suspend current execution thread for a given time.
    static void thread_sleep(int32_t time);

    /// Yield current execution thread.
    static void thread_yield();

protected:
    /// set thread running state.
    void set_running(bool running);

    /// return thread running state.
    bool is_running();

    /// function that controls the lifetime of the running thread.
    static void run_thread(LuceneThread* thread);
};

} // namespace Lucene

#endif // LUCENE_THREAD_H
