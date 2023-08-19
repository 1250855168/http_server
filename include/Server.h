
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
  static Server *instance;  // Server 单例实例指针
  std::shared_ptr<EventLoop> loop;  // EventLoop 实例的共享指针
  Acceptor *acceptor;  // Acceptor 实例指针
  std::map<int, std::shared_ptr<Connection>> connections;  // 存储连接的映射表，键为套接字描述符，值为 Connection 的共享指针
  Server(std::shared_ptr<EventLoop>);  // 私有构造函数
  ~Server();  // 析构函数
  static std::mutex mutex;  // 互斥锁

public:
  static Server *getServerInstance(std::shared_ptr<EventLoop> loop) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!instance) {
      instance = new Server(loop);
    }
    return instance;
  }  // 获取 Server 单例实例的静态方法

  static void destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance) {
      delete instance;
      instance = nullptr;
    }
  }  // 销毁 Server 单例实例的静态方法

  void handleReadEvent(int);  // 处理读事件的方法
  void newConnection(Socket *sock);  // 建立新连接的方法
  void deleteConnection(int sockfd);  // 删除连接的方法
};