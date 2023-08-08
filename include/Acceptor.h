/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#pragma once
#include <functional>
#include <memory>

class EventLoop;
class Socket;
class Channel;
class Acceptor {
private:
  std::shared_ptr<EventLoop> loop;
  Socket *sock;
  Channel *acceptChannel;
  std::function<void(Socket *)> newConnectionCallback;

public:
  Acceptor(std::shared_ptr<EventLoop> _loop);
  ~Acceptor();
  void acceptConnection();
  void setNewConnectionCallback(std::function<void(Socket *)>);
};
