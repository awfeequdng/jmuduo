#include "EventLoop.h"
#include "Thread.h"
#include "CurrentThread.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

using namespace jmuduo;
using namespace jmuduo::net;

void threadFunc()
{
	printf("pid = %d ; tid = %d\n",static_cast<int>(getpid()),CurrentThread::tid());
	EventLoop loop;
	loop.loop();
}

int main()
{
	printf("pid = %d ; tid = %d\n",static_cast<int>(getpid()),CurrentThread::tid());
	EventLoop loop;

	Thread thread(threadFunc);
	thread.start();
	loop.loop();
	thread.join();
	return 0;
}