/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#pragma once
#include <functional>
#include <memory>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop {
private:
  Epoll *ep;
  std::shared_ptr<ThreadPool> threadPool;
  bool quit;

public:
  EventLoop();
  ~EventLoop();

  void loop();
  void updateChannel(Channel *);

  void addThread(std::function<void()>);
};
