#ifndef SEGMENT_MANAGER_H
#define SEGMENT_MANAGER_H

#include "lucene.h"
#include <boost/thread/shared_mutex.hpp>

namespace Lucene {

// Forward declaration.
class SegmentManager;
typedef std::shared_ptr<SegmentManager> SegmentManagerPtr;

class SegmentManager : public LuceneObject {
protected:
    SegmentManager();

public:
    virtual ~SegmentManager();
    static SegmentManagerPtr get_instance();

protected:
    std::vector<String> _segments;
    boost::shared_mutex _segments_mutex;

    std::mutex _unique_name_mutex;
    int32_t _seg_num_max;

    int32_t _msg_id_max;

    String _directory;
public:
    static const String& SEGMENTS_META;

public:
    int init_from_file(const String& path);
    // return current working segments
    std::vector<String> segments_at_work();
    String next_segment_name();

public:
    void on_merge_done(const String& s1, const String& s2, const String& snew);

    void add_segment(const String& s);
};

} // namespace Lucene
#endif // SEGMENT_MANAGER_H
