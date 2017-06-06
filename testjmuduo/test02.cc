#include "EventLoop.h"
#include "Thread.h"

using namespace jmuduo;
using namespace jmuduo::net;

EventLoop *g_loop;

void threadFunc()
{
	g_loop->loop();
}

int main()
{
	EventLoop loop;
	g_loop = &loop;
	Thread t1(threadFunc);
	t1.start();
	t1.join();
	return 0;
}