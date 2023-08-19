

#include "Buffer.h"
#include <string.h>
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char* _str, int _size){
    for(int i = 0; i < _size; ++i){
        if(_str[i] == '\0') break; // 如果遇到空字符'\0'，则停止追加
        buf.push_back(_str[i]); // 将字符追加到缓冲区的末尾
    }
}

ssize_t Buffer::size(){
    return buf.size(); // 返回缓冲区的大小
}

const char* Buffer::c_str(){
    return buf.c_str(); // 返回缓冲区的字符数组表示
}

void Buffer::clear(){
    buf.clear(); // 清空缓冲区
}

void Buffer::getline(){
    buf.clear(); // 清空缓冲区
    std::getline(std::cin, buf); // 从标准输入读取一行文本，并存储到缓冲区中
}

void Buffer::setBuf(const char* _buf){
    buf.clear(); // 清空缓冲区
    buf.append(_buf); // 将给定的字符数组追加到缓冲区
}