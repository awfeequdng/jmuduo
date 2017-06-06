#ifndef __EVENTLOOP_H
#define __EVENTLOOP_H
#include "CurrentThread.h"
#include "TimerId.h"
#include "Timestamp.h"
#include "Callback.h"
#include "Mutex.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

namespace jmuduo
{
namespace net
{
class Channel;
class Poller;
class TimerQueue;

class EventLoop
{
	public:
		typedef std::vector<Channel*> ChannelList;
		typedef boost::function<void()> Functor;
		EventLoop();
		~EventLoop();
		void loop();
		bool isInLoopThread() const
		{
			return threadId_ == CurrentThread::tid();
		}
		void abortNotInLoopThread()
		{
			if(!isInLoopThread())
			{
				printf("EventLoop Not In LoopThread\n");
				exit(-1);
			}
		}
		void quit();
		void runInLoop(const Functor& cb);
		void updateChannel(Channel* channel);
		void removeChannel(Channel* channel);
		TimerId runAt(const Timestamp& time,const TimerCallback& cb);
		TimerId runAfter(double delay,const TimerCallback& cb);
		TimerId runEvery(double interval,const TimerCallback& cb);

		void queueInLoop(const Functor& cb);
	private:
		bool looping_;
		bool quit_;

		const pid_t threadId_;
		ChannelList activeChannels_;
		boost::scoped_ptr<Poller> poller_;
		boost::scoped_ptr<TimerQueue> timerQueue_;
		
		bool callingPendingFunctor_;
		MutexLock mutex_;
		std::vector<Functor> pendingFunctors_;
		int wakeupFd_;
		boost::scoped_ptr<Channel> wakeChannel_;

		
		void handleRead(Timestamp receiveTime);
		void wakeup();
		void doPendingFunctors_();
};

}

}

#endif