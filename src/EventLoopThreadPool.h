#ifndef __EVENTLOOPTHREADPOOL_H
#define __EVENTLOOPTHREADPOOL_H


#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

namespace jmuduo
{


namespace net
{
class EventLoopThread;
class EventLoop;

class EventLoopThreadPool
{

	public:
		EventLoopThreadPool(EventLoop* loop);

		~EventLoopThreadPool();

		void setNumThread(int numThreads) { numThreads_ = numThreads;}
		void start();

		EventLoop* getNextLoop();

	private:
		EventLoop* baseloop_;
		bool started_;
		int nextId_;
		int numThreads_;
		std::vector<EventLoop*> loops_;
		boost::ptr_vector<EventLoopThread> threads_;
};



}
	
}



#endif