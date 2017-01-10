#include "misc_utils.h"

namespace Lucene {


uint64_t MiscUtils::get_time_millis(boost::posix_time::ptime time) {
    return boost::posix_time::time_duration(time - boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1))).total_milliseconds();
}

uint64_t MiscUtils::current_time_millis() {
    return get_time_millis(boost::posix_time::microsec_clock::universal_time());
}

int64_t MiscUtils::unsigned_shift(int64_t num, int64_t shift) {
    return (shift & 0x3f) == 0 ? num : (((uint64_t)num >> 1) & 0x7fffffffffffffffLL) >> ((shift & 0x3f) - 1);
}

int32_t MiscUtils::unsigned_shift(int32_t num, int32_t shift) {
    return (shift & 0x1f) == 0 ? num : (((uint32_t)num >> 1) & 0x7fffffff) >> ((shift & 0x1f) - 1);
}

} // namespace Lucene
