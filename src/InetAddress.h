#ifndef __INETADDRESS_H
#define __INETADDRESS_H
#include <netinet/in.h>
#include <stdint.h>
#include <string>
namespace jmuduo
{

namespace net
{

class InetAddress
{
	public:
		explicit InetAddress(const struct sockaddr_in& addr):
			addr_(addr)
		{
		}

		InetAddress(const char* ip="0.0.0.0",uint16_t port=0);

		explicit InetAddress(uint16_t port,bool loopbackonly = false);
		const struct sockaddr_in* getSockAddr()const {return &addr_;}

		short family() const {return addr_.sin_family;}   //short == sa_family_t

		uint16_t toPort() const;

		std::string toIp() const;
		void setInetAddress(const struct sockaddr_in& addr) { addr_ = addr;}
	private:
		struct sockaddr_in addr_;
};

}


}

#endif