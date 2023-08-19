
#pragma once
#include <functional>
#include <memory>

class EventLoop;
class Socket;
class Channel;

class Acceptor {
private:
  std::shared_ptr<EventLoop> loop;  // 指向 EventLoop 对象的指针
  Socket *sock;                     // 指向 Socket 对象的指针
  Channel *acceptChannel;           // 指向负责接受连接的 Channel 对象的指针
  std::function<void(Socket *)> newConnectionCallback;  // 用于处理新连接的回调函数

public:
  Acceptor(std::shared_ptr<EventLoop> _loop);  // 构造函数，接受一个 shared_ptr
  ~Acceptor();  // 析构函数
  void acceptConnection();  // 接受连接的方法
  void setNewConnectionCallback(std::function<void(Socket *)>);  // 设置处理新连接的回调函数
};
