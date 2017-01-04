#ifndef LUCENE_ALLOCATOR_H
#define LUCENE_ALLOCATOR_H

namespace Lucene {

/// Allocate block of memory.
void* AllocMemory(size_t size);

/// Reallocate a given block of memory.
void* ReallocMemory(void* memory, size_t size);

/// Release a given block of memory.
void FreeMemory(void *memory);

} // namespace Lucene

#endif // LUCENE_ALLOCATOR_H
