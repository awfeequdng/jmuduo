#include "SocketOps.h"
#include "Endian.h"
#include "Types.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/uio.h>
#include <assert.h>
#include <arpa/inet.h>
#include <strings.h> //bzero
using namespace jmuduo;
using namespace jmuduo::net;


int socket::createSocketNonblocking()
{
	int sock =::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,0);
	return sock;
}

void socket::bindSocket(int sockfd,const struct sockaddr_in* ip)
{
	
	int ret = ::bind(sockfd,static_cast<const struct sockaddr*>(implicit_cast<const void*>(ip)),sizeof(struct sockaddr_in));
	assert(ret==0);
	(void)ret;
}

void socket::listenSocket(int sockfd)
{
	int ret = ::listen(sockfd,SOMAXCONN);
	assert(ret==0);
	(void)ret;
}

int socket::accept(int sockfd,struct sockaddr_in* addr)
{
	socklen_t len = static_cast<socklen_t>(sizeof(addr));
	int ret = ::accept4(sockfd,static_cast<struct sockaddr*>(implicit_cast<void*>(addr)),&len,SOCK_NONBLOCK|SOCK_CLOEXEC);
	//FIXME CHECK ret
	return ret;
}

int socket::connect(int sockfd,struct sockaddr_in* addr)
{
	return ::connect(sockfd,static_cast<struct sockaddr*>(implicit_cast<void*>(addr)),static_cast<socklen_t>(sizeof(addr)));
}

ssize_t socket::read(int sockfd,void* buf,size_t count)
{
	return ::read(sockfd,buf,count);
}

ssize_t socket::readv(int sockfd,const struct iovec* iov,int iovcnt)
{
	return ::readv(sockfd,iov,iovcnt);
}

ssize_t socket::write(int sockfd,void* buf,size_t count)
{
	return ::write(sockfd,buf,count);
}

void socket::close(int sockfd)
{
	::close(sockfd);
}

void socket::shutdownWrite(int sockfd)
{
	::shutdown(sockfd,SHUT_WR);
}

void socket::fromIpPort(const char* ip,uint16_t port,struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	//FIXME check return value
	inet_pton(AF_INET,ip,&addr->sin_addr);

}

void socket::toIp(const void* ip,char* dst,size_t size)
{
	::inet_ntop(AF_INET,ip,dst,static_cast<socklen_t>(size));
}

struct sockaddr_in socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr));
	::getsockname(sockfd,static_cast<struct sockaddr*>(implicit_cast<void*>(&addr)),&len);
	return addr;
}