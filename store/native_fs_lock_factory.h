#ifndef NATIVE_LOCK_FACTORY_H
#define NATIVE_LOCK_FACTORY_H

#include "fs_lock_factory.h"
#include "lock.h"

namespace Lucene {

class NativeFSLockFactory : public FSLockFactory {
public:
    /// Create a NativeFSLockFactory instance, storing lock files into the
    /// specified lockDirName.
    /// @param lockDirName where lock files are created.
    NativeFSLockFactory(const String& lockDirName = EmptyString);

    virtual ~NativeFSLockFactory() = default;

    LUCENE_CLASS(NativeFSLockFactory);

public:
    virtual LockPtr make_lock(const String& lockName);

    virtual void clear_lock(const String& lockName);
protected:
    /// Simple test to verify locking system is "working".
    void acquire_test_lock();
};

class NativeFSLock : public Lock {
public:
    NativeFSLock(const String& lockDir, const String& lockFileName);
    virtual ~NativeFSLock();

    LUCENE_CLASS(NativeFSLock);

protected:
    String m_lockDir;
    String m_path;
    FileLockPtr m_lock;

    static SynchronizePtr LOCK_HELD_LOCK();
    static HashSet<String> LOCK_HELD();

public:
    virtual bool obtain();
    virtual void release();

    virtual bool is_locked();
    virtual String to_string();

protected:
    bool lock_exists();
};

} // namespace Lucene

#endif // NATIVE_LOCK_FACTORY_H
