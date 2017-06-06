#include "Socket.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h> //bzero
#include <netinet/tcp.h> //TCP_NODELAY
using namespace jmuduo;
using namespace jmuduo::net;

Socket::Socket():
	fd_(socket::createSocketNonblocking())
{

}

Socket::Socket(int fd):
	fd_(fd)
{

}

Socket::~Socket()
{
	socket::close(fd_);
}

void Socket::bindAddress(const InetAddress& localaddr)
{
	socket::bindSocket(fd_,localaddr.getSockAddr());
}

void Socket::listen()
{
	socket::listenSocket(fd_);
}

int Socket::accept(InetAddress* peeraddr)
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	int connd = socket::accept(fd_,&addr);
	if(connd>0)
	{
		peeraddr->setInetAddress(addr);
	}
	return connd;
}
void Socket::shutdownWrite()
{
	socket::shutdownWrite(fd_);
}

void Socket::setReuseAddr(bool on)
{
	int optval = on? 1 : 0 ;
	::setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&optval,static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on)
{
	int optval = on? 1 : 0 ;
	::setsockopt(fd_,SOL_SOCKET,SO_REUSEPORT,&optval,static_cast<socklen_t>(sizeof(optval)));	
}

void Socket::setKeepAlive(bool on)
{
	int optval = on? 1 : 0 ;
	::setsockopt(fd_,SOL_SOCKET,SO_KEEPALIVE,&optval,static_cast<socklen_t>(sizeof(optval)));
}
void Socket::setNoDelay(bool on)
{
	int optval = on? 1 : 0;
	::setsockopt(fd_,IPPROTO_TCP,TCP_NODELAY,&optval,sizeof(optval));
}