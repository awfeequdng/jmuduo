#include "Buffer.h"
#include "Types.h"
#include <sys/uio.h>
#include <errno.h>
using namespace jmuduo;
using namespace jmuduo::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

ssize_t Buffer::readFd(int fd,int* savedErrno)
{
	char buf[65536];
	struct iovec iov[2];
	size_t writable = writableBytes();
	iov[0].iov_base = begin()+writerIndex_;
	iov[0].iov_len = writable;

	iov[1].iov_base = buf;
	iov[1].iov_len = sizeof(buf);
	
	int iovcnt = writable > sizeof(buf) ? 1:2;
	ssize_t n = socket::readv(fd,iov,iovcnt);
	
	if(n < 0)
	{
		*savedErrno = errno;
	}
	else if(implicit_cast<size_t>(n) <= writable)
	{
		writerIndex_ += n ;
	}
	else
	{
		
		writerIndex_ = buffer_.size();
		append(buf,n-writable);
	}
	
	
	return n;

}