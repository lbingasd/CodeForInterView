#include <iostream>
using namespace std;

class EagerSingleton
{
public:
    // 必须是 static，这样才能通过类名直接调用：EagerSingleton::GetInstance()
    static EagerSingleton* GetInstance()
    {
        // C++11 起，静态局部变量的初始化是线程安全的
        static EagerSingleton Instance;
        return &Instance;
    }

    void Show()
    {
        cout << "I'm from EagerSingleton" << endl;
    }
    
    // 禁用拷贝构造和赋值操作符，确保单例唯一性
    EagerSingleton(const EagerSingleton& other) = delete;
    EagerSingleton& operator=(const EagerSingleton& other) = delete;

protected:
    // 构造函数设为 protected 或 private，防止外部直接创建
    EagerSingleton(){}
    ~EagerSingleton(){}
};