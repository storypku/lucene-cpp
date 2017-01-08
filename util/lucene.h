#ifndef LUCENE_H
#define LUCENE_H

#if (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4))
#   define UNUSED __attribute__((__unused__))
#else
#   define UNUSED
#endif

#include "array.h"

namespace Lucene {

typedef Array<uint8_t> ByteArray;

} // namespace Lucene

#include "synchronize.h"
#include "lucene_sync.h"
#include "lucene_signal.h"
#include "lucene_object.h"
#include "lucene_types.h"
#include "lucene_thread.h"
#include "lucene_factory.h"
#include "lucene_exception.h"

#include "hash_set.h"

#endif // LUCENE_H
