#include <fstream>
#include <boost/interprocess/sync/file_lock.hpp>
#include "lucene.h"
#include "native_fs_lock_factory.h"
#include "file_utils.h"

namespace Lucene {

NativeFSLockFactory::NativeFSLockFactory(const String& lockDirName) {
    set_lock_dir(lockDirName);
}

LockPtr NativeFSLockFactory::make_lock(const String& lockName) {
    SyncLock syncLock(this);
    return new_lucene<NativeFSLock>(m_lockDir,
                                    m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName);
}

void NativeFSLockFactory::clear_lock(const String& lockName) {
    // note that this isn't strictly required anymore because the existence of these files does not mean
    // they are locked, but still do this in case people really want to see the files go away
    if (FileUtils::is_directory(m_lockDir)) {
        String lockPath(FileUtils::join_path(m_lockDir,
                                             m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName));

        if (FileUtils::file_exists(lockPath) && !FileUtils::remove_file(lockPath)) {
            throw IOException("Failed to delete: " + lockPath);
        }
    }
}

NativeFSLock::NativeFSLock(const String& lockDir, const String& lockFileName)
    : m_lockDir(lockDir) {
    m_path = FileUtils::join_path(m_lockDir, lockFileName);
}

NativeFSLock::~NativeFSLock() {
    try {
        release();
    } catch (...) {
    }
}

// 干嘛用的什么鬼
// A lock for held locks.
SynchronizePtr NativeFSLock::LOCK_FOR_HELD_LOCKS() {
    static SynchronizePtr _lock4HeldLocks;

    if (!_lock4HeldLocks) {
        _lock4HeldLocks = new_instance<Synchronize>();
    }

    return _lock4HeldLocks;
}

HashSet<String> NativeFSLock::HELD_LOCKS() {
    static HashSet<String> _HELD_LOCKS;

    if (!_HELD_LOCKS) {
        _HELD_LOCKS = HashSet<String>::new_instance();
    }

    return _HELD_LOCKS;
}

// protected
bool NativeFSLock::lock_exists() {
    SyncLock syncLock(this);
    return m_lock.get() != NULL;
}

bool NativeFSLock::obtain() {
    SyncLock syncLock(this);

    if (lock_exists()) {
        // our instance is already locked.
        return false;
    }

    // ensure that lockdir exists and is a directory.
    if (!FileUtils::file_exists(m_lockDir)) {
        if (!FileUtils::create_directory(m_lockDir)) {
            throw IOException("Can't create directory: " + m_lockDir);
        }
    } else if (!FileUtils::is_directory(m_lockDir)) {
        throw IOException("Found regular file where directory expected: " + m_lockDir);
    }

    bool markedHeld = false;

    // make sure that nobody else in-process has this lock held already and
    // mark it held if not.
    {
        SyncLock heldLock(LOCK_FOR_HELD_LOCKS());

        if (HELD_LOCKS().contains(m_path)) {
            // someone else already has the lock
            return false;
        } else {
            // this "reserves" the fact that we are the one thread trying to
            // obtain this lock, so we own the only instance of a channel
            // against this file.
            HELD_LOCKS().add(m_path);
            markedHeld = true;
        }
    }

    try {
        // we can get intermittent "access denied" here, so we treat this as failure to acquire the lock
        std::ofstream f(m_path, std::ios::binary | std::ios::out);

        if (f.is_open()) {
            std::string lockpath;
            {
                lockpath = boost::filesystem::path(m_path).string();
            }
            m_lock = new_instance<boost::interprocess::file_lock>(lockpath.c_str());
            m_lock->lock();
        }
    } catch (...) {
        m_lock.reset();
    }

    if (markedHeld && !lock_exists()) {
        SyncLock heldLock(LOCK_FOR_HELD_LOCKS());
        HELD_LOCKS().remove(m_path);
        return false;
    }

    return true;
}

void NativeFSLock::release() {
    SyncLock syncLock(this);

    if (lock_exists()) {
        try {
            m_lock->unlock();
            m_lock.reset();
        } catch (...) {
        }

        {
            SyncLock heldLock(LOCK_FOR_HELD_LOCKS());
            HELD_LOCKS().remove(m_path);
        }

        // we don't care anymore if the file can't be deleted because it's held
        // up by another process. (eg. AntiVirus. 什么鬼？) NativeFSLock does
        // not depend on the existence/absence of the lock file.
        FileUtils::remove_file(m_path);
    } else {
        // if we don't hold the lock, and somebody still called release(), for example as a result of
        // calling IndexWriter.unlock(), we should attempt to obtain the lock and release it.  If the
        // obtain fails, it means the lock cannot be released, and we should throw a proper exception
        // rather than silently failing/not doing anything.
        bool obtained = false;
        LuceneException finally;

        try {
            obtained = this->obtain();

            if (!obtained) {
                throw LockReleaseFailedException("Can't forcefully unlock a NativeFSLock which is held by another indexer component: "
                                                 + m_path);
            }
        } catch (LuceneException& e) {
            finally = e;
        }

        if (obtained) {
            this->release();
        }

        finally.throw_exception();
    }
}

// 真是令人望而却步的判断逻辑…… by storypku
bool NativeFSLock::is_locked() {
    SyncLock syncLock(this);

    // the test for is_locked is not directly possible with native file locks:
    // first a short cut, if a lock reference in this instance is available.
    if (lock_exists()) {
        return true;
    }

    // look if lock file is present; if not, there can definitely be no lock!
    if (!FileUtils::file_exists(m_path)) {
        return false;
    }

    // try to obtain and release (if was locked) the lock.
    try {
        bool obtained = this->obtain();

        if (obtained) {
            this->release();
        }

        return !obtained;
    } catch (LuceneException&) {
        return false;
    }
}

String NativeFSLock::to_string() {
    return get_class_name() + "@" + m_path;
}

} // namespace Lucene

