#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>
#include <cstdint>
#include <memory>

#include "lucene_allocator.h"
#include "lucene_factory.h"

namespace Lucene {

template <typename TYPE>
class ArrayData {
public:
    ArrayData(int32_t size) : m_data(nullptr) {
        resize(size);
    }

    ~ArrayData() {
        resize(0);
    }

public:
    TYPE* m_data;
    int32_t m_size;

public:
    void resize(int32_t size) {
        if (size == 0) {
            FreeMemory(m_data);
            m_data = nullptr;
        } else if (m_data == nullptr) {
            m_data = (TYPE*)AllocMemory(size * sizeof(TYPE));
        } else {
            m_data = (TYPE*)ReallocMemory(m_data, size * sizeof(TYPE));
        }
        this->m_size = size;
    }
};

/// Utility template class to handle sharable arrays of simple data types
template <typename TYPE>
class Array {
public:
    typedef Array<TYPE> this_type;
    typedef ArrayData<TYPE> array_type;

    Array() : m_array(nullptr) {
    }

protected:
    std::shared_ptr<array_type> m_container;
    array_type* m_array;

public:
    static this_type new_instance(int32_t size) {
        this_type instance;
        instance.m_container = Lucene::new_instance<array_type>(size);
        instance.m_array = instance.m_container.get();
        return instance;
    }

    void reset() {
        resize(0);
    }

    void resize(int32_t size) {
        if (size == 0) {
            m_container.reset();
        } else if (!m_container) {
            m_container = Lucene::new_instance<array_type>(size);
        } else {
            m_container->resize(size);
        }
        m_array = m_container.get();
    }

    TYPE* get() const {
        return m_array->m_data;
    }

    int32_t size() const {
        return m_array->m_size;
    }

    bool equals(const this_type& other) const {
        if (m_array->m_size != other.m_array->m_size) {
            return false;
        }
        return (std::memcmp(m_array->m_data, other.m_array->m_data, m_array->m_size) == 0);
    }

    int32_t hash_code() const {
        return (int32_t)(int64_t)m_array;
    }

    TYPE& operator[] (int32_t i) const {
        BOOST_ASSERT(i >= 0 && i < m_array->m_size);
        return m_array->m_data[i];
    }

    operator bool () const {
        return m_container.get() != nullptr;
    }

    bool operator! () const {
        return !m_container;
    }

    bool operator== (const Array<TYPE>& other) {
        return (m_container == other.m_container);
    }

    bool operator!= (const Array<TYPE>& other) {
        return (m_container != other.m_container);
    }
};

template <class TYPE>
inline std::size_t hash_value(const Array<TYPE>& value) {
    return (std::size_t)value.hash_code();
}

template <class TYPE>
inline bool operator== (const Array<TYPE>& value1, const Array<TYPE>& value2) {
    return (value1.hash_code() == value2.hash_code());
}

}

#endif
