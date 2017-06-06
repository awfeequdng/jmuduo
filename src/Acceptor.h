#ifndef __ACCEPT_H
#define __ACCEPT_H
#include <boost/function.hpp>
#include "Socket.h"
#include "Channel.h"
namespace jmuduo
{

namespace net
{

class EventLoop;
class InetAddress;

class Acceptor
{
	public:
		typedef boost::function<void (int sockfd,const InetAddress& addr)> NewConnectionCallback;
		Acceptor(EventLoop* loop,const InetAddress& inetAddress);

		~Acceptor();
		bool listenning() const {return listenning_;}
		void setNewConnectionCallback(NewConnectionCallback newConnectionCallback) {newConnectionCallback_ = newConnectionCallback;}
		void listen();
	private:
		
		EventLoop* loop_;
		Socket acceptSocket_;
		Channel acceptChannel_;
		bool listenning_;
		int idleFd_;
		NewConnectionCallback newConnectionCallback_;
		void handleRead();
};

}


}



#endif