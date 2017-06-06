#ifndef __CALLBACK_H
#define __CALLBACK_H
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "Timestamp.h"
namespace jmuduo
{

namespace net
{
	class Buffer;
	class TcpConnection;
	typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef boost::function<void()> TimerCallback;
	
	typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;

	typedef boost::function<void (const TcpConnectionPtr&,Buffer*,Timestamp)> MessageCallback;

	typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;

	typedef boost::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;

	typedef boost::function<void (const TcpConnectionPtr&,int)> HighWaterMarkCallback;

	void defaultConnectionCallback(const TcpConnectionPtr& conn);

	void defaultMessageCallback(const TcpConnectionPtr& conn,Buffer* buf,Timestamp receiveTime);

}


}


#endif