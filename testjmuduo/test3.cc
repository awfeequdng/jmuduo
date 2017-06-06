#include "EventLoop.h"
#include "Channel.h"
#include "Timestamp.h"
#include <sys/timerfd.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>

using namespace jmuduo;

using namespace jmuduo::net;

EventLoop* g_loop;

void timeout(Timestamp receiveTime)
{
	printf("%s Timeout!\n",receiveTime.toFormattedString().c_str());
	g_loop->quit();
}
int main()
{
	printf("%s start!\n",Timestamp::now().toFormattedString().c_str());
	EventLoop loop;
	g_loop = &loop;
	int timerfd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);

	Channel channel(&loop,timerfd);

	struct itimerspec itime;
	bzero(&itime,sizeof(itime));
	itime.it_value.tv_sec = 5;
	timerfd_settime(timerfd,0,&itime,NULL);
	channel.setReadCallback(timeout);
	channel.enableReading();
	loop.loop();
	::close(timerfd);
	return 0;
}


