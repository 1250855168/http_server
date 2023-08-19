#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* 从套接字中读取一行数据并存储在缓冲区 buf 中，最多读取 size 个字节 */
int get_line(int sock, char *buf, int size);

/* 关闭连接套接字 cfd 并从 epoll 实例 epfd 中删除该套接字 */
void disconnect(int cfd, int epfd);

/* 处理 HTTP 请求，根据请求的内容执行相应的操作，并将结果发送回客户端套接字 cfd */
void http_request(const char *request, int cfd);

/* 发送 HTTP 响应头部，包括状态码、描述、内容类型和内容长度 */
void send_respond_head(int cfd, int no, const char *desp, const char *type, long len);

/* 发送文件内容到客户端套接字 cfd */
void send_file(int cfd, const char *filename);

/* 发送目录内容到客户端套接字 cfd */
void send_dir(int cfd, const char *dirname);

/* 将字符串 from 进行 URL 编码，并将结果存储在 to 中 */
void encode_str(char *to, int tosize, const char *from);

/* 将字符串 from 进行 URL 解码，并将结果存储在 to 中 */
void decode_str(char *to, char *from);

/* 根据文件名获取文件的 MIME 类型 */
const char *get_file_type(const char *name);