#include "BlockingQueue.h"

using namespace jmuduo;

template<class T>
void BlockingQueue<T>::put(T x)
{
	MutexLockGuard lock(mutex_);
	deque_.put_back(x);
	notEmpty_.notifyAll();
}


template<class T>
T BlockingQueue<T>::take()
{
	MutexLockGuard lock(mutex_);
	while(deque_.size()==0)
	{
		notEmpty_.wait();
	}
	T res=deque_.front();
	deque_.pop();
	return res;
}