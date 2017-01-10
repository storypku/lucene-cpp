#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "lucene_object.h"

namespace Lucene {

/// A Directory is a flat list of files.  Files may be written once, when they are created.  Once a file
/// is created it may only be opened for read, or deleted.  Random access is permitted both when reading
/// and writing.  Directory locking is implemented by an instance of LockFactory, and can be changed
/// for each Directory instance using #setLockFactory.

} // namespace Lucene
#endif // DIRECTORY_H
