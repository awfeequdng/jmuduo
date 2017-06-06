#ifndef __BUFFER_H
#define __BUFFER_H

#include <vector>
#include <assert.h>
#include <stdint.h>
#include <string>
#include <algorithm> //std::copy
#include <string.h> //memcpy

#include "Endian.h"
#include "SocketOps.h"
#include <stdio.h>
namespace jmuduo
{

namespace net
{

class Buffer
{
	public:
		static const size_t kCheapPrepend = 8;
		static const size_t kInitialSize = 1024;
 		Buffer(int initialSize = kInitialSize):
			buffer_(kCheapPrepend+initialSize),
			readerIndex_(kCheapPrepend),
			writerIndex_(kCheapPrepend)
		{
			
		}

		size_t prependableBytes() const
		{ return readerIndex_;}

		size_t readableBytes() const
		{ return writerIndex_ - readerIndex_;}

		size_t writableBytes() const
		{ return buffer_.size() - writerIndex_; }
		
		char* begin()
		{ return &*buffer_.begin();}

		const char* begin() const
		{ return &*buffer_.begin();}

		const char* peek() const
		{ return begin() + readerIndex_;}
		
		char* beginWrite()
		{ return begin() + writerIndex_;}

		const char* beginWrite() const
		{ return begin() + writerIndex_;}

		

		void retrieveAll()
		{
			readerIndex_ =  kCheapPrepend;
			writerIndex_ = kCheapPrepend;
		}

		void retrieve(size_t len)
		{
			if(len < readableBytes())
			{
				readerIndex_ += len;
			}
			else
			{
				retrieveAll();
			}
		}
		
		void retrieveUntil(const char* end)
		{
			assert(peek() <= end);
			assert(end <= beginWrite());
			retrieve(end - peek());		
		}

		void retrieveInt64()
		{
			retrieve(sizeof(int64_t));
		}

		void retrieveInt32()
		{
			retrieve(sizeof(int32_t));
		}

		void retrieveInt16()
		{
			retrieve(sizeof(int16_t));
		}
		
		void retrieveInt8()
		{
			retrieve(sizeof(int8_t));
		}
		
		std::string retrieveAsString(size_t len)
		{
			assert(len <= readableBytes());
			std::string result(peek(),len);
			retrieve(len);
			return result;
		}

		std::string retrieveAllAsString()
		{
			//retrieveAsString(readableBytes()); 忘记返回值
			return retrieveAsString(readableBytes());
		}
		
		void append(const char* data,size_t len)
		{
			ensureWritable(len);
			std::copy(data,data+len,beginWrite());
			hasWritten(len);			
		}

		void append(const void* data,size_t len)
		{
			append(static_cast<const char*>(data),len);
		}

		void appendInt64(int64_t x)
		{
			int64_t be64 = socket::hostToNetwork64(x);
			append(&be64,sizeof(be64));
		}

		void appendInt32(int32_t x)
		{
			int32_t be32 = socket::hostToNetwork32(x);
			append(&be32,sizeof(be32));
		}


		void appendInt16(int16_t x)
		{
			int16_t be16 = socket::hostToNetwork16(x);
			append(&be16,sizeof(be16));
		}

		void appendInt8(int8_t x)  //only one byte need not to be transform to Network from Host
		{
			append(&x,sizeof(x));
		}

		void ensureWritable(size_t len)
		{
			if(writableBytes() < len)
			{
				makeSpace(len);
			}
		}

		void makeSpace(size_t len)
		{
			printf("makespace()?\n");
			if(prependableBytes() + writableBytes() >= len + kCheapPrepend)
			{
				size_t readable = readableBytes();
				std::copy(begin()+readerIndex_, begin()+writerIndex_,begin()+kCheapPrepend);
				readerIndex_ = kCheapPrepend;
				writerIndex_ = readerIndex_ + readable ; 
			}
			else
			{
				printf("resize()?\n");
				buffer_.resize(writerIndex_+len);
			}
		}

		void hasWritten(size_t len)
		{
			assert( writableBytes() >= len);
			writerIndex_ += len;
		}
		
		int64_t peekInt64() const
		{
			assert(readableBytes() >= sizeof(int64_t));
			int64_t be64 = 0;
			::memcpy(&be64,peek(),sizeof(int64_t));
			return socket::networkToHost64(be64);

		}
		
		int32_t peekInt32() const
		{
			assert(readableBytes() >= sizeof(int32_t));
			int32_t be32 = 0;
			::memcpy(&be32,peek(),sizeof(int32_t));
			return socket::networkToHost32(be32);

		}

		int16_t peekInt16() const
		{
			assert(readableBytes() >= sizeof(int16_t));
			int16_t be16 = 0;
			::memcpy(&be16,peek(),sizeof(int16_t));
			return socket::networkToHost16(be16);

		}

		int8_t peekInt8() const
		{
			assert(readableBytes() >= sizeof(int8_t));
			int8_t be8 = *peek();
			return be8;

		}

		int64_t readInt64()
		{
			int64_t result = peekInt64();
			retrieveInt64();
			return result;
		}
		
		int32_t readInt32()
		{
			int32_t result = peekInt32();
			retrieveInt32();
			return result;
		}

		int16_t readInt16()
		{
			int16_t result = peekInt16();
			retrieveInt16();
			return result;
		}

		int8_t readInt8()
		{
			int64_t result = peekInt8();
			retrieveInt8();
			return result;
		}

		void prepend(const void* data,size_t len)
		{
			assert(len < prependableBytes());
			readerIndex_ -= len;
			const char* d = static_cast<const char*>(data);
			std::copy(d,d+len,begin()+readerIndex_);
		}

		void prependInt64(int64_t x)
		{
			int64_t be64 = socket::hostToNetwork64(x);
			prepend(&be64,sizeof(int64_t));
		}

		void prependInt32(int32_t x)
		{
			int32_t be32 = socket::hostToNetwork32(x);
			prepend(&be32,sizeof(int32_t));
		}

		void prependInt16(int16_t x)
		{
			int16_t be16 = socket::hostToNetwork16(x);
			prepend(&be16,sizeof(int16_t));
		}

		void prependInt8(int8_t x)
		{
			
			prepend(&x,sizeof(int8_t));
		}
		void printBufferInfo()
		{
			printf("%zd  %zd\n",buffer_.size(),buffer_.capacity());
			printf("%zd  %zd\n",readerIndex_,writerIndex_);
		}
		ssize_t readFd(int fd,int* savedErrno);
	private:
		std::vector<char> buffer_;
		size_t readerIndex_;
		size_t writerIndex_;
		
		static const char kCRLF[];
};

}

}




#endif