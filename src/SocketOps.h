#ifndef __SOCKETOPS_H
#define __SOCKETOPS_H
#include <stdint.h>
#include <arpa/inet.h>

namespace jmuduo
{

namespace net
{

namespace socket
{

int createSocketNonblocking();

void bindSocket(int sockfd,const struct sockaddr_in* ip);

void listenSocket(int sockfd);

int accept(int sockfd,struct sockaddr_in* addr);

int connect(int sockfd,struct sockaddr_in* addr);

ssize_t read(int sockfd,void* buf,size_t count);

ssize_t readv(int sockfd,const struct iovec* iov,int iovcnt);

ssize_t write(int sockfd,void* buf,size_t count);

void close(int sockfd);

void shutdownWrite(int sockfd);

void fromIpPort(const char* ip,uint16_t port,struct sockaddr_in* addr);

void toIp(const void* ip,char* dst,size_t size);

 struct sockaddr_in getLocalAddr(int sockfd);


}


}

}

#endif