#include <iostream>
#include <memory>   // std::allocator
#include <algorithm>

template <typename T>
class MyVector {
public:
    // 迭代器本质上就是原生指针
    typedef T* iterator;
    typedef const T* const_iterator;

    MyVector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}

    // 析构函数：销毁对象并释放内存
    ~MyVector() {
        clear();
        _alloc.deallocate(_start, capacity());
    }

    // 关键操作：push_back
    void push_back(const T& val) {
        if (_finish == _end_of_storage) {
            // 空间满了，扩容
            size_t new_cap = (capacity() == 0) ? 1 : capacity() * 2;
            reserve(new_cap);
        }
        // 在 _finish 指向的未初始化内存处构造对象
        _alloc.construct(_finish, val);
        _finish++;
    }

    // 核心逻辑：reserve 预留空间
    void reserve(size_t n) {
        if (n > capacity()) {
            size_t old_size = size();
            // 1. 分配新空间
            T* new_data = _alloc.allocate(n);

            // 2. 将旧数据移动/拷贝到新空间
            for (size_t i = 0; i < old_size; ++i) {
                _alloc.construct(new_data + i, std::move(_start[i]));
                _alloc.destroy(_start + i); // 销毁旧对象
            }

            // 3. 释放旧内存
            _alloc.deallocate(_start, capacity());

            // 4. 更新指针
            _start = new_data;
            _finish = _start + old_size;
            _end_of_storage = _start + n;
        }
    }

    // 基本接口
    size_t size() const { return _finish - _start; }
    size_t capacity() const { return _end_of_storage - _start; }
    bool empty() const { return _start == _finish; }

    void clear() {
        while (_finish != _start) {
            _alloc.destroy(--_finish);
        }
    }

    // 下标访问
    T& operator[](size_t n) { return _start[n]; }
    const T& operator[](size_t n) const { return _start[n]; }

    // 迭代器接口
    iterator begin() { return _start; }
    iterator end() { return _finish; }

private:
    std::allocator<T> _alloc; // 内存分配器
    T* _start;
    T* _finish;
    T* _end_of_storage;
};