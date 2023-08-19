#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

class Epoll {
private:
    int epfd;                       // epoll 文件描述符
    struct epoll_event *events;     // 存储事件的数组

public:
    Epoll();                        // 构造函数
    ~Epoll();                       // 析构函数

    void updateChannel(Channel*);   // 更新通道的事件
    void deleteChannel(Channel*);   // 删除通道

    std::vector<Channel*> poll(int timeout = -1);   // 等待事件发生并返回就绪的通道
};