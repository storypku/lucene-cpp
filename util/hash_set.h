#ifndef HASH_SET_H
#define HASH_SET_H

// #include <boost/unordered_set.hpp>
#include <unordered_set>
#include "lucene_sync.h"

namespace Lucene {

/// Utility template class to handle hash set collections that can be safely copied and shared
///                                 boost::hash
template < class TYPE, class HASH = std::hash<TYPE>, class EQUAL = std::equal_to<TYPE> >
class HashSet : public LuceneSync {
public:
    typedef HashSet<TYPE, HASH, EQUAL> this_type;
    typedef std::unordered_set<TYPE, HASH, EQUAL> set_type;
    typedef typename set_type::iterator iterator;
    typedef typename set_type::const_iterator const_iterator;
    typedef TYPE value_type;

    virtual ~HashSet() {
    }

protected:
    std::shared_ptr<set_type> m_container;

public:
    static this_type new_instance() {
        this_type instance;
        instance.m_container = Lucene::new_instance<set_type>();
        return instance;
    }

    template <class ITER>
    static this_type new_instance(ITER first, ITER last) {
        this_type instance;
        instance.m_container = Lucene::new_instance<set_type>(first, last);
        return instance;
    }

    void reset() {
        m_container.reset();
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

    operator bool() const {
        return m_container.get() != NULL;
    }

    bool operator!() const {
        return !m_container;
    }

    this_type& operator= (const this_type& other) {
        m_container = other.m_container;
        return *this;
    }

    bool add(const TYPE& type) {
        return m_container->insert(type).second;
    }

    template <class ITER>
    void add_all(ITER first, ITER last) {
        m_container->insert(first, last);
    }

    bool remove(const TYPE& type) {
        return (m_container->erase(type) > 0);
    }

    iterator find(const TYPE& type) {
        return m_container->find(type);
    }

    bool contains(const TYPE& type) const {
        return (m_container->find(type) != m_container->end());
    }
};

} // namespace Lucene

#endif // HASH_SET_H
