#pragma once

class InetAddress;
class Socket {
private:
    int fd;  // 套接字文件描述符

public:
    Socket();  // 默认构造函数
    Socket(int _fd);  // 接受文件描述符的构造函数
    ~Socket();  // 析构函数

    void bind(InetAddress*);  // 绑定 IP 地址和端口号
    void listen();  // 监听连接请求
    int accept(InetAddress*);  // 接受连接请求，并返回连接的文件描述符

    void connect(InetAddress*);  // 连接到指定的 IP 地址和端口号

    void setnonblocking();  // 设置套接字为非阻塞模式
    int getFd();  // 获取套接字文件描述符
};