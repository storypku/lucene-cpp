#ifndef FS_LOCK_FACTORY_H
#define FS_LOCK_FACTORY_H

#include "lock_factory.h"

namespace Lucene {

/// Base class for file system locking implementation.
class FSLockFactory : public LockFactory {
protected:
    FSLockFactory() = default;

public:
    virtual ~FSLockFactory() = default;
    LUCENE_CLASS(FSLockFactory);

protected:
    /// Directory for the lock files.
    String m_lockDir;

public:
    /// Set the lock directory. This method can be only called once to
    /// initialize the lock directory. It is used by FSDirectory
    /// to set the lock directory to itself.  Subclasses can also use
    /// this method to set the directory in the constructor.
    void set_lock_dir(const String& lockDir);

    /// Retrieve the lock directory.
    String get_lock_dir();
};

} // namespace Lucene

#endif // FS_LOCK_FACTORY_H
