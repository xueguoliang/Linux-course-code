
#ifndef MYNETWORK_H
#define MYNETWORK_H

#include "header.h"

int myServer(uint16_t port, const char* ip);
int myAccept(int server, struct sockaddr* addr, socklen_t* addrlen);
int myRead(int fd, char buf[], int len);
int myWrite(int fd, const char buf[], int len);
int myRead2(int fd, char buf[], int len);
int myConnect(int fd, uint16_t port, const char* ip);
int myConnect2(uint16_t port, const char* ip);
int myWriteLen(int fd, uint32_t len);
int myReadLen(int fd, uint32_t* len);
int myFileSize(const char* path);
char* myReadBuf(int fd);
void myWriteBuf(int fd, const char* buf);

#endif
