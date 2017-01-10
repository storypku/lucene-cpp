#include "lucene.h"
#include "directory.h"
#include "lock_factory.h"
#include "buffered_index_output.h"
// TODO #include "index_file_name_filter.h"
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

} // namespace Lucene

