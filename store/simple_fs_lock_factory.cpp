#include <fstream>
#include "lucene.h"
#include "simple_fs_lock_factory.h"
#include "file_utils.h"

namespace Lucene {

SimpleFSLockFactory::SimpleFSLockFactory(const String& lockDir) {
    set_lock_dir(lockDir);
}

LockPtr SimpleFSLockFactory::make_lock(const String& lockName) {
    return new_lucene<SimpleFSLock>(m_lockDir,
                                    m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName);
}

void SimpleFSLockFactory::clear_lock(const String& lockName) {
    if (FileUtils::is_directory(m_lockDir)) {
        String lockPath(FileUtils::join_path(m_lockDir,
                                             m_lockPrefix.empty() ? lockName : m_lockPrefix + "-" + lockName));

        if (FileUtils::file_exists(lockPath) && !FileUtils::remove_file(lockPath)) {
            throw IOException("Can't delete " + lockPath);
        }
    }
}

bool SimpleFSLock::obtain() {
    // Ensure that m_lockDir exists and is a directory.
    if (!FileUtils::file_exists(m_lockDir)) {
        if (!FileUtils::create_directory(m_lockDir)) {
            throw RuntimeException("Can't create directory: " + m_lockDir);
        }
    } else if (!FileUtils::is_directory(m_lockDir)) {
        throw RuntimeException("Found regular file where directory expected: " + m_lockDir);
    }

    if (FileUtils::file_exists(FileUtils::join_path(m_lockDir, m_lockFile))) {
        return false; // 如果文件存在，则认为锁已经存在。在Lucene6.2.1中也说，... fail if it already exists.
    }

    std::ofstream f;

    try {
        f.open(FileUtils::join_path(m_lockDir, m_lockFile), std::ios::binary | std::ios::out);
    } catch (...) {
    }

    bool success = f.is_open();
    f.close();

    return success;
}

void SimpleFSLock::release() {
    String path(FileUtils::join_path(m_lockDir, m_lockFile));

    if (FileUtils::file_exists(path) && !FileUtils::remove_file(path)) {
        throw LockReleaseFailedException("Failed to delete " + path);
    }
}

bool SimpleFSLock::is_locked() {
    return FileUtils::file_exists(FileUtils::join_path(m_lockDir, m_lockFile));
}

String SimpleFSLock::to_string() {
    return get_class_name() + "@" + FileUtils::join_path(m_lockDir, m_lockFile);
}

} // namespace Lucene
