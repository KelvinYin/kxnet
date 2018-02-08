#include <kxnet/base/ThreadPool.h>
#include <kxnet/base/CountDownLatch.h>
#include <kxnet/base/CurrentThread.h>
#include <kxnet/base/Logging.h>

#include <stdio.h>
#include <unistd.h>

void print()
{
  printf("tid=%d\n", kxnet::CurrentThread::tid());
}

void printString(const std::string& str)
{
  LOG_INFO << str;
  usleep(100*1000);
}

void test(int maxSize)
{
  LOG_WARN << "Test ThreadPool with max queue size = " << maxSize;
  kxnet::ThreadPool pool("MainThreadPool");
  pool.setMaxQueueSize(maxSize);
  pool.start(5);

  LOG_WARN << "Adding";
  pool.run(print);
  pool.run(print);
  for (int i = 0; i < 100; ++i)
  {
    char buf[32];
    snprintf(buf, sizeof buf, "task %d", i);
    pool.run(std::bind(printString, std::string(buf)));
  }
  LOG_WARN << "Done";

  kxnet::CountDownLatch latch(1);
  pool.run(std::bind(&kxnet::CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}

int main()
{
  test(0);
  test(1);
  test(5);
  test(10);
  test(50);
}
