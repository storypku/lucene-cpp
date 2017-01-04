#include "lucene_object.h"
// #include "string_utils.h"

namespace Lucene {

LuceneObject::LuceneObject() {
}

LuceneObject::~LuceneObject() {
}

void LuceneObject::initialize() {
    // override
}

LuceneObjectPtr LuceneObject::clone(const LuceneObjectPtr& other) {
    if (!other) {
        // boost::throw_exception(UnsupportedOperationException(L"clone must not be null"));
    }
    return other;
}

int32_t LuceneObject::hash_code() {
    return (int32_t)(int64_t)this;
}

bool LuceneObject::equals(const LuceneObjectPtr& other) {
    return (other && this == other.get());
}

int32_t LuceneObject::compare_to(const LuceneObjectPtr& other) {
    return (int32_t)(this - other.get());
}

String LuceneObject::to_string() {
    // return StringUtils::to_string(hash_code());
    return std::to_string(hash_code());
}

} // namespace Eureka
