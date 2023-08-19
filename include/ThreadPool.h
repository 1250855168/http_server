
#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> threads;  // 线程池中的线程
  std::queue<std::function<void()>> tasks;  // 任务队列，存储待执行的任务
  std::mutex tasks_mtx;  // 任务队列的互斥锁
  std::condition_variable cv;  // 条件变量，用于线程间的同步
  bool stop;  // 线程池停止标志

public:
  ThreadPool(int size = 10);  // 构造函数，创建指定数量的线程
  ~ThreadPool();  // 析构函数，销毁线程池

  // void add(std::function<void()>);
  template <class F, class... Args>
  auto add(F &&f, Args &&...args) -> std::future<decltype(f(args...))>;
};

// 不能放在 cpp 文件，原因是 C++ 编译器不支持模板的分离编译
template <class F, class... Args>
auto ThreadPool::add(F &&f, Args &&...args)
    -> std::future<decltype(f(args...))> {
  using return_type = decltype(f(args...));

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);

    // 在停止线程池之后不允许加入新的任务
    if (stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([task]() { (*task)(); });
  }
  cv.notify_one();
  return res;
}