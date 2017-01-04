#include <cstdlib>
#include "lucene_allocator.h"

namespace Lucene {

void* AllocMemory(size_t size) {
    return malloc(size);
}

void* ReallocMemory(void* memory, size_t size) {
    if (memory == nullptr) {
        return AllocMemory(size);
    }

    if (size == 0) {
        FreeMemory(memory);
        return nullptr;
    }

    return realloc(memory, size);
}

void FreeMemory(void* memory) {
    if (memory == nullptr) {
        return;
    }
    free(memory);
}

} // namespace Lucene
