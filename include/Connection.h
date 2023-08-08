/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#pragma once
#include <functional>
#include <memory>
#include <string>
class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection {
private:
  std::shared_ptr<EventLoop> loop;
  Socket *sock;
  Channel *channel;
  std::function<void(int)> deleteConnectionCallback;
  std::shared_ptr<std::string> inBuffer;
  std::shared_ptr<Buffer> readBuffer;

public:
  Connection(std::shared_ptr<EventLoop> _loop, Socket *_sock);
  ~Connection();

  void echo(int sockfd);
  void setDeleteConnectionCallback(std::function<void(int)>);
  void send(int sockfd);
};
