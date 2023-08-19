#pragma once
#include <arpa/inet.h>

class InetAddress {
private:
    struct sockaddr_in addr;  // 存储 IP 地址和端口号的结构体

public:
    InetAddress();  // 默认构造函数
    InetAddress(const char* _ip, uint16_t _port);  // 接受 IP 地址和端口号的构造函数
    ~InetAddress();  // 析构函数

    void setInetAddr(sockaddr_in _addr);  // 设置地址
    sockaddr_in getAddr();  // 获取地址
};