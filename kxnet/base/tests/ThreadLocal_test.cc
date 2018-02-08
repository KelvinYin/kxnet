#include <kxnet/base/ThreadLocal.h>
#include <kxnet/base/CurrentThread.h>
#include <kxnet/base/Thread.h>

#include <stdio.h>

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

kxnet::ThreadLocal<Test> testObj1;
kxnet::ThreadLocal<Test> testObj2;

void print()
{
  printf("tid=%d, obj1 %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &testObj1.value(),
         testObj1.value().name().c_str());
  printf("tid=%d, obj2 %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &testObj2.value(),
         testObj2.value().name().c_str());
}

void threadFunc()
{
  print();
  testObj1.value().setName("changed 1");
  testObj2.value().setName("changed 42");
  print();
}

int main()
{
  testObj1.value().setName("main one");
  print();
  kxnet::Thread t1(threadFunc);
  t1.start();
  t1.join();
  testObj2.value().setName("main two");
  print();

  pthread_exit(0);
}
