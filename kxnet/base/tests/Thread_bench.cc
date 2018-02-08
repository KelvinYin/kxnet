#include <kxnet/base/CurrentThread.h>
#include <kxnet/base/Mutex.h>
#include <kxnet/base/Thread.h>
#include <kxnet/base/Timestamp.h>

#include <map>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

kxnet::MutexLock g_mutex;
std::map<int, int> g_delays;

void threadFunc()
{
  //printf("tid=%d\n", kxnet::CurrentThread::tid());
}

void threadFunc2(kxnet::Timestamp start)
{
  kxnet::Timestamp now(kxnet::Timestamp::now());
  int delay = static_cast<int>(timeDifference(now, start) * 1000000);
  kxnet::MutexLockGuard lock(g_mutex);
  ++g_delays[delay];
}

void forkBench()
{
  sleep(10);
  kxnet::Timestamp start(kxnet::Timestamp::now());
  int kProcesses = 10*1000;

  for (int i = 0; i < kProcesses; ++i)
  {
    pid_t child = fork();
    if (child == 0)
    {
      exit(0);
    }
    else
    {
      waitpid(child, NULL, 0);
    }
  }

  double timeUsed = timeDifference(kxnet::Timestamp::now(), start);
  printf("process creation time used %f us\n", timeUsed*1000000/kProcesses);
  printf("number of created processes %d\n", kProcesses);
}

int main(int argc, char* argv[])
{
  printf("pid=%d, tid=%d\n", ::getpid(), kxnet::CurrentThread::tid());
  kxnet::Timestamp start(kxnet::Timestamp::now());

  int kThreads = 100*1000;
  for (int i = 0; i < kThreads; ++i)
  {
    kxnet::Thread t1(threadFunc);
    t1.start();
    t1.join();
  }

  double timeUsed = timeDifference(kxnet::Timestamp::now(), start);
  printf("thread creation time %f us\n", timeUsed*1000000/kThreads);
  printf("number of created threads %d\n", kxnet::Thread::numCreated());

  for (int i = 0; i < kThreads; ++i)
  {
    kxnet::Timestamp now(kxnet::Timestamp::now());
    kxnet::Thread t2(std::bind(threadFunc2, now));
    t2.start();
    t2.join();
  }
  {
    kxnet::MutexLockGuard lock(g_mutex);
    for (std::map<int, int>::iterator it = g_delays.begin();
        it != g_delays.end(); ++it)
    {
      printf("delay = %d, count = %d\n",
             it->first, it->second);
    }
  }

  forkBench();
}
