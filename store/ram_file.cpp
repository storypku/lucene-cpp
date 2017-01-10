#include "lucene.h"
#include "ram_file.h"
#include "ram_directory.h"
#include "misc_utils.h"

namespace Lucene {

RAMFile::RAMFile() {
    this->m_buffers = Collection<ByteArray>::new_instance();
    this->m_length = 0;
    this->m_sizeInBytes = 0;
    this->m_lastModified = MiscUtils::current_time_millis();
}

RAMFile::RAMFile(const RAMDirectoryPtr& directory) {
    this->m_buffers = Collection<ByteArray>::new_instance();
    this->m_length = 0;
    this->m_sizeInBytes = 0;
    this->m_directory_ = directory;
    this->m_lastModified = MiscUtils::current_time_millis();
}

RAMFile::~RAMFile() {
}

int64_t RAMFile::get_length() {
    SyncLock syncLock(this);
    return m_length;
}

void RAMFile::set_length(int64_t length) {
    SyncLock syncLock(this);
    this->m_length = length;
}

int64_t RAMFile::get_last_modified() {
    SyncLock syncLock(this);
    return m_lastModified;
}

void RAMFile::set_last_modified(int64_t lastModified) {
    SyncLock syncLock(this);
    this->m_lastModified = lastModified;
}

ByteArray RAMFile::add_buffer(int32_t size) {
    ByteArray buffer(new_buffer(size));
    {
        SyncLock syncLock(this);
        m_buffers.add(buffer);
        m_sizeInBytes += size;
    }

    RAMDirectoryPtr directory(m_directory_.lock());
    if (directory) {
        SyncLock dirLock(directory);
        directory->m_sizeInBytes += size;
    }

    return buffer;
}

ByteArray RAMFile::get_buffer(int32_t index) {
    SyncLock syncLock(this);
    return m_buffers[index];
}

int32_t RAMFile::num_buffers() {
    SyncLock syncLock(this);
    return m_buffers.size();
}

ByteArray RAMFile::new_buffer(int32_t size) {
    return ByteArray::new_instance(size);
}

int64_t RAMFile::get_size_in_bytes() {
    SyncLock syncLock(this);
    return m_sizeInBytes;
}

} // namespace Lucene

