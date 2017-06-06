#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketOps.h"

#include <assert.h>
#include <boost/bind.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
using namespace jmuduo;
using namespace jmuduo::net;

Acceptor::Acceptor(EventLoop* loop,const InetAddress& inetAddress):
	loop_(loop),
	acceptSocket_(),
	acceptChannel_(loop,acceptSocket_.fd()),
	listenning_(false),
	idleFd_(::open("/dev/null",O_RDONLY | O_CLOEXEC))
{
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.bindAddress(inetAddress);
	acceptChannel_.setReadCallback(boost::bind(&Acceptor::handleRead,this));
	
}
void Acceptor::listen()
{
	loop_->abortNotInLoopThread();
	listenning_ = true;
	acceptSocket_.listen();
	acceptChannel_.enableReading();
}
Acceptor::~Acceptor()
{
	
	acceptChannel_.disableAll();
	acceptChannel_.remove();
}

void Acceptor::handleRead()
{
	loop_->abortNotInLoopThread();
	InetAddress addr;

	int connd = acceptSocket_.accept(&addr);
	if(connd >= 0)
	{
		if(newConnectionCallback_)
		{
			newConnectionCallback_(connd,addr);
		}
		else
		{
			socket::close(connd);
		}
	}
	else
	{
		if(errno == EMFILE)
		{
			socket::close(idleFd_);
			idleFd_ = ::accept(acceptSocket_.fd(),NULL,NULL);
			socket::close(idleFd_);
			idleFd_ = ::open("/dev/null",O_RDONLY | O_CLOEXEC);
		}
		else
		{
			
		}
	}

}