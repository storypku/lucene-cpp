#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <cstdint>
#include <algorithm>

namespace Lucene {

class MiscUtils {
public:
    /// Copy elements from on buffer to another
    template <typename SOURCE, typename DEST>
    static void array_copy(SOURCE source, int32_t sourceOffset, DEST dest, int32_t destOffset, int32_t length) {
        std::copy(source + sourceOffset, source + sourceOffset + length, dest + destOffset);
    }

    /// Fill buffer with given element
    template <typename DEST, typename FILL>
    static void array_fill(DEST dest, int32_t destFrom, int32_t destTo, FILL value) {
        std::fill(dest + destFrom, dest + destTo, value);
    }

    /// Perform unsigned right-shift (left bits are zero filled)
    static int64_t unsignedShift(int64_t num, int64_t shift);

    /// Perform unsigned right-shift (left bits are zero filled)
    static int32_t unsignedShift(int32_t num, int32_t shift);
};

} // namespace Lucene

#endif // MISC_UTILS_H
