#include "Poller.h"
#include "Channel.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
using namespace jmuduo;
using namespace jmuduo::net;

Timestamp Poller::poll(int timeoutMs,ChannelList* activeChannels)
{
	int numEvents = ::poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);
	Timestamp now(Timestamp::now());
	if(numEvents == 0)
	{
		printf("Poller::poll()  nothing happen\n");
	}
	else if(numEvents > 0)
	{
		fillActiveChannels(numEvents,activeChannels);
	}
	else
	{
		printf("Poller::poll() error\n");
		exit(-1);
	}
	return now;
}
void Poller::fillActiveChannels(int numEvents,ChannelList* activeChannels)
{
	for(PollFdList::iterator iter = pollfds_.begin();iter!=pollfds_.end()&&numEvents>0;++iter)
	{
		if(iter->revents>0)
		{
			--numEvents;

			ChannelMap::iterator ch = channels_.find(iter->fd);
			assert(ch!=channels_.end());
			Channel* channel = ch->second;
			assert(channel->fd()==iter->fd);

			channel->set_revents(iter->revents);
			
			activeChannels->push_back(channel);
			
		}
	}
}

void Poller::updateChannel(Channel* channel)
{
	abortNotInLoopThread();

	if(channel->index() < 0)   //a new channel
	{
		assert(channels_.find(channel->fd()) == channels_.end());
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;

		pollfds_.push_back(pfd);
		int idx = static_cast<int>(pollfds_.size()) - 1;   
		channel->set_index(idx);
		channels_[pfd.fd]=channel;
	}
	else
	{
		int idx = channel->index();
		struct pollfd& pfd = pollfds_[idx];
		
		pfd.events = static_cast<short>(channel->events());
		pfd.revents = 0;
		if(channel->isNoneEvent())
		{
			pfd.fd=-channel->fd()-1;
		}


	}
}

void Poller::removeChannel(Channel* channel)
{
	assert(channel->isNoneEvent());
	
	channels_.erase(channel->fd());
	
	int idx = channel->index();
	assert(pollfds_[idx].fd == -channel->fd()-1 && pollfds_[idx].events == channel->events());
	if(idx == static_cast<int>(pollfds_.size())-1)
	{
		pollfds_.pop_back();
	}
	else
	{
		int channelfd = pollfds_.back().fd;
		iter_swap(pollfds_.begin()+idx,pollfds_.end()-1);
		if(channelfd<0)
		{
			channelfd=-channelfd-1;
		}
		channels_[channelfd]->set_index(idx);
		pollfds_.pop_back();
	}

}
