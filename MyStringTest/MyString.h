#include <iostream>
#include <cstring>
#include <algorithm>

class MyString {
public:
    // 1. 默认构造函数（处理空字符串）
    MyString() : _data(new char[1]), _size(0) {
        _data[0] = '\0';
    }

    // 2. 带参构造函数
    MyString(const char* str) {
        if (str == nullptr) {
            _data = new char[1];
            _data[0] = '\0';
            _size = 0;
        } else {
            _size = std::strlen(str);
            _data = new char[_size + 1];
            std::strcpy(_data, str);
        }
    }

    // 3. 拷贝构造函数（深拷贝的关键）
    MyString(const MyString& other) : _size(other._size) {
        _data = new char[_size + 1];
        std::strcpy(_data, other._data);
    }

    // 4. 拷贝赋值运算符（现代写法：Copy-and-Swap）
    MyString& operator=(MyString other) {
        swap(*this, other);
        return *this;
    }

    // 5. 移动构造函数 (C++11)
    MyString(MyString&& other) noexcept : _data(nullptr), _size(0) {
        swap(*this, other);
    }

    // 6. 析构函数
    ~MyString() {
        delete[] _data;
    }

    // 辅助函数：交换
    friend void swap(MyString& first, MyString& second) noexcept {
        using std::swap;
        swap(first._data, second._data);
        swap(first._size, second._size);
    }

    // 获取长度和 C 风格字符串
    size_t size() const { return _size; }
    const char* c_str() const { return _data; }

    // 重载输出流
    friend std::ostream& operator<<(std::ostream& os, const MyString& str) {
        os << str._data;
        return os;
    }

private:
    char* _data;   // 指向堆内存的指针
    size_t _size;  // 字符串长度
};