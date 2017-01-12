#include "lucene.h"
#include "directory.h"
#include "lock_factory.h"
#include "buffered_index_output.h"
// #include "index_file_name_filter.h"
#include "index_input.h"
#include "index_output.h"

namespace Lucene {

void Directory::close() {
    // override
}

void Directory::sync(UNUSED const String& name) {
    // override
}

IndexInputPtr Directory::open_input(const String& name, UNUSED int32_t bufferSize) {
    return open_input(name);
}

LockPtr Directory::make_lock(const String& name) {
    return m_lockFactory->make_lock(name);
}

void Directory::clear_lock(const String& name) {
    if (m_lockFactory) {
        m_lockFactory->clear_lock(name);
    }
}

void Directory::set_lock_factory(const LockFactoryPtr& lockFactory) {
    BOOST_ASSERT(lockFactory);
    this->m_lockFactory = lockFactory;
    this->m_lockFactory->set_lock_prefix(get_lock_id());
}

LockFactoryPtr Directory::get_lock_factory() {
    return m_lockFactory;
}

String Directory::get_lock_id() {
    return to_string();
}

String Directory::to_string() {
    return LuceneObject::to_string() + " lockFactory=" + m_lockFactory->to_string();
}

void Directory::copy(const DirectoryPtr& src, const DirectoryPtr& dest, bool closeDirSrc) {
    HashSet<String> files(src->list_all());
    ByteArray buf(ByteArray::new_instance(BufferedIndexOutput::BUFFER_SIZE));

    for (HashSet<String>::iterator file = files.begin(); file != files.end(); ++file) {
        // if (!IndexFileNameFilter::accept("", *file)) {
        //    continue;
        // }
        IndexOutputPtr os;
        IndexInputPtr is;

        LuceneException finally;

        try {
            // create file in dest directory
            os = dest->create_output(*file);
            // read current file
            is = src->open_input(*file);
            // and copy to dest directory
            int64_t len = is->length();
            int64_t readCount = 0;

            while (readCount < len) {
                int32_t toRead = (readCount + BufferedIndexOutput::BUFFER_SIZE > len) ?
                                 (int32_t)(len - readCount) : BufferedIndexOutput::BUFFER_SIZE;
                is->read_bytes(buf.get(), 0, toRead);
                os->write_bytes(buf.get(), 0, toRead);
                readCount += toRead;

            }
        } catch (LuceneException& e) {
            finally = e;
        }

        // graceful cleanup
        try {
            if (os) {
                os->close();
            }
        } catch (...) {
        }

        try {
            if (is) {
                is->close();
            }
        } catch (...) {
        }

        finally.throw_exception();
    }

    if (closeDirSrc) {
        src->close();
    }
}

void Directory::ensure_open() {
    if (!m_isOpen) {
        throw AlreadyClosedException("This directory is closed");
    }
}

} // namespace Lucene

