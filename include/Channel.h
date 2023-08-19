#pragma once
#include <functional>
#include <memory>
class Socket;
class EventLoop;

class Channel {
private:
    std::shared_ptr<EventLoop> loop;    // 指向 EventLoop 对象的智能指针
    int fd;                            // 文件描述符
    uint32_t events;                   // 注册的事件类型
    uint32_t ready;                    // 就绪的事件类型
    bool inEpoll;                      // 是否在 Epoll 中
    bool useThreadPool;                // 是否使用线程池
    std::function<void()> readCallback;    // 读回调函数
    std::function<void()> writeCallback;   // 写回调函数

public:
    Channel(std::shared_ptr<EventLoop> _loop, int _fd);  // 构造函数，接受 EventLoop 智能指针和文件描述符
    ~Channel();  // 析构函数

    void handleEvent();  // 处理事件
    void enableRead();   // 启用读事件

    int getFd();  // 获取文件描述符
    uint32_t getEvents();  // 获取注册的事件类型
    uint32_t getReady();   // 获取就绪的事件类型
    bool getInEpoll();     // 获取是否在 Epoll 中
    void setInEpoll(bool _in = true);  // 设置是否在 Epoll 中
    void useET();  // 使用边缘触发模式

    void setReady(uint32_t);  // 设置就绪的事件类型
    void setReadCallback(std::function<void()>);  // 设置读回调函数
    void setUseThreadPool(bool use = true);  // 设置是否使用线程池
};