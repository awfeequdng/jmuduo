#include "InetAddress.h"
#include "SocketOps.h"
#include "Endian.h"

#include <strings.h>  //bzero
using namespace jmuduo;
using namespace jmuduo::net;

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;


InetAddress::InetAddress(const char* ip,uint16_t port)
{
	::bzero(&addr_,sizeof(addr_));
	socket::fromIpPort(ip,port,&addr_);
}
InetAddress::InetAddress(uint16_t port,bool loopbackonly)
{
	::bzero(&addr_,sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = socket::hostToNetwork16(port);
	in_addr_t ip = loopbackonly ? kInaddrAny:kInaddrLoopback;
	addr_.sin_addr.s_addr = socket::hostToNetwork32(ip);
}

uint16_t InetAddress::toPort() const
{
	return socket::networkToHost16(addr_.sin_port);
}

std::string InetAddress::toIp() const
{
	
	char buf[32]="";
	socket::toIp(&addr_.sin_addr,buf,sizeof(buf));
	return buf;
}
