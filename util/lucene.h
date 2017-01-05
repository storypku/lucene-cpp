#ifndef LUCENE_H
#define LUCENE_H

#if (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4))
#   define UNUSED __attribute__((__unused__))
#else
#   define UNUSED
#endif

#include "synchronize.h"
#include "lucene_sync.h"
#include "lucene_signal.h"
#include "lucene_object.h"
#include "lucene_types.h"
#include "lucene_thread.h"
#include "lucene_factory.h"

#endif // LUCENE_H
