#include "Acceptor.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
#include <cstdio>

Acceptor::Acceptor(std::shared_ptr<EventLoop> _loop)
    : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
  sock = new Socket();
  int flag = 1;
  setsockopt(sock->getFd(), SOL_SOCKET, SO_REUSEADDR, &flag,
             sizeof(flag)); // 设置套接字选项，允许地址重用
  InetAddress *addr = new InetAddress(
      "127.0.0.1", 1234); // 创建一个 IPv4 地址对象，监听本地地址和端口 1234
  sock->bind(addr); // 将套接字绑定到指定的地址和端口
  sock->listen();   // 开始监听连接请求
  acceptChannel = new Channel(
      loop, sock->getFd()); // 创建一个 Channel 对象来处理套接字上的事件
  std::function<void()> cb =
      std::bind(&Acceptor::acceptConnection,
                this); // 创建一个回调函数，用于处理可读事件，即接受新连接
  acceptChannel->setReadCallback(cb);     // 设置可读事件的回调函数
  acceptChannel->enableRead();            // 启用可读事件
  acceptChannel->setUseThreadPool(false); // 设置不使用线程池来处理事件
  delete addr;                            // 释放地址对象的内存
}

Acceptor::~Acceptor() {
  delete sock;          // 释放套接字对象的内存
  delete acceptChannel; // 释放 Channel 对象的内存
}

void Acceptor::acceptConnection() {
  InetAddress *clnt_addr =
      new InetAddress(); // 创建一个地址对象，用于存储客户端的地址信息
  Socket *clnt_sock = new Socket(
      sock->accept(clnt_addr)); // 接受客户端的连接请求，返回一个新的套接字对象
  char buf[1024];
  std::sprintf(
      buf, "new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
      inet_ntoa(clnt_addr->getAddr().sin_addr),
      ntohs(clnt_addr->getAddr()
                .sin_port)); // 打印客户端的文件描述符、IP地址和端口号信息
  Logger &logger = Logger::getInstance(); // 获取日志记录器的实例
  logger.log(LOG_INFO, buf);              // 记录日志信息
  clnt_sock->setnonblocking(); // 将客户端套接字设置为非阻塞模式
  newConnectionCallback(clnt_sock); // 调用回调函数，处理新的连接
  delete clnt_addr;                 // 释放地址对象的内存
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb) {
  newConnectionCallback = _cb; // 设置新连接的回调函数
}