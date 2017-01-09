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
    return new_lucene<NativeFSLock>(m_lockDir, m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName);
}

void NativeFSLockFactory::clear_lock(const String& lockName) {
    // note that this isn't strictly required anymore because the existence of these files does not mean
    // they are locked, but still do this in case people really want to see the files go away
    if (FileUtils::is_directory(m_lockDir)) {
        String lockPath(FileUtils::join_path(m_lockDir, m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName));
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



} // namespace Lucene
