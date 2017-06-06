#include "BoundedBlockingQueue.h"

using namespace jmuduo;

template<class T>
void BoundedBlockingQueue<T>::put(T x)
{
	MutexLockGuard lock(mutex_);
	while(deque_.full())
	{
		notFull_.wait();
	}
	deque_.put_back(x);
	notEmpty_.notify();
}

template<class T>
T BoundedBlockingQueue<T>::take()
{
	MutexLockGuard lock(mutex_);
	while(deque_.empty())
	{
		notEmpty_.wait();
	}
	T res=deque_.front();
	deque_.pop_front();
	notFull_.notify();
	return res;

}

template<class T>
bool BoundedBlockingQueue<T>::empty() const
{
	MutexLockGuard lock(mutex_);
	return deque_.empty();
}
template<class T>
bool BoundedBlockingQueue<T>::full() const
{
	MutexLockGuard lock(mutex_);
	return deque_.full();
}
template<class T>
size_t BoundedBlockingQueue<T>::size() const
{
	MutexLockGuard lock(mutex_);
	return deque_.size();
}
template<class T>
size_t BoundedBlockingQueue<T>::capacity() const
{
	MutexLockGuard lock(mutex_);
	return deque_.capacity();
}