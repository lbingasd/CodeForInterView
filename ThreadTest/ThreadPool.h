#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
public:
    // 构造函数：初始化时立即创建并启动指定数量的线程
    ThreadPool(size_t threads) : isStop(false)
    {
        for (size_t i = 0; i < threads; i++)
        {
            // 使用 emplace_back 直接在 vector 内部构造线程
            workers.emplace_back([this]() {
                while (true)
                {
                    std::function<void()> task;
                    {
                        // 1. 获取锁：保护任务队列的访问
                        std::unique_lock<std::mutex> lc(mtx);

                        // 2. 阻塞等待：直到 线程池停止 或 队列中有新任务
                        cv.wait(lc, [&]() {
                            return isStop || !tasks.empty();
                        });

                        // 3. 退出判定：
                        // 注意：如果追求“处理完所有任务再退出”，这里应改为 (isStop && tasks.empty())
                        if (isStop && tasks.empty())
                        {
                            break;
                        }

                        // 4. 获取任务：从队列中取出任务对象
                        task = std::move(tasks.front()); // 使用 move 避免 function 对象的深拷贝
                        tasks.pop();
                    } // 5. 自动释放锁：在执行任务前释放锁，允许其他线程取任务

                    // 6. 执行任务：在锁的范围之外执行，保证并发效率
                    if (task) {
                        task();
                    }
                }
            });
        }
    }

    // 停止线程池
    void Stop()
    {
        {
            std::lock_guard<std::mutex> lc(mtx);
            if (isStop) return; // 防止重复停止
            isStop = true;
        }
        // 7. 广播通知：唤醒所有正在 wait 的线程，让他们检查 isStop 标志并退出
        cv.notify_all();

        // 8. 回收线程：等待所有工作线程安全结束
        for (std::thread &worker : workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    // 投递任务
    void Enqueue(std::function<void()> task)
    {
        {
            std::lock_guard<std::mutex> lc(mtx);
            if (isStop) return; // 线程池关闭后不再接受新任务
            tasks.push(std::move(task));
        }
        // 9. 通知一个闲置线程：如果有线程在 wait，唤醒它
        cv.notify_one(); 
    }

    // 禁用默认构造
    ThreadPool() = delete;

    // 析构函数：利用 RAII 确保对象离开作用域时自动停止并清理线程
    ~ThreadPool() {
        Stop();
    }

private:
    bool isStop;
    std::mutex mtx;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::condition_variable cv;
};