/******************************
 *   author: yuesong-feng
 *
 *
 *
 ******************************/
#pragma once
#include <map>
#include <memory>
#include <mutex>
class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
private:
  static Server *instance;
  std::shared_ptr<EventLoop> loop;
  Acceptor *acceptor;
  std::map<int, std::shared_ptr<Connection>> connections;
  Server(std::shared_ptr<EventLoop>);
  ~Server();
  static std::mutex mutex;

public:
  static Server *getServerInstance(std::shared_ptr<EventLoop> loop) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!instance) {
      instance = new Server(loop);
    }
    return instance;
  }

  static void destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance) {
      delete instance;
      instance = nullptr;
    }
  }

  void handleReadEvent(int);
  void newConnection(Socket *sock);
  void deleteConnection(int sockfd);
};
