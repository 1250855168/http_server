#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(std::shared_ptr<EventLoop> _loop, int _fd) 
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true) {}

Channel::~Channel(){
    if(fd != -1){
        close(fd); // 关闭文件描述符
        fd = -1;
    }
}

void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){ // 若有可读事件发生
        if(useThreadPool)
            loop->addThread(readCallback); // 将读回调函数添加到线程池中执行
        else
            readCallback(); // 直接执行读回调函数
    }
    if(ready & (EPOLLOUT)){ // 若有可写事件发生
        if(useThreadPool)
            loop->addThread(writeCallback); // 将写回调函数添加到线程池中执行
        else
            writeCallback(); // 直接执行写回调函数
    }
}

void Channel::enableRead(){
    events |= EPOLLIN | EPOLLPRI; // 启用读事件监听
    loop->updateChannel(this); // 更新事件循环中的通道
}

void Channel::useET(){
    events |= EPOLLET; // 使用边缘触发模式
    loop->updateChannel(this); // 更新事件循环中的通道
}

int Channel::getFd(){
    return fd; // 获取文件描述符
}

uint32_t Channel::getEvents(){
    return events; // 获取当前监听的事件类型
}

uint32_t Channel::getReady(){
    return ready; // 获取已就绪的事件类型
}

bool Channel::getInEpoll(){
    return inEpoll; // 判断通道是否在 epoll 中
}

void Channel::setInEpoll(bool _in){
    inEpoll = _in; // 设置通道是否在 epoll 中
}

void Channel::setReady(uint32_t _ev){
    ready = _ev; // 设置已就绪的事件类型
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb; // 设置读回调函数
}

void Channel::setUseThreadPool(bool use){
    useThreadPool = use; // 设置是否使用线程池执行回调函数
}