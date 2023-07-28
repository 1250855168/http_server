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

int get_line(int sock, char *buf, int size);
void disconnect(int cfd, int epfd);
void http_request(const char *request, int cfd);
void send_respond_head(int cfd, int no, const char *desp, const char *type,
                       long len);
void send_file(int cfd, const char *filename);
void send_dir(int cfd, const char *dirname);
void encode_str(char *to, int tosize, const char *from);
void decode_str(char *to, char *from);
const char *get_file_type(const char *name);