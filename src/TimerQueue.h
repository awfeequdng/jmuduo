#ifndef __TIMERQUEUE_H
#define __TIMERQUEUE_H
#include "Callback.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Timestamp.h"
#include <vector>
#include <set>

namespace jmuduo
{

namespace net
{

class TimerId;
class Timer;


class TimerQueue
{
	public:
		

		TimerQueue(EventLoop* loop);
		~TimerQueue();
		TimerId addTimer(const TimerCallback& callback,Timestamp when,double interval);

	private:
		typedef std::pair<Timestamp,Timer*> Entry;
		typedef std::set<Entry> TimerList;

		EventLoop *loop_;
		const int timerfd_;
		Channel timerfdChannel_;
		TimerList timers_;
		
		void handleRead(Timestamp receiveTime);
		void addTimerInLoop(Timer *timer);
		std::vector<Entry> getExpired(Timestamp now);
		bool insert(Timer* timer);
		void reset(std::vector<Entry>& expired,Timestamp now);
};



}

}


#endif