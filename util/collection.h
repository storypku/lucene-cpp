#ifndef COLLECTION_H
#define COLLECTION_H

#include <vector>
#include "lucene_sync.h"

namespace Lucene {

/// Utility template class to handle collections that can be safely copied and shared
template <class TYPE>
class Collection : public LuceneSync {
public:
    typedef Collection<TYPE> this_type;
    typedef std::shared_ptr<this_type> shared_ptr;
    typedef std::vector<TYPE> collection_type;
    typedef typename collection_type::iterator iterator;
    typedef typename collection_type::const_iterator const_iterator;
    typedef TYPE value_type;

    virtual ~Collection() {
    }

protected:
    std::shared_ptr<collection_type> m_container;

public:
    static this_type new_instance(int32_t size = 0) {
        this_type instance;
        instance.m_container = Lucene::new_instance<collection_type>(size);
        return instance;
    }

    template <class ITER>
    static this_type new_instance(ITER first, ITER last) {
        this_type instance;
        instance.m_container = Lucene::new_instance<collection_type>(first, last);
        return instance;
    }

    void reset() {
        resize(0);
    }

    void resize(int32_t size) {
        if (size == 0) {
            m_container.reset();
        } else {
            m_container->resize(size);
        }
    }

    int32_t size() const {
        return (int32_t)m_container->size();
    }

    bool empty() const {
        return m_container->empty();
    }

    void clear() {
        m_container->clear();
    }

    iterator begin() {
        return m_container->begin();
    }

    iterator end() {
        return m_container->end();
    }

    const_iterator begin() const {
        return m_container->begin();
    }

    const_iterator end() const {
        return m_container->end();
    }

    void add(const TYPE& type) {
        m_container->push_back(type);
    }

    void add(int32_t pos, const TYPE& type) {
        m_container->insert(m_container->begin() + pos, type);
    }

    template <class ITER>
    void add_all(ITER first, ITER last) {
        m_container->insert(m_container->end(), first, last);
    }

    template <class ITER>
    void insert(ITER pos, const TYPE& type) {
        m_container->insert(pos, type);
    }

    template <class ITER>
    ITER remove(ITER pos) {
        return m_container->erase(pos);
    }

    template <class ITER>
    ITER remove(ITER first, ITER last) {
        return m_container->erase(first, last);
    }

    void remove(const TYPE& type) {
        m_container->erase(std::remove(m_container->begin(), m_container->end(), type), m_container->end());
    }

    template <class PRED>
    void remove_if(PRED comp) {
        m_container->erase(std::remove_if(m_container->begin(), m_container->end(), comp), m_container->end());
    }

    TYPE remove_first() {
        TYPE front = m_container->front();
        m_container->erase(m_container->begin());
        return front;
    }

    TYPE remove_last() {
        TYPE back = m_container->back();
        m_container->pop_back();
        return back;
    }

    iterator find(const TYPE& type) {
        return std::find(m_container->begin(), m_container->end(), type);
    }

    template <class PRED>
    iterator find_if(PRED comp) {
        return std::find_if(m_container->begin(), m_container->end(), comp);
    }

    bool contains(const TYPE& type) const {
        return (std::find(m_container->begin(), m_container->end(), type) != m_container->end());
    }

    template <class PRED>
    bool contains_if(PRED comp) const {
        return (std::find_if(m_container->begin(), m_container->end(), comp) != m_container->end());
    }

    bool equals(const this_type& other) const {
        return equals(other, std::equal_to<TYPE>());
    }

    template <class PRED>
    bool equals(const this_type& other, PRED comp) const {
        if (m_container->size() != other.m_container->size()) {
            return false;
        }
        return std::equal(m_container->begin(), m_container->end(), other.m_container->begin(), comp);
    }

    int32_t hash_code() {
        return (int32_t)(int64_t)m_container.get();
    }

    void swap(this_type& other) {
        m_container.swap(other->m_container);
    }

    TYPE& operator[] (int32_t pos) {
        return (*m_container)[pos];
    }

    const TYPE& operator[] (int32_t pos) const {
        return (*m_container)[pos];
    }

    operator bool() const {
        return m_container.get() != NULL;
    }

    bool operator! () const {
        return !m_container;
    }

    bool operator== (const this_type& other) {
        return (m_container == other.m_container);
    }

    bool operator!= (const this_type& other) {
        return (m_container != other.m_container);
    }
};

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1) {
    Collection<TYPE> result = Collection<TYPE>::new_instance();
    result.add(a1);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2) {
    Collection<TYPE> result = new_collection(a1);
    result.add(a2);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3) {
    Collection<TYPE> result = new_collection(a1, a2);
    result.add(a3);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4) {
    Collection<TYPE> result = new_collection(a1, a2, a3);
    result.add(a4);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4);
    result.add(a5);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5, const TYPE& a6) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4, a5);
    result.add(a6);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5, const TYPE& a6, const TYPE& a7) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4, a5, a6);
    result.add(a7);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5, const TYPE& a6, const TYPE& a7, const TYPE& a8) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4, a5, a6, a7);
    result.add(a8);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5, const TYPE& a6, const TYPE& a7, const TYPE& a8, const TYPE& a9) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4, a5, a6, a7, a8);
    result.add(a9);
    return result;
}

template <typename TYPE>
Collection<TYPE> new_collection(const TYPE& a1, const TYPE& a2, const TYPE& a3, const TYPE& a4, const TYPE& a5, const TYPE& a6, const TYPE& a7, const TYPE& a8, const TYPE& a9, const TYPE& a10) {
    Collection<TYPE> result = new_collection(a1, a2, a3, a4, a5, a6, a7, a8, a9);
    result.add(a10);
    return result;
}

}

#endif
