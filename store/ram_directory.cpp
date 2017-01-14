#include "lucene.h"
#include "ram_file.h"
#include "ram_directory.h"
#include "ram_input_stream.h"
#include "ram_output_stream.h"
#include "single_instance_lock_factory.h"
// #include "lucene_thread.h"
#include "misc_utils.h"

namespace Lucene {

RAMDirectory::RAMDirectory()
    : m_sizeInBytes(0)
    , m_fileMap(MapStringRAMFile::new_instance())
    , m_copyDirectory(false)
    , m_closeDir(false) {
    set_lock_factory(new_lucene<SingleInstanceLockFactory>());
}

RAMDirectory::RAMDirectory(const DirectoryPtr& dir)
    : m_sizeInBytes(0)
    , m_fileMap(MapStringRAMFile::new_instance())
    , m_dirSource_(dir)
    , m_copyDirectory(true)
    , m_closeDir(false) {
    set_lock_factory(new_lucene<SingleInstanceLockFactory>());
}

RAMDirectory::RAMDirectory(const DirectoryPtr& dir, bool closeDir)
    : m_sizeInBytes(0)
    , m_fileMap(MapStringRAMFile::new_instance())
    , m_dirSource_(dir)
    , m_copyDirectory(true)
    , m_closeDir(closeDir) {
    set_lock_factory(new_lucene<SingleInstanceLockFactory>());
}

RAMDirectory::~RAMDirectory() {
}

void RAMDirectory::initialize() {
    if (m_copyDirectory) {
        Directory::copy(DirectoryPtr(m_dirSource_), shared_from_this(), m_closeDir);
    }
}

HashSet<String> RAMDirectory::list_all() {
    SyncLock syncLock(this);
    ensure_open();
    HashSet<String> result(HashSet<String>::new_instance());

    for (auto fileName = m_fileMap.begin(); fileName != m_fileMap.end(); ++fileName) {
        result.add(fileName->first);
    }

    return result;
}

bool RAMDirectory::file_exists(const String& name) {
    SyncLock syncLock(this);
    ensure_open();
    return m_fileMap.contains(name);
}

uint64_t RAMDirectory::file_modified(const String& name) {
    SyncLock syncLock(this);
    ensure_open();

    auto ramFile = m_fileMap.find(name);

    if (ramFile == m_fileMap.end()) {
        throw FileNotFoundException(name);
    }

    return ramFile->second->get_last_modified();
}

void RAMDirectory::touch_file(const String& name) {
    ensure_open();
    RAMFilePtr file;
    {
        SyncLock syncLock(this);
        auto ramFile = m_fileMap.find(name);

        if (ramFile == m_fileMap.end()) {
            throw FileNotFoundException(name);
        }

        file = ramFile->second;
    }

    // WHY DO THIS
    // int64_t ts1 = MiscUtils::current_time_millis();
    // while (ts1 == MiscUtils::current_time_millis()) {
    //    LuceneThread::thread_sleep(1);
    // }

    file->set_last_modified(MiscUtils::current_time_millis());
}

int64_t RAMDirectory::file_length(const String& name) {
    SyncLock syncLock(this);
    ensure_open();
    auto ramFile = m_fileMap.find(name);

    if (ramFile == m_fileMap.end()) {
        throw FileNotFoundException(name);
    }

    return ramFile->second->get_length();
}

int64_t RAMDirectory::size_in_bytes() {
    SyncLock syncLock(this);
    ensure_open();
    return m_sizeInBytes;
}

void RAMDirectory::delete_file(const String& name) {
    SyncLock syncLock(this);
    ensure_open();

    auto ramFile = m_fileMap.find(name);

    if (ramFile == m_fileMap.end()) {
        throw FileNotFoundException(name);
    }

    m_sizeInBytes -= ramFile->second->get_size_in_bytes();
    m_fileMap.remove(name);
}

IndexOutputPtr RAMDirectory::create_output(const String& name) {
    ensure_open();
    RAMFilePtr  file(new_lucene<RAMFile>(shared_from_this()));
    {
        SyncLock syncLock(this);
        auto existing = m_fileMap.find(name);

        if (existing != m_fileMap.end()) {
            m_sizeInBytes -= existing->second->get_size_in_bytes();
            existing->second->m_directory_.reset();
        }

        m_fileMap.put(name, file);
    }
    return new_lucene<RAMOutputStream>(file);
}

IndexInputPtr RAMDirectory::open_input(const String& name) {
    ensure_open();
    RAMFilePtr file;
    {
        SyncLock syncLock(this);
        auto ramFile = m_fileMap.find(name);

        if (ramFile == m_fileMap.end()) {
            throw FileNotFoundException(name);
        }

        file = ramFile->second;
    }
    return new_lucene<RAMInputStream>(file);
}

void RAMDirectory::close() {
    m_isOpen = false;
    m_fileMap.reset();
}

} // namespace Lucene

