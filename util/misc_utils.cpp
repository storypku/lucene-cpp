#include "misc_utils.h"

namespace Lucene {

int64_t MiscUtils::unsignedShift(int64_t num, int64_t shift) {
    return (shift & 0x3f) == 0 ? num : (((uint64_t)num >> 1) & 0x7fffffffffffffffLL) >> ((shift & 0x3f) - 1);
}

int32_t MiscUtils::unsignedShift(int32_t num, int32_t shift) {
    return (shift & 0x1f) == 0 ? num : (((uint32_t)num >> 1) & 0x7fffffff) >> ((shift & 0x1f) - 1);
}

} // namespace Lucene
