/**
 * provide a simplest version of lru cache
 */

#ifndef WZP_CPP_LIB_BIG_DATA_LRU_CACHE_HPP_
#define WZP_CPP_LIB_BIG_DATA_LRU_CACHE_HPP_

/**
 * Example:
LRUCache cache = new LRUCache( 2 );

cache.put(1, 1);
cache.put(2, 2);
cache.get(1);       // returns 1
cache.put(3, 3);    // evicts key 2
cache.get(2);       // returns -1 (not found)
cache.put(4, 4);    // evicts key 1
cache.get(1);       // returns -1 (not found)
cache.get(3);       // returns 3
cache.get(4);       // returns 4
 */


#include <unordered_map>
#include <vector>
#include <mutex>

// include thee optional module
#include "wzp_cpp_lib/container/optional.hpp"

namespace wzp
{

namespace lru_details
{

// an implement of linked list, to store key
template<typename Key>
struct Node {
    Key key;
    Node* next;
    Node* pre;
    Node(const Key& k) : key(k), next(nullptr), pre(nullptr) {}
};

template<typename Key>
class List {

public:
    // constructor
    List(size_t capacity) : capacity_(capacity) {}

    // deconstructor, destroy the memory
    ~List() {
        Node<Key> *h = head_, *n = nullptr;
        while(h) {
            if(h->next)
                n = h->next;
            else
                n = nullptr;
            delete h;
            h = n;
        }
        head_ = nullptr;
        tail_ = nullptr;
    }

    // upade the least recent used key
    void Update(const Key& key) {
        // must have this key when call this func
        auto* node = locs_map_[key]; // get this key's location
        if(node == tail_)
            return;
        // move the node into the tail
        if(node == head_) {
            head_->next->pre = nullptr;
            head_ = head_->next;
            tail_->next = node;
            node->pre = tail_;
            tail_ = node;
            tail_->next = nullptr;
        } else {
            node->pre->next = node->next;
            node->next->pre = node->pre;
            tail_->next = node;
            node->pre = tail_;
            tail_ = node;
            tail_->next = nullptr;
        }
    }

    // push the new key into the list, return if need to erase one key
    // return true, if need to erase one key
    bool Push(const Key& key, Key* erase_key = nullptr) {
        // check has this node or not
        if(locs_map_.count(key)) {
            Update(key);
            return false;
        }
        // create an insert the new key
        Node<Key>* tmp = new Node<Key>(key);
        locs_map_[key] = tmp;
        // insert the new key into the list
        if(tail_) {
            tail_->next = tmp;
            tmp->pre = tail_;
        }
        tail_ = tmp;
        if(!head_)
            head_ = tmp;
        if(++size_ <= capacity_)
            return false;
        else {
            auto* del = head_;
            head_ = head_->next;
            head_->pre = nullptr;
            *erase_key = del->key;
            // delete this node
            locs_map_.erase(*erase_key);
            delete del;
            del = nullptr;
            return true;
        }
    }

private:
    size_t size_ = 0;
    size_t capacity_;
    Node<Key>* head_ = nullptr;
    Node<Key>* tail_ = nullptr;
    std::unordered_map<Key, Node<Key>*> locs_map_;
};

} //lru_details


// the single thread only version of lru cache
// the simplest one, the key type must have default constructor
template<typename Key, typename Value>
class LRUCache {
public:
    // constructor
    LRUCache(size_t capacity) : capacity_(capacity_), list_(capacity) {}

    // the get interface, to get one key from the list
    optional<Value> Get(const Key& key) {
        if(capacity_ == 0)
            return {};
        optional<Value> val;
        if(hashmap_.count(key)) {
            list_.Update(key);
            val = hashmap_[key];
        }
        return val;
    }

    // the put interface, add new cache into the cache
    void Put(const Key& key, const Value& val) {
        if(capacity_ == 0)
            return;
        if(hashmap_.count(key)) {
            // only update, not insert
            hashmap_[key] = val;
            list_.Push(key); // because has this key, so do not need to erase anyone
        } else {
            // insert a new key, maybe need to erase one old key
            hashmap_.emplace(key, val);
            Key maybe_erase_key;
            if(list_.Push(key, &maybe_erase_key)) {
                hashmap_.erase(maybe_erase_key);
            }
        }
    }

private:
    // the personal members
    size_t capacity_;
    lru_details::List<Key> list_; // save the key
    std::unordered_map<Key, Value> hashmap_; // the real store
};


// the version of mutithread safe
// with a big lock
template<typename Key, typename Value>
class LRUCacheBigLock {
public:
    // constructor
    LRUCacheBigLock(size_t capacity) : capacity_(capacity_), list_(capacity) {}

    // the get interface, to get one key from the list
    optional<Value> Get(const Key& key) {
        std::lock_guard<std::mutex> lock(mut_);
        if(capacity_ == 0)
            return {};
        optional<Value> val;
        if(hashmap_.count(key)) {
            list_.Update(key);
            val = hashmap_[key];
        }
        return val;
    }

    // the put interface, add new cache into the cache
    void Put(const Key& key, const Value& val) {
        std::lock_guard<std::mutex> lock(mut_);
        if(capacity_ == 0)
            return;
        if(hashmap_.count(key)) {
            // only update, not insert
            hashmap_[key] = val;
            list_.Push(key); // because has this key, so do not need to erase anyone
        } else {
            // insert a new key, maybe need to erase one old key
            hashmap_.emplace(key, val);
            Key maybe_erase_key;
            if(list_.Push(key, &maybe_erase_key)) {
                hashmap_.erase(maybe_erase_key);
            }
        }
    }

private:
    // the mutex for multithread
    std::mutex mut_;
    // the personal members
    size_t capacity_;
    lru_details::List<Key> list_; // save the key
    std::unordered_map<Key, Value> hashmap_; // the real store
};


// the concurrent version of lru cache with segment lock
// with num of locks
template<typename Key, typename Value>
class ConcurrentLRUCache {

private:
    const size_t m_mask;
    using InnerCache = LRUCache<Key, Value>;
    std::vector<std::mutex> m_muts;
    std::vector<InnerCache> m_shared_cache;

    // the getter ref of shared map
    InnerCache& GetShared(const Key& key) {
        auto h = std::hash<Key>()(key);
        return m_shared_cache[h & m_mask];
    }

    // the index computer
    inline size_t GetIndex(const Key& key) const {
        auto h = std::hash<Key>()(key);
        return h & m_mask;
    }

public:
    // the construct with a default segment num
    ConcurrentLRUCache(size_t capacity, size_t num_shared = 8)
        : m_mask(num_shared - 1),
          m_muts(num_shared),
          m_shared_cache(num_shared, LRUCache<Key, Value>(capacity)) {
        if((num_shared & m_mask) != 0)
            throw std::runtime_error("num shared param must be a power of 2");
    }

    // the get interface
    optional<Value> Get(const Key& key) {
        size_t idx = GetIndex(key);
        // lock
        std::lock_guard<std::mutex> lock(m_muts[idx]);
        return m_shared_cache[idx].Get(key);
    }

    // the put interface
    void Put(const Key& key, const Value& val) {
        // compute the index
        size_t idx = GetIndex(key);
        std::lock_guard<std::mutex> lock(m_muts[idx]);
        m_shared_cache[idx].Put(key, val);
    }
};

} //wzp

#endif /*WZP_CPP_LIB_BIG_DATA_LRU_CACHE_HPP_*/
