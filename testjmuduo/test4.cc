#include <stdio.h>
#include "EventLoop.h"
#include "Thread.h"
using namespace jmuduo;
using namespace jmuduo::net;

EventLoop* g_loop;

void print()
{
	printf("hello\n");
}

void threadFunc()
{
	g_loop->runAfter(5.0,print);
}

int main()
{
	EventLoop loop;
	g_loop = &loop;
	Thread t1(threadFunc);
	t1.start();
	loop.loop();


}