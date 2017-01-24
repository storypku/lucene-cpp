#ifndef SIMPLE_FS_DIRECTORY_H
#define SIMPLE_FS_DIRECTORY_H

#include "fs_directory.h"

namespace Lucene {

/// A straightforward implementation of FSDirectory using std::ofstream and
/// std::ifstream.
class SimpleFSDirectory : public FSDirectory {
public:
    /// Create a new SimpleFSDirectory for the named location and
    /// NativeFSLockFactory.
    /// @param path the path of the directory
    /// @param lockFactory the lockFactory to use or null for the default
    /// (NativeFSLockFactory)
    SimpleFSDirectory(const String& path, const LockFactoryPtr& lockFactory = LockFactoryPtr());

    virtual ~SimpleFSDirectory();

    LUCENE_CLASS(SimpleFSDirectory);

public:
    virtual IndexOutputPtr create_output(const String& name);

    virtual IndexInputPtr open_input(const String& name);

    virtual IndexInputPtr open_input(const String& name, int32_t bufferSize);
};

} // namespace Lucene

#endif // SIMPLE_FS_DIRECTORY_H
