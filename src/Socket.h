#ifndef __SOCKET_H
#define __SOCKET_H

namespace jmuduo
{

namespace net
{
class InetAddress;

class Socket
{
	public:
		Socket();
		explicit Socket(int fd);
		~Socket();

		int fd() const {return fd_;}

		void bindAddress(const  InetAddress& localaddr);

		void listen();

		int accept(InetAddress* peeraddr);
		void shutdownWrite();

		void setReuseAddr(bool on);

		void setReusePort(bool on);

		void setKeepAlive(bool on);

		void setNoDelay(bool on);

		
	private:
		const int fd_;

};


}


}


#endif