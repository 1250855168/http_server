#pragma once
#include <functional>
#include <memory>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
private:
  Epoll *ep;                            // 指向 Epoll 对象的指针
  std::shared_ptr<ThreadPool> threadPool;  // 线程池的智能指针
  bool quit;                            // 事件循环退出标志

public:
  EventLoop();                          // 构造函数
  ~EventLoop();                         // 析构函数

  void loop();                          // 事件循环
  void updateChannel(Channel *);        // 更新通道的事件

  void addThread(std::function<void()>);  // 添加线程到线程池
};