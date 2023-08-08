/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#include "Connection.h"
#include "Buffer.h"
#include "Channel.h"
#include "Socket.h"
#include "util.h"
#include <arpa/inet.h>
#include <cstdio>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

#include "Http_request.h"
#include "Logger.h"

Connection::Connection(std::shared_ptr<EventLoop> _loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr), inBuffer(new std::string()),
      readBuffer(nullptr) {
  channel = new Channel(loop, sock->getFd());
  channel->enableRead();
  channel->useET();
  std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
  channel->setReadCallback(cb);
  channel->setUseThreadPool(true);
  readBuffer = std::make_shared<Buffer>();
}

Connection::~Connection() {
  delete channel;
  delete sock;
  // delete readBuffer;
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb) {
  deleteConnectionCallback = _cb;
}

void Connection::echo(int sockfd) {
  Logger &logger = Logger::getInstance();
  char buf[1024]; // 这个buf大小无所谓
  while (
      true) { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    bzero(&buf, sizeof(buf));
    ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
    if (bytes_read > 0) {
      readBuffer->append(buf, bytes_read);
    } else if (bytes_read == -1 && errno == EINTR) { // 客户端正常中断、继续读取
      printf("continue reading\n");
      continue;
    } else if (bytes_read == -1 &&
               ((errno == EAGAIN) ||
                (errno ==
                 EWOULDBLOCK))) { // 非阻塞IO，这个条件表示数据全部读取完毕
      std::sprintf(buf, "message from client fd %d: ", sockfd);
      logger.log(LOG_INFO, buf);
      logger.log(LOG_INFO, readBuffer->c_str());
      // errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1,
      // "socket write error");
      send(sockfd);
      readBuffer->clear();
      break;
    } else if (bytes_read == 0) { // EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", sockfd);
      deleteConnectionCallback(sockfd);
      break;
    } else if (bytes_read == -1) {
      // 读取数据时出现了错误
      std::error_code ec = std::error_code(errno, std::system_category());
      if (ec == std::errc::bad_file_descriptor) {
        logger.log(LOG_INFO, "bad_file_descriptor");
        break;
      } else {
        std::cerr << "Error reading data: " << ec.message() << std::endl;
        logger.log(LOG_INFO, "Error reading data:");
      }
    }
  }
}

void Connection::send(int sockfd) {
  std::string s1;

  // 找到第一个 '\n' 字符
  const char *line_end = strchr(readBuffer->c_str(), '\n');

  // 如果找到了 '\n' 字符
  if (line_end != nullptr) {
    // 计算行的长度
    size_t line_len = line_end - readBuffer->c_str();

    // 复制行到 s1 中
    char line[line_len + 1];
    strncpy(line, readBuffer->c_str(), line_len);
    line[line_len] = '\0';
    s1 = line;
  }

  if (s1.find("GET") != std::string::npos) { // 请求行: get
    // http/1.1 处理http请求
    http_request(s1.c_str(), sockfd);

    // 关闭套接字, sockfd从epoll上del
    deleteConnectionCallback(sockfd);
  }
}
