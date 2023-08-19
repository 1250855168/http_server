
#pragma once
#include <string>

class Buffer {
private:
    std::string buf;  // 存储数据的字符串

public:
    Buffer();  // 构造函数
    ~Buffer();  // 析构函数

    // 向缓冲区追加数据
    void append(const char* _str, int _size);

    // 获取缓冲区的大小
    ssize_t size();

    // 返回指向缓冲区数据的指针
    const char* c_str();

    // 清空缓冲区
    void clear();

    // 读取一行数据
    void getline();

    // 设置缓冲区的内容
    void setBuf(const char*);
};

