#include <kxnet/base/Singleton.h>
#include <kxnet/base/CurrentThread.h>
#include <kxnet/base/ThreadLocal.h>
#include <kxnet/base/Thread.h>

#include <stdio.h>
#include <unistd.h>

class Test : kxnet::noncopyable
{
 public:
  Test()
  {
    printf("tid=%d, constructing %p\n", kxnet::CurrentThread::tid(), this);
  }

  ~Test()
  {
    printf("tid=%d, destructing %p %s\n", kxnet::CurrentThread::tid(), this, name_.c_str());
  }

  const kxnet::string& name() const { return name_; }
  void setName(const kxnet::string& n) { name_ = n; }

 private:
  kxnet::string name_;
};

#define STL kxnet::Singleton<kxnet::ThreadLocal<Test> >::instance().value()

void print()
{
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &STL,
         STL.name().c_str());
}

void threadFunc(const char* changeTo)
{
  print();
  STL.setName(changeTo);
  sleep(1);
  print();
}

int main()
{
  STL.setName("main one");
  kxnet::Thread t1(std::bind(threadFunc, "thread1"));
  kxnet::Thread t2(std::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  print();
  t2.join();
  pthread_exit(0);
}
