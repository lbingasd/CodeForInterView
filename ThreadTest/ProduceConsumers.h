#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>
#include <condition_variable>

class ProduceConsumers
{
public:
    // 构造函数：初始化缓冲区容量
    ProduceConsumers(size_t cap) : capacity(cap), isStop(false) {}

    // 析构函数：确保对象销毁时资源能正确释放
    ~ProduceConsumers() {
        Stop(); // 安全起见，销毁前强制停止
    }

    // 生产者推入数据
    void Push(int value)
    {
        // 1. 使用 unique_lock，因为 wait 需要在阻塞时释放锁，醒来时重新加锁
        std::unique_lock<std::mutex> lc(mtx);

        /* * 2. wait 逻辑：
         * 如果返回 false，线程释放锁并进入阻塞状态。
         * 如果返回 true，线程继续执行。
         * 条件：系统未停止 且 缓冲区还没满
         */
        cv_producer.wait(lc, [&](){
            return isStop || buffer.size() < capacity;
        });

        // 3. 检查唤醒原因：如果是为了停止系统，则直接返回
        if (isStop) return;

        buffer.push(value);
        std::cout << "Push value: " << value << " | Current size: " << buffer.size() << std::endl;

        // 4. 唤醒一个正在等待数据的消费者
        cv_consumer.notify_one();
    }

    // 消费者拉取数据
    void Pop()
    {
        std::unique_lock<std::mutex> lc(mtx);

        /* * wait 条件：系统未停止 且 缓冲区不为空
         */
        cv_consumer.wait(lc, [&](){
            return isStop || !buffer.empty();
        }); 

        // 如果是为了停止且缓冲区已空，则退出
        if (isStop && buffer.empty()) return;

        int value = buffer.front();
        buffer.pop();
        std::cout << "Pop value: " << value << " | Remaining size: " << buffer.size() << std::endl;

        // 5. 唤醒一个正在等待空位的生产者
        cv_producer.notify_one();
    }

    // 停止所有线程
    void Stop()
    {
        {
            // 使用 lock_guard 即可，不需要在 Stop 中释放锁
            std::lock_guard<std::mutex> lc(mtx);
            isStop = true; // 修改标志位
        }
        // 6. 核心：必须 notify_all，否则还在 wait 状态的线程会死锁
        cv_producer.notify_all();
        cv_consumer.notify_all();
    }

private:
    bool isStop;                        // 系统停止标志位
    size_t capacity;                    // 缓冲区最大容量
    std::mutex mtx;                     // 互斥锁，保护共享资源 buffer
    std::queue<int> buffer;             // 共享缓冲区（队列）
    std::condition_variable cv_producer; // 生产者的条件变量
    std::condition_variable cv_consumer; // 消费者的条件变量
};

/**
 * 生产者任务函数
 */
void ProducerTask(ProduceConsumers& q, int id) {
    for (int i = 0; i < 5; ++i) {
        q.Push(id * 100 + i);
        // 模拟生产耗时
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

/**
 * 消费者任务函数
 */
void ConsumerTask(ProduceConsumers& q) {
    // 实际场景中消费者通常是 while(true)
    for (int i = 0; i < 10; ++i) {
        q.Pop();
        // 模拟消费耗时（消费比生产慢时，缓冲区会堆积）
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}