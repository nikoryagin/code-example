#include <iostream>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <list>
#include <vector>
template<class KeyType, class ValueType, class Hash = std::hash<KeyType> > class HashMap {
    public:
    typedef std::list<std::pair<KeyType, ValueType>> List;
    typedef typename List::iterator iterator;
    typedef typename List::const_iterator const_iterator;
    List container;
    std::vector<std::list<iterator>> hash_table;
    Hash hasher;
    size_t sz = 101;
    size_t count = 0;
    HashMap(Hash hasher_ ):hasher(hasher_) {
        hash_table.resize(sz);
    }
    HashMap() {
        hash_table.resize(sz);
    }
    template<class Iterator> 
    HashMap(Iterator begin, Iterator end, Hash hasher_):hasher(hasher_) {
        auto it = begin;
        while (it != end) {
            it++;
            sz++;
        }
        sz *= 3;
        hash_table.resize(sz);
        it = begin;
        while (it != end) {
            (*this).insert(*it);
            it++;
        }
    }
    template<class Iterator> 
    HashMap(Iterator begin, Iterator end) {
        auto it = begin;
        while (it != end) {
            it++;
            sz++;
        }
        sz *= 3;
        hash_table.resize(sz);
        it = begin;
        while (it != end) {
            (*this).insert(*it);
            it++;
        }
    }
    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& init, Hash hasher_ ):
            hasher(hasher_) {
        hash_table.resize(sz);
        for (auto& item: init)
            (*this).insert(item);
    }
    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& init) {
        hash_table.resize(sz);
        for (auto& item: init)
            (*this).insert(item);
    }
    const size_t size() const {
        return count;
    }
    const bool empty() const {
        return count == 0;
    }
    const Hash hash_function() const {
        return hasher;
    }
    void insert(std::pair<KeyType, ValueType> item) {
        if (count > sz) {
            (*this).realloc();
        }
        size_t hash = hasher(item.first) % sz;
        for (auto& it:hash_table[hash]) {
            if ((*it).first == item.first)
                return;
        }
        count++;
        container.push_front(item);
        hash_table[hash].push_front(container.begin());
    }
    void erase(KeyType& key) {
        size_t hash = hasher(key) % sz;
        for (auto& it:hash_table[hash]) {
            if ((*it).first == key) {
                container.erase(it);
                count--;
                auto iter = hash_table[hash].begin();
                while (*iter != it)
                    iter++;
                hash_table[hash].erase(iter);
                return;}
        }
    }
    iterator find(const KeyType& key) {
        size_t hash = hasher(key) % sz;
        for (auto& it:hash_table[hash]) {
            if ((*it).first == key) {
                return it;}
        }
        return container.end();
    }
    const_iterator find(const KeyType& key) const {
        size_t hash = hasher(key) % sz;
        for (auto& it:hash_table[hash]) {
            if ((*it).first == key) {
                return it;}
        }
        return container.cend();
    }
    const_iterator begin() const {
        return container.cbegin();
    }
    iterator begin() {
        return container.begin();
    }
    iterator end() {
        return container.end();
    }
    const_iterator end() const {
        return container.cend();
    }
    void realloc() {
       HashMap temporary(container.begin(), container.end(), hasher);
       swap(hash_table, temporary.hash_table);
       swap(container, temporary.container);
       sz = temporary.sz;
    }
    ValueType& operator[] (const KeyType& key) {
        (*this).insert(std::make_pair(key, ValueType()));
        iterator it = (*this).find(key);
        return (*it).second;
    }
    const ValueType& at(const KeyType& key) const  {
        size_t hash = hasher(key) % sz;
        for (auto& it:hash_table[hash]) {
            if ((*it).first == key) {
                return (*it).second;}
        }
        throw std::out_of_range("SA");
    }
    void clear() {
        container.clear();
        hash_table.clear();
        hash_table.resize(20);
        sz = 20;
        count = 0;
    }
};
