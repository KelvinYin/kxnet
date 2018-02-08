#pragma once
#include <kxnet/base/BlockingQueue.h>
#include <kxnet/base/BoundedBlockingQueue.h>
#include <kxnet/base/CountDownLatch.h>
#include <kxnet/base/Mutex.h>
#include <kxnet/base/Thread.h>
#include <kxnet/base/LogStream.h>

#include <vector>

namespace kxnet
{

class AsyncLogging : noncopyable
{
 public:

  AsyncLogging(const string& basename,
               size_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (running_)
    {
      stop();
    }
  }

  void append(const char* logline, int len);

  void start()
  {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop()
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

 private:

  void threadFunc();

  typedef kxnet::detail::FixedBuffer<kxnet::detail::kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef BufferVector::value_type BufferPtr;

  const int flushInterval_;
  bool   running_;
  string basename_;
  size_t rollSize_;
  kxnet::Thread         thread_;
  kxnet::CountDownLatch latch_;
  kxnet::MutexLock      mutex_;
  kxnet::Condition      cond_;
  BufferPtr    currentBuffer_;
  BufferPtr    nextBuffer_;
  BufferVector buffers_;
};

}

