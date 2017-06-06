#include "TcpConnection.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "InetAddress.h"
#include "Timestamp.h"
#include "Buffer.h"

#include <string>
using namespace jmuduo;
using namespace jmuduo::net;

void onMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp receiveTime)
{
	buf->printBufferInfo();
	printf("onMessage():[%s]\n",buf->retrieveAllAsString().c_str());
	buf->printBufferInfo();
}

int main()
{
	EventLoop loop;
	InetAddress inetAddress(8888);
	std::string name("TcpConnection ");
	TcpServer server(&loop,inetAddress,name);
	//server.setMessageCallback(onMessage);
	server.start();
	loop.loop();




}