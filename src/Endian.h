#ifndef __ENDIAN_H
#define __ENDIAN_H


#include <stdint.h>
#include <endian.h>

namespace jmuduo
{

namespace net
{

namespace socket
{


inline uint64_t hostToNetwork64(uint64_t host)
{
	return htobe64(host);
}

inline uint32_t hostToNetwork32(uint32_t host)
{
	return htobe32(host);
}

inline uint16_t hostToNetwork16(uint16_t host)
{
	return htobe16(host);
}

inline uint64_t networkToHost64(uint64_t network)
{
	return be64toh(network);
}

inline uint32_t networkToHost32(uint32_t network)
{
	return be32toh(network);
}

inline uint16_t networkToHost16(uint16_t network)
{
	return be16toh(network);
}

}


}


}



#endif