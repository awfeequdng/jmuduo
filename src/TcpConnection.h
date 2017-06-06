#ifndef __TCPCONNECTION_H
#define __TCPCONNECTION_H

#include <string>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include "InetAddress.h"
#include "Callback.h"
#include "Timestamp.h"
#include "Buffer.h"
namespace jmuduo
{

namespace net
{

class EventLoop;
class Channel;
class Socket;

class TcpConnection:public boost::enable_shared_from_this<TcpConnection>
{
	public:
		TcpConnection(EventLoop* loop,std::string name,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr);

		void setConnectionCallback(const ConnectionCallback& cb ){connectionCallback_ = cb;}

		void setMessageCallback(const MessageCallback& cb) {messageCallback_ = cb;}

		void setCloseCallback(const CloseCallback& cb) {closeCallback_ = cb;}

		void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb;}

		void setHighWaterMarkCallback(const HighWaterMarkCallback& cb) { highWaterMarkCallback_ = cb;}

		void send(const void* message,int len);
		void shutdown();
		void setTcpNoDelay(bool on);


		void connectEstablished();
		void connectDestroy();
			
		bool connected() const { return state_==kConnected;}


		std::string name()const {return name_;}
		EventLoop* getLoop()const {return loop_;}
		void removeChannel();

	private:
		enum StateE {kConnecting,kConnected,kDisconnected,kDisconnecting};

		void setState(StateE s) { state_ = s;}

		void handleRead(Timestamp receiveTime);
		void handleWrite();
		void handleClose();
		void handleError();

		void sendInLoop(const void* message,int len);
		void shutdownInLoop();

		EventLoop* loop_;
		std::string name_;

		StateE state_;
		boost::scoped_ptr<Socket> socket_;
		boost::scoped_ptr<Channel> channel_;
		InetAddress localAddr_;
		InetAddress peerAddr_;
		size_t highWaterMark_;
		Buffer inputBuffer_;
		Buffer outputBuffer_;
		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
		CloseCallback closeCallback_;
		WriteCompleteCallback writeCompleteCallback_;
		HighWaterMarkCallback highWaterMarkCallback_;
};


}


}



#endif