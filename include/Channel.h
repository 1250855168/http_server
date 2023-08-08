/******************************
*   author: yuesong-feng
*   
*
*
******************************/
#pragma once
#include <functional>
#include <memory>
class Socket;
class EventLoop;
class Channel
{
private:
    std::shared_ptr<EventLoop> loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(std::shared_ptr<EventLoop>_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};

