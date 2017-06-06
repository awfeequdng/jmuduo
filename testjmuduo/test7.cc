#include "InetAddress.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketOps.h"

#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace jmuduo;
using namespace jmuduo::net;
void newConnection(int sockfd,const InetAddress& addr)
{
	printf("receive from %s:%d\n",addr.toIp().c_str(),addr.toPort());
	::write(sockfd,"hello\n",7);
	socket::close(sockfd);
}

int main()
{
	EventLoop loop;
	InetAddress inetAddress(8888);
	Acceptor acceptor(&loop,inetAddress);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();
	loop.loop();


}