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
  std::shared_ptr<EventLoop> loop;  // 指向 EventLoop 对象的智能指针
  Socket *sock;                     // 指向 Socket 对象的指针
  Channel *channel;                 // 指向 Channel 对象的指针
  std::function<void(int)> deleteConnectionCallback; // 删除连接的回调函数
  std::shared_ptr<std::string> inBuffer;  // 入站缓冲区的智能指针
  std::shared_ptr<Buffer> readBuffer;     // 读缓冲区的智能指针

public:
  Connection(std::shared_ptr<EventLoop> _loop, Socket *_sock);  // 构造函数，接受 EventLoop 智能指针和 Socket 指针
  ~Connection();  // 析构函数

  void echo(int sockfd);  // 回显消息
  void setDeleteConnectionCallback(std::function<void(int)>);  // 设置删除连接的回调函数
  void send(int sockfd);  // 发送消息
};