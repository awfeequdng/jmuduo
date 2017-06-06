#ifndef __TCPSERVER_H
#define __TCPSERVER_H
#include "Atomic.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

#include "Callback.h"
#include "TcpConnection.h"
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
namespace jmuduo
{

namespace net
{


class InetAddress;
class EventLoop;

class TcpServer
{

	public:
		TcpServer(EventLoop* loop,const InetAddress& inetAddress,const std::string& name);
		void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb;}
		void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb;}
		void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb;}
		void setHighWaterMarkCallback(const HighWaterMarkCallback& cb) { highWaterMarkCallback_ = cb;}
		void setNumThread(int numThread) { threadPool_->setNumThread(numThread);}
		void start();
	private:
		typedef std::map<std::string,TcpConnectionPtr> ConnectionMap;

		EventLoop* loop_;
		boost::scoped_ptr<Acceptor> acceptor_;
		boost::scoped_ptr<EventLoopThreadPool> threadPool_;
		int nextConnId_;   //always in loop thread
		ConnectionMap connections_;
		AtomicInt32 started_;
		const std::string name_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		WriteCompleteCallback writeCompleteCallback_;
		HighWaterMarkCallback highWaterMarkCallback_;

		void newConnection(int sockfd,const InetAddress& addr);
		void removeConnection(const TcpConnectionPtr& conn);
		void removeConnectionInLoop(const TcpConnectionPtr& conn);
};


}


}


#endif