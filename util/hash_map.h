#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <unordered_map>
#include "lucene_sync.h"
#include "lucene_factory.h"

namespace Lucene {

/// Utility template class to handle hash maps that can be safely copied and shared
template < class KEY, class VALUE, class HASH = std::hash<KEY>, class EQUAL = std::equal_to<KEY> >
class HashMap : public LuceneSync {
public:
    typedef HashMap<KEY, VALUE, HASH, EQUAL> this_type;
    typedef std::pair<KEY, VALUE> key_value;
    typedef std::unordered_map<KEY, VALUE, HASH, EQUAL> map_type;
    typedef typename map_type::iterator iterator;
    typedef typename map_type::const_iterator const_iterator;
    typedef KEY key_type;
    typedef VALUE value_type;

    virtual ~HashMap() {
    }

protected:
    std::shared_ptr<map_type> m_container;

public:
    static this_type new_instance() {
        this_type instance;
        instance.m_container = Lucene::new_instance<map_type>();
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

    bool operator! () const {
        return !m_container;
    }

    this_type& operator= (const this_type& other) {
        m_container = other.m_container;
        return *this;
    }

    void put(const KEY& key, const VALUE& value) {
        (*m_container)[key] = value;
    }

    template <class ITER>
    void putAll(ITER first, ITER last) {
        for (iterator current = first; current != last; ++current) {
            (*m_container)[current->first] = current->second;
        }
    }

    template <class ITER>
    void remove(ITER pos) {
        m_container->erase(pos);
    }

    template <class ITER>
    ITER remove(ITER first, ITER last) {
        return m_container->erase(first, last);
    }

    bool remove(const KEY& key) {
        return (m_container->erase(key) > 0);
    }

    iterator find(const KEY& key) {
        return m_container->find(key);
    }

    VALUE get(const KEY& key) const {
        iterator findValue = m_container->find(key);
        return findValue == m_container->end() ? VALUE() : findValue->second;
    }

    bool contains(const KEY& key) const {
        return (m_container->find(key) != m_container->end());
    }

    VALUE& operator[] (const KEY& key) {
        return (*m_container)[key];
    }
};

/// Utility template class to handle weak keyed maps
template < class KEY, class VALUE, class HASH = std::hash<KEY>, class EQUAL = std::equal_to<KEY> >
class WeakHashMap : public HashMap<KEY, VALUE, HASH, EQUAL> {
public:
    typedef WeakHashMap<KEY, VALUE, HASH, EQUAL> this_type;
    typedef std::pair<KEY, VALUE> key_value;
    typedef typename std::unordered_map<KEY, VALUE, HASH, EQUAL> map_type;
    typedef typename map_type::iterator iterator;

    static this_type new_instance() {
        this_type instance;
        instance.m_container = Lucene::new_instance<map_type>();
        return instance;
    }

    void remove_weak() {
        if (!this->m_container || this->m_container->empty()) {
            return;
        }
        map_type clearCopy;
        for (iterator key = this->m_container->begin(); key != this->m_container->end(); ++key) {
            if (!key->first.expired()) {
                clearCopy.insert(*key);
            }
        }
        this->m_container->swap(clearCopy);
    }

    VALUE get(const KEY& key) {
        iterator findValue = this->m_container->find(key);
        if (findValue != this->m_container->end()) {
            return findValue->second;
        }
        remove_weak(); // 时机为什么在这儿？
        return VALUE();
    }
};

} // namespace Lucene
#endif // HASH_MAP_H
