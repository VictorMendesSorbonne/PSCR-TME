#pragma once

#include <stddef.h>
#include <ostream>

#include <vector>
#include <forward_list>
namespace pr{
    template <typename K, typename V>
    class HashMap {

    public:
        class Entry {
        public:
            const K key;
            V value;
            Entry(const K &k,const V& v):key(k),value(v){}
        };

    private:
        typedef std::vector<std::forward_list<Entry>> buckets_t;
        buckets_t buckets;
        size_t size;

    public:
        HashMap(size_t sz):size(0), buckets(sz) {}

        V* get(const K & key) {
            size_t h = std::hash<K>()(key);
            size_t target = h % buckets.size();
            for (Entry &e: buckets[target])
                if (e.key == key)
                    return &e.value;
            return nullptr;
        }

        bool put(const K &key, const V &value) {
            size_t h = std::hash<K>()(key);
            size_t target = h % buckets.size();
            for (Entry &e: buckets[target])
                if (e.key == key) {
                    e.value = value;
                    return true;
                }
            size++;
            buckets[h].emplace_front(key,value);
            return false;
        }
        
        /*
        size_t size() const {
            return size;
        }*/

        void grow () {
            HashMap h = HashMap(buckets.size * 2);
            for (int i = 0; i < h.size; i++) {
                for (int j = 0; j < h[i].size; j++) {
                    h.put(h[i][j].key, h[i][j].value);
                }
            }
            buckets = h.buckets;
        }
    };
}