#ifndef __TYPES_H
#define __TYPES_H

namespace jmuduo
{

template<class To,class From>
inline To implicit_cast(From const &f)
{
	return f;
}




}

#endif