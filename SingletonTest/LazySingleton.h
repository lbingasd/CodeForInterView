#include <iostream>
#include <string>
#include <mutex>

using namespace std;

class LazySingleton
{
public:
    static LazySingleton* GetInstance()
    {
        // 第一次检查：性能优化，实例存在则直接返回
        //每一个线程在想要获取实例时，无论对象是否已经创建，都必须先去“排队领钥匙”（获取 mutex 锁）。加锁和解锁操作涉及到操作系统内核的调用，是非常耗时的。
        if (Instance == nullptr)
        {
            // 加锁
            lock_guard<mutex> lc(mtx);
            // 第二次检查：安全保障，防止多个线程同时通过第一次检查
            if (Instance == nullptr)
            {
                Instance = new LazySingleton();
            }
        }
        return Instance;
    }

    void Show()
    {
        cout << "I'm from LazySingleton" << endl;
    }

protected:
    LazySingleton() {}
    ~LazySingleton() {}
    // 禁用拷贝和赋值
    LazySingleton(const LazySingleton& other) = delete;
    LazySingleton& operator=(const LazySingleton& other) = delete;

private:
    static LazySingleton* Instance;
    static mutex mtx; // 静态成员变量声明
};

// 静态成员变量必须在类外进行定义和初始化
LazySingleton* LazySingleton::Instance = nullptr;
mutex LazySingleton::mtx;