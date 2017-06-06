#include "TcpServer.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Callback.h"

#include <string>
#include <stdio.h>
#include <boost/bind.hpp>
#include <unistd.h>

using namespace jmuduo;
using namespace jmuduo::net;



class EchoServer
{
	public:
		EchoServer(EventLoop* loop,const InetAddress& inetAddress,const std::string& name):
			serv(loop,inetAddress,name),
			message1(100,'A'),
			message2(200,'B')
		{
			serv.setConnectionCallback(boost::bind(&EchoServer::onConnection,this,_1));
		}
		void start()
		{
			serv.start();
		}

		void onConnection(const TcpConnectionPtr& conn)
		{
			if(conn->connected())
			{
				printf("connection[%s] is up\n",conn->name().c_str());
				conn->send(message1.data(),message1.size());
				conn->send(message2.data(),message2.size());
				conn->shutdown();
			}
			else
			{
				printf("connection[%s] is down\n",conn->name().c_str());
			}
		}
	private:
		TcpServer serv;
		std::string message1;
		std::string message2;
};

int main()
{
	EventLoop loop;
	InetAddress inetAddr(8888);
	EchoServer echoServer(&loop,inetAddr,"echoServer");
	echoServer.start();
	loop.loop();

}