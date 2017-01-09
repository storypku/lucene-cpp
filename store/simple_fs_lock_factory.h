#ifndef SIMPLE_FS_LOCK_FACTORY_H
#define SIMPLE_FS_LOCK_FACTORY_H

#include "fs_lock_factory.h"
#include "lock.h"

namespace Lucene {

class SimpleFSLockFactory : public FSLockFactory {
public:
    /// Create a SimpleFSLockFactory instance, with null (unset) lock
    /// directory. When you pass this factory to a FSDirectory subclass, the lock
    /// directory is automatically set to the directory itself.
    /// Be sure to create one instance for each directory you create!
    SimpleFSLockFactory() = default;

    /// Instantiate using the provided directory name.
    /// @param lockDir where lock files should be created.
    SimpleFSLockFactory(const String& lockDir);

    virtual ~SimpleFSLockFactory() = default;

public:
    virtual LockPtr make_lock(const String& lockName);

    virtual void clear_lock(const String& lockName);
};


class SimpleFSLock : public Lock {
public:
    SimpleFSLock(const String& lockDir, const String& lockFileName)
        : m_lockDir(lockDir), m_lockFile(lockFileName) {
    }

    virtual ~SimpleFSLock() = default;
    LUCENE_CLASS(SimpleFSLock);

public:
    String m_lockDir;
    String m_lockFile;

public:
    virtual bool obtain();
    virtual void release();

    virtual bool is_locked();
    virtual String to_string();
};

} // namespace Lucene
#endif // SIMPLE_FS_LOCK_FACTORY_H
