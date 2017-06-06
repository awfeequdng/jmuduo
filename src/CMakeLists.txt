#SET(LIB_SRC Atomic.h Timestamp.cc Timestamp.h Thread.h Thread.cc CurrentThread.h Mutex.h Condition.h  CountDownLatch.h CountDownLatch.cc)
SET(LIB_SRC Timestamp.cc Thread.cc CountDownLatch.cc BlockingQueue.cc BoundedBlockingQueue.cc ThreadPool.cc Singleton.cc ThreadLocal.cc EventLoop.cc Poller.cc Channel.cc Timer.cc TimerQueue.cc EventLoopThread.cc SocketOps.cc Socket.cc InetAddress.cc Acceptor.cc TcpConnection.cc TcpServer.cc Buffer.cc EventLoopThreadPool.cc)  

ADD_LIBRARY(jmuduo SHARED ${LIB_SRC})