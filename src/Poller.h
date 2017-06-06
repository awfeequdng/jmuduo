#ifndef __POLLER_H
#define __POLLER_H

#include "Timestamp.h"
#include "EventLoop.h"
#include <poll.h>
#include <vector>
#include <map>

namespace jmuduo
{

namespace net
{

class Channel;



class Poller
{

	public:
		typedef std::vector<Channel*> ChannelList;
		explicit Poller(EventLoop* loop):
			ownerloop_(loop)
		{
		}

		~Poller()
		{
		}

		Timestamp poll(int timeoutMs,ChannelList* activeChannels);

		void updateChannel(Channel* channel);

		void abortNotInLoopThread()
		{
			ownerloop_->abortNotInLoopThread();
		}
		void removeChannel(Channel* channel);
	private:
		
		typedef std::vector<struct pollfd> PollFdList;
		typedef std::map<int,Channel*> ChannelMap;
		
		EventLoop* ownerloop_;
		PollFdList pollfds_;
		ChannelMap channels_;

		void fillActiveChannels(int numEvents,ChannelList* activeChannels);
};



}

}



#endif