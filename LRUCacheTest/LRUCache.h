#include <iostream>
#include <unordered_map>
#include <list>
#include <mutex>

template<typename K, typename V>
class LRUCache 
{
public:
    LRUCache(size_t capacity) : _capacity(capacity) {}

    // 获取数据
    bool Get(const K& key, V& value) {
        std::lock_guard<std::mutex> lock(_mtx);
        
        auto it = _map.find(key);
        if (it == _map.end()) {
            return false;
        }

        // 将访问过的节点移动到链表头部（最近使用）
        _list.splice(_list.begin(), _list, it->second);
        value = it->second->second;
        return true;
    }

    // 插入数据
    void Put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(_mtx);

        auto it = _map.find(key);
        if (it != _map.end()) {
            // key 已存在，更新值并移到头部
            it->second->second = value;
            _list.splice(_list.begin(), _list, it->second);
            return;
        }

        // 如果达到容量上限，淘汰最久未使用的（尾部）
        if (_list.size() == _capacity) {
            K lastKey = _list.back().first;
            _map.erase(lastKey);
            _list.pop_back();
        }

        // 插入新节点到头部
        _list.emplace_front(key, value);
        _map[key] = _list.begin();
    }

private:
    size_t _capacity;
    std::mutex _mtx;
    
    // 存储键值对：{Key, Value}
    std::list<std::pair<K, V>> _list;
    
    // 存储 Key 到链表迭代器的映射，实现 O(1) 查找
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> _map;
};