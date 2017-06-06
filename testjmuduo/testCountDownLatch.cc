#include "CountDownLatch.h"

#include "Thread.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <boost/bind.hpp>
using namespace jmuduo;

CountDownLatch countdownlatch(2);

void waitThread()
{
	countdownlatch.wait();
	printf("countdown!\n");
}

void countdownThread()
{
	countdownlatch.countdown();
	printf("count:%d\n",countdownlatch.getCount());
}

int main()
{
	

	Thread thread_1(waitThread);

	Thread thread_2(countdownThread);
	Thread thread_3(countdownThread);

	thread_1.start();
	sleep(1);
	thread_2.start();
	sleep(1);
	
	thread_3.start();
	
	thread_1.join();
	return 0;
	

}