#include <kxnet/base/BlockingQueue.h>
#include <kxnet/base/CountDownLatch.h>
#include <kxnet/base/Thread.h>
#include <kxnet/base/Timestamp.h>

#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>

using std::placeholders::_1;

class Bench
{
 public:
  Bench(int numThreads)
    : latch_(numThreads),
      threads_()
  {
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.emplace_back(new kxnet::Thread(
            std::bind(&Bench::threadFunc, this), kxnet::string(name)));
    }
    for (auto& thr : threads_)
    {
      thr->start();
    }
  }

  void run(int times)
  {
    printf("waiting for count down latch\n");
    latch_.wait();
    printf("all threads started\n");
    for (int i = 0; i < times; ++i)
    {
      kxnet::Timestamp now(kxnet::Timestamp::now());
      queue_.put(now);
      usleep(1000);
    }
  }

  void joinAll()
  {
    for (size_t i = 0; i < threads_.size(); ++i)
    {
      queue_.put(kxnet::Timestamp::invalid());
    }

    for (auto& thr : threads_)
    {
      thr->join();
    }
  }

 private:

  void threadFunc()
  {
    printf("tid=%d, %s started\n",
           kxnet::CurrentThread::tid(),
           kxnet::CurrentThread::name());

    std::map<int, int> delays;
    latch_.countDown();
    bool running = true;
    while (running)
    {
      kxnet::Timestamp t(queue_.take());
      kxnet::Timestamp now(kxnet::Timestamp::now());
      if (t.valid())
      {
        int delay = static_cast<int>(timeDifference(now, t) * 1000000);
        printf("tid=%d, latency = %d us\n",
               kxnet::CurrentThread::tid(), delay);
        ++delays[delay];
      }
      running = t.valid();
    }

    printf("tid=%d, %s stopped\n",
           kxnet::CurrentThread::tid(),
           kxnet::CurrentThread::name());
    for (std::map<int, int>::iterator it = delays.begin();
        it != delays.end(); ++it)
    {
      printf("tid = %d, delay = %d, count = %d\n",
             kxnet::CurrentThread::tid(),
             it->first, it->second);
    }
  }

  kxnet::BlockingQueue<kxnet::Timestamp> queue_;
  kxnet::CountDownLatch latch_;
  std::vector<std::unique_ptr<kxnet::Thread>> threads_;
};

int main(int argc, char* argv[])
{
  int threads = argc > 1 ? atoi(argv[1]) : 1;

  Bench t(threads);
  t.run(10000);
  t.joinAll();
}
