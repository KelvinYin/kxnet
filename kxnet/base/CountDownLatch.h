#pragma once

#include <kxnet/base/Condition.h>
#include <kxnet/base/Mutex.h>

namespace kxnet
{

class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};

}

