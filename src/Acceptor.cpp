/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#include "Acceptor.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
#include <cstdio>

Acceptor::Acceptor(std::shared_ptr<EventLoop>_loop)
    : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
  sock = new Socket();
  int flag = 1;
  setsockopt(sock->getFd(), SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  InetAddress *addr = new InetAddress(
      "127.0.0.1", 1234); //这里我没有传入进去 只是传去port 因为用127.0.0.1
                          //Ubuntu下的服务器 win下访问不了
  sock->bind(addr);
  // sock->setnonblocking();
  sock->listen();
  acceptChannel = new Channel(loop, sock->getFd());
  std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
  acceptChannel->setReadCallback(cb);
  acceptChannel->enableRead();
  acceptChannel->setUseThreadPool(false);
  delete addr;
}

Acceptor::~Acceptor() {
  delete sock;
  delete acceptChannel;
}

void Acceptor::acceptConnection() {
  InetAddress *clnt_addr = new InetAddress();
  Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
  char buf[1024];
  std::sprintf(buf, "new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
               inet_ntoa(clnt_addr->getAddr().sin_addr),
               ntohs(clnt_addr->getAddr().sin_port));
  Logger &logger = Logger::getInstance();
  logger.log(LOG_INFO, buf);
  clnt_sock->setnonblocking();
  newConnectionCallback(clnt_sock);
  delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb) {
  newConnectionCallback = _cb;
}