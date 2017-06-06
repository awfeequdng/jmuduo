#include "TcpServer.h"

#include "InetAddress.h"
#include "EventLoop.h"
#include "SocketOps.h"

#include <assert.h>
#include <boost/bind.hpp>
#include <stdio.h>

using namespace jmuduo;
using namespace jmuduo::net;

TcpServer::TcpServer(EventLoop* loop,const InetAddress& inetAddress,const std::string& name):
	loop_(loop),
	acceptor_(new Acceptor(loop,inetAddress)),
	threadPool_(new EventLoopThreadPool(loop)),
	nextConnId_(1),
	name_(name),
	connectionCallback_(defaultConnectionCallback),
	messageCallback_(defaultMessageCallback)

{
	acceptor_->setNewConnectionCallback(boost::bind(&TcpServer::newConnection,this,_1,_2));
}

void TcpServer::start()
{
	if(started_.getAndSet(1)==0)
	{
		assert(!acceptor_->listenning());
		loop_->runInLoop(boost::bind(&Acceptor::listen,get_pointer(acceptor_)));
		threadPool_->start();
	}
	
}

void TcpServer::newConnection(int sockfd,const InetAddress& addr)
{
	loop_->abortNotInLoopThread();
	char buf[32];
	snprintf(buf,sizeof(buf),"#%d",nextConnId_);
	++nextConnId_;
	std::string connName = name_+buf;
	InetAddress localAddr(socket::getLocalAddr(sockfd));

	EventLoop* ioLoop = threadPool_->getNextLoop();
	TcpConnectionPtr conn(new TcpConnection(ioLoop,connName,sockfd,localAddr,addr));

	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->setCloseCallback(boost::bind(&TcpServer::removeConnection,this,_1));
	//conn->setWriteCompleteCallback(boost::bind(writeCompleteCallback_,_1));
	//conn->setHighWaterMarkCallback(boost::bind(highWaterMarkCallback_,_1,_2));
	ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished,conn));


}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
	
	loop_->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop,this,conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	loop_->abortNotInLoopThread();
	connections_.erase(conn->name());
	EventLoop* ioLoop = conn->getLoop();
	ioLoop->runInLoop(boost::bind(&TcpConnection::connectDestroy,conn));
}