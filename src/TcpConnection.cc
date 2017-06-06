#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Types.h"

#include <unistd.h>
#include <stdio.h>
#include <boost/bind.hpp>
#include <errno.h>

using namespace jmuduo;
using namespace jmuduo::net;


void jmuduo::net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{

}

void jmuduo::net::defaultMessageCallback(const TcpConnectionPtr& conn,Buffer* buf,Timestamp receiveTime)
{
	printf("from %s:%s\n",conn->name().c_str(),buf->retrieveAllAsString().c_str());
	
}


TcpConnection::TcpConnection(EventLoop* loop,std::string name,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr):
	loop_(loop),
	name_(name),
	state_(kConnecting),
	socket_(new Socket(sockfd)),
	channel_(new Channel(loop,socket_->fd())),
	localAddr_(localAddr),
	peerAddr_(peerAddr),
	highWaterMark_(64*1024*1024)

{
	channel_->setReadCallback(boost::bind(&TcpConnection::handleRead,this,_1));
	channel_->setCloseCallback(boost::bind(&TcpConnection::handleClose,this));
	channel_->setErrorCallback(boost::bind(&TcpConnection::handleError,this));
	channel_->setWriteCallback(boost::bind(&TcpConnection::handleWrite,this));
}


void TcpConnection::handleRead(Timestamp receiveTime)
{
	loop_->abortNotInLoopThread();
	
	int saveErrno = 0;
	ssize_t n = inputBuffer_.readFd(channel_->fd(),&saveErrno);
	if(n > 0)
	{
		messageCallback_(shared_from_this(),&inputBuffer_,receiveTime);
	}
	else if(n == 0)
	{
		handleClose();
	}
	else
	{
		handleError();
	}

}

void TcpConnection::connectEstablished()
{
	loop_->abortNotInLoopThread();
	setState(kConnected);
	
	channel_->tie(shared_from_this());
	channel_->enableReading();
	connectionCallback_(shared_from_this());

}

void TcpConnection::handleWrite()
{
	loop_->abortNotInLoopThread();
	if(channel_->isWriting())
	{
		ssize_t n =::write(channel_->fd(),outputBuffer_.peek(),outputBuffer_.readableBytes());

		if(n > 0)
		{
			outputBuffer_.retrieve(n);
			if(outputBuffer_.readableBytes()==0)
			{
				channel_->disableWriting();
				
				if(writeCompleteCallback_!=NULL)
				{
					loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
				}
				if(state_ == kDisconnecting)
				{
					shutdownInLoop();
				}
			}
			else
			{
				//continue write
			}
		}
		else
		{
			//deal with n<=0
		}
	}
	else
	{
		//deal with !channel_->isWriting()
	}
}
void TcpConnection::handleClose()
{
	loop_->abortNotInLoopThread();
	assert(state_ == kConnected || state_ == kDisconnecting);
	setState(kDisconnected);
	channel_->disableAll();
	TcpConnectionPtr guardthis(shared_from_this());
	connectionCallback_(guardthis);
	closeCallback_(guardthis);

	//connectionCallback_(shared_from_this());
	//closeCallback_(shared_from_this());
}
void TcpConnection::handleError()
{

}

void TcpConnection::connectDestroy()
{
	loop_->abortNotInLoopThread();
	if(state_ == kConnected)
	{
		setState(kDisconnected);
		channel_->disableAll();
		connectionCallback_(shared_from_this());

	}
	channel_->remove();
}

void TcpConnection::send(const void* message,int len)
{
	if(state_ == kConnected)
	{
		if(loop_->isInLoopThread())
		{
			sendInLoop(message,len);
		}
		else
		{
			loop_->runInLoop(boost::bind(&TcpConnection::sendInLoop,this,message,len));
		}
	}
}

void TcpConnection::sendInLoop(const void* message,int len)
{
	loop_->abortNotInLoopThread();
	ssize_t nwrote = 0;
	int remaining = len;
	if(!channel_->isWriting() && outputBuffer_.readableBytes()==0)
	{
		nwrote = ::write(channel_->fd(),message,len);
		remaining = len - nwrote;
		if(nwrote>=0)
		{
			if(remaining == 0)
			{
				if(writeCompleteCallback_)
				{
					loop_->queueInLoop(boost::bind(writeCompleteCallback_,shared_from_this()));
				}
			}
			
		}
		else
		{
			if(errno == EPIPE)   //man 2 write , read Error section
			{
				return;
			}
		}
	}
	assert(nwrote>=0);
	if(remaining>0)
	{
		int oldlen = outputBuffer_.readableBytes();
		if(oldlen + remaining >= highWaterMark_ && oldlen < highWaterMark_ && highWaterMarkCallback_)
		{
			loop_->queueInLoop(boost::bind(highWaterMarkCallback_,shared_from_this(),oldlen + remaining));
		}
		outputBuffer_.append(static_cast<const char*>(message)+nwrote,len-nwrote);
		if(!channel_->isWriting())
		{
			channel_->enableWriting();
		}
	}
}

void TcpConnection::shutdown()
{
	if(state_ == kConnected)
	{
		setState(kDisconnecting);
		loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop,this));
	}

}

void TcpConnection::shutdownInLoop()
{
	loop_->abortNotInLoopThread();
	if(!channel_->isWriting())
	{
		socket_->shutdownWrite();
	}
}

void TcpConnection::setTcpNoDelay(bool on)
{
	socket_->setNoDelay(on);
}