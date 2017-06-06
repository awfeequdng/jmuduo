#ifndef __THREADPOOL_H
#define __THREADPOOL_H

#include <deque>
#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"
namespace jmuduo
{

class ThreadPool
{
	public:
		typedef boost::function<void()> Task;

		ThreadPool();

		~ThreadPool();

		void start(int numThreads);

		void stop();

		void runInThread();

		void run(Task task);

		Task take();

		void setMaxTasks(int maxTasks)
		{
			maxTasks_ = maxTasks;
		}

		bool isFull();

		bool isEmpty();
	private:
		bool running_;
		int maxTasks_;
		MutexLock mutex_;
		Condition notEmpty_;
		Condition notFull_;
		
		
		boost::ptr_vector<jmuduo::Thread> threads_;
		std::deque<Task> tasks_;

	
};


}



#endif