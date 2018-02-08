#include <kxnet/base/Singleton.h>
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

class TestNoDestroy : kxnet::noncopyable
{
 public:
  // Tag member for Singleton<T>
  void no_destroy();

  TestNoDestroy()
  {
    printf("tid=%d, constructing TestNoDestroy %p\n", kxnet::CurrentThread::tid(), this);
  }

  ~TestNoDestroy()
  {
    printf("tid=%d, destructing TestNoDestroy %p\n", kxnet::CurrentThread::tid(), this);
  }
};

void threadFunc()
{
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &kxnet::Singleton<Test>::instance(),
         kxnet::Singleton<Test>::instance().name().c_str());
  kxnet::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
  kxnet::Singleton<Test>::instance().setName("only one");
  kxnet::Thread t1(threadFunc);
  t1.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &kxnet::Singleton<Test>::instance(),
         kxnet::Singleton<Test>::instance().name().c_str());
  kxnet::Singleton<TestNoDestroy>::instance();
  printf("with valgrind, you should see %zd-byte memory leak.\n", sizeof(TestNoDestroy));
}
