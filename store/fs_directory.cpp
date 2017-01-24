#include "lucene.h"
#include "fs_directory.h"
#include "native_fs_lock_factory.h"
#include "simple_fs_directory.h"
#include "buffered_index_input.h"
#include "lucene_thread.h"
#include "file_utils.h"

#if defined(__APPLE__)
    #include <fcntl.h>
#else
    #include <unistd.h>
#endif

#include <boost/iostreams/device/file_descriptor.hpp>

extern "C" {
#include "md5/md5.h"
}

namespace Lucene {

/// Default read chunk size.
#ifdef LPP_BUILD_64
const int32_t FSDirectory::DEFAULT_READ_CHUNK_SIZE = std::numeric_limits<int32_t>::max();
#else
const int32_t FSDirectory::DEFAULT_READ_CHUNK_SIZE = 100 * 1024 * 1024; // 100mb
#endif

FSDirectory::FSDirectory(const String& path, const LockFactoryPtr& lockFactory) {
    m_checked = false;
    m_chunkSize = DEFAULT_READ_CHUNK_SIZE;
    LockFactoryPtr _lockFactory(lockFactory);

    // new ctors use always NativeFSLockFactory as default
    if (!_lockFactory) {
        _lockFactory = new_lucene<NativeFSLockFactory>();
    }
    m_directory = path;

    if (FileUtils::file_exists(m_directory) && !FileUtils::is_directory(m_directory)) {
        throw NoSuchDirectoryException("File '" + m_directory + "' exists but is not a directory");
    }

    set_lock_factory(_lockFactory);
    // for filesystem based LockFactory, delete the lockPrefix if the locks
    // are placed in index dir. if no index dir is given, set ourselves
    FSLockFactoryPtr lf(std::dynamic_pointer_cast<FSLockFactory>(_lockFactory));

    BOOST_ASSERT(lf);

    if (lf->get_lock_dir().empty()) {
        lf->set_lock_dir(m_directory);
        lf->set_lock_prefix("");
    } else if (lf->get_lock_dir() == m_directory) {
        lf->set_lock_prefix("");
    }
}

FSDirectory::~FSDirectory() {
}

FSDirectoryPtr FSDirectory::open(const String& path) {
    return open(path, LockFactoryPtr());
}

FSDirectoryPtr FSDirectory::open(const String& path, const LockFactoryPtr& lockFactory) {
    return new_lucene<SimpleFSDirectory>(path, lockFactory);
}

void FSDirectory::create_dir() {
    if (!m_checked) {
        if (!FileUtils::file_exists(m_directory) && !FileUtils::create_directory(m_directory)) {
            throw IOException("Cannot create directory: " + m_directory);
        }
        m_checked = true;
    }
}

void FSDirectory::init_output(const String& name) {
    ensure_open();
    create_dir();

    String path(FileUtils::join_path(m_directory, name));
    if (FileUtils::file_exists(path) && !FileUtils::remove_file(path)) {
        // delete existing, if any
        throw IOException("Can't overwrite: " + name);
    }
}

HashSet<String> FSDirectory::list_all(const String& dir) {
    if (!FileUtils::file_exists(dir)) {
        throw NoSuchDirectoryException("Directory '" + dir + "' does not exist");
    } else if (!FileUtils::is_directory(dir)) {
        throw NoSuchDirectoryException("File '" + dir + "' exists but is not a directory");
    }

    HashSet<String> result(HashSet<String>::new_instance());
    // Exclude subdirs
    if (!FileUtils::list_directory(dir, true, result)) {
        throw IOException("Directory '" + dir + "' exists and is a directory, but cannot be listed.");
    }

    return result;
}

HashSet<String> FSDirectory::list_all() {
    ensure_open();
    return list_all(m_directory);
}

bool FSDirectory::file_exists(const String& name) {
    ensure_open();
    return FileUtils::file_exists(FileUtils::join_path(m_directory, name));
}

uint64_t FSDirectory::file_modified(const String& name) {
    ensure_open();
    return FileUtils::file_modified(FileUtils::join_path(m_directory, name));
}

uint64_t FSDirectory::file_modified(const String& directory, const String& name) {
    return FileUtils::file_modified(FileUtils::join_path(directory, name));
}

void FSDirectory::touch_file(const String& name) {
    ensure_open();
    FileUtils::touch_file(FileUtils::join_path(m_directory, name));
}

void FSDirectory::delete_file(const String& name) {
    ensure_open();
    if (!FileUtils::remove_file(FileUtils::join_path(m_directory, name))) {
        throw IOException("Can't delete: " + name);
    }
}

int64_t FSDirectory::file_length(const String& name) {
    ensure_open();
    return FileUtils::file_length(FileUtils::join_path(m_directory, name));
}

void FSDirectory::sync(const String& name) {
    ensure_open();
    String path(FileUtils::join_path(m_directory, name));
    bool success = false;
    for (int32_t retryCount = 0; retryCount < 5; ++retryCount) {
        boost::iostreams::file_descriptor syncFile;
        try {
            syncFile.open(path);
        } catch (...) {
        }
        if (syncFile.is_open()) {
            boost::iostreams::file_descriptor::handle_type fd = syncFile.handle();
#if defined(__APPLE__)
            bool ok = fcntl(fd, F_FULLFSYNC) == 0;
#else
            bool ok = fsync(fd) == 0;
#endif
            syncFile.close();
            if (ok) {
                success = true;
                break;
            }
        }
        LuceneThread::thread_sleep(5); // pause 5 msec
    }
    if (!success) {
        throw IOException("Sync failure: " + path);
    }
}

IndexInputPtr FSDirectory::open_input(const String& name) {
    ensure_open();
    return open_input(name, BufferedIndexInput::BUFFER_SIZE);
}

IndexInputPtr FSDirectory::open_input(const String& name, int32_t bufferSize) {
    return Directory::open_input(name, bufferSize);
}

String FSDirectory::get_lock_id() {
    ensure_open();
    md5_state_t state;
    md5_byte_t digest[16];
    md5_init(&state);
    md5_append(&state, (const md5_byte_t*)m_directory.c_str(), m_directory.size());
    md5_finish(&state, digest);

    static const char *hexDigits = "0123456789abcdef";
    String lockID("lucene-");
    for (int32_t i = 0; i < 16; ++i) {
        lockID += hexDigits[(digest[i] >> 4) & 0x0f];
        lockID += hexDigits[digest[i] & 0x0f];
    }
    return lockID;
}

void FSDirectory::close() {
    SyncLock syncLock(this);
    m_isOpen = false;
}

String FSDirectory::to_string() {
    return get_class_name() + "@" + m_directory + " lockFactory=" + get_lock_factory()->to_string();
}

String FSDirectory::get_file() {
    ensure_open();
    return m_directory;
}

void FSDirectory::set_read_chunk_size(int32_t chunkSize) {
#ifndef LPP_BUILD_64
    this->m_chunkSize = chunkSize;
#endif
}

int32_t FSDirectory::get_read_chunk_size() {
    return m_chunkSize;
}

} // namespace Lucene
