
#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"
#include <memory>
#include <vector>

EventLoop::EventLoop() : ep(nullptr), threadPool(nullptr), quit(false) {
  ep = new Epoll();
  threadPool = std::make_shared<ThreadPool>();
}

EventLoop::~EventLoop() {
  // delete threadPool;
  delete ep;
}

void EventLoop::loop() {
  while (!quit) {
    std::vector<Channel *> chs;
    chs = ep->poll();
    for (auto it = chs.begin(); it != chs.end(); ++it) {
      (*it)->handleEvent();
    }
  }
}

void EventLoop::updateChannel(Channel *ch) { ep->updateChannel(ch); }

void EventLoop::addThread(std::function<void()> func) { threadPool->add(func); }