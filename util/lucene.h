#ifndef LUCENE_H
#define LUCENE_H

#if (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4))
#   define UNUSED __attribute__((__unused__))
#else
#   define UNUSED
#endif

#include <boost/assert.hpp>

#include "array.h"
#include "hash_map.h"
#include "hash_set.h"
#include "lucene_object.h"
#include "lucene_types.h"
#include "lucene_thread.h"
#include "lucene_exception.h"
#include "collection.h"
#include "synchronize.h"

namespace Lucene {

typedef Array<uint8_t> ByteArray;
typedef HashMap< String, RAMFilePtr > MapStringRAMFile;

} // namespace Lucene

#endif // LUCENE_H
