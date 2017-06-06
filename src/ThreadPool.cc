#include "ThreadPool.h"

#include <assert.h>
#include <boost/bind.hpp>
using namespace jmuduo;

ThreadPool::ThreadPool():
	mutex_(),
	notEmpty_(mutex_),
	notFull_(mutex_),
	maxTasks_(0),
	running_(false)
{

}

ThreadPool::~ThreadPool()
{
	if(running_)
	{
		stop();
	}
}

void ThreadPool::start(int numThreads)
{
	assert(threads_.empty());
	running_ = true;

	//Effects: Expands the allocated buffer
	threads_.reserve(numThreads);

	for(int i=0;i<numThreads;++i)
	{
		threads_.push_back(new jmuduo::Thread(boost::bind(&ThreadPool::runInThread,this)));
		threads_[i].start();
	}

}

void ThreadPool::stop()
{
	MutexLockGuard lock(mutex_);
	running_ = false;
	notEmpty_.notifyAll();
	for(boost::ptr_vector<Thread>::iterator threadIterator=threads_.begin();threadIterator!=threads_.end();++threadIterator)
	{
		(*threadIterator).join();
	}
}

void ThreadPool::runInThread()
{
	while(running_)
	{
		Task task(take());
		if(task)
		{
			task();
		}
	}
}

void ThreadPool::run(Task task)
{
	if(threads_.empty())
	{
		task();
	}
	else
	{
		MutexLockGuard lock(mutex_);
		while(isFull())
		{
			notFull_.wait();
		}
		tasks_.push_back(task);
		notEmpty_.notify();
	}
}

ThreadPool::Task ThreadPool::take()
{
	MutexLockGuard lock(mutex_);
	while(isEmpty() && running_==false)
	{
		notEmpty_.wait();
	}
	if(running_)
	{
		Task task = tasks_.front();
		tasks_.pop_front();
		notFull_.notify();
		return task;
	}
	else
	{
		return NULL;
	}
}

bool ThreadPool::isFull()
{
	MutexLockGuard lock(mutex_);
	return maxTasks_ > 0 && tasks_.size() >= maxTasks_;
}

bool ThreadPool::isEmpty()
{
	MutexLockGuard lock(mutex_);
	return tasks_.empty();
}