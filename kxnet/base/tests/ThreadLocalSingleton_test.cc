#include <kxnet/base/ThreadLocalSingleton.h>
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

void threadFunc(const char* changeTo)
{
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &kxnet::ThreadLocalSingleton<Test>::instance(),
         kxnet::ThreadLocalSingleton<Test>::instance().name().c_str());
  kxnet::ThreadLocalSingleton<Test>::instance().setName(changeTo);
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &kxnet::ThreadLocalSingleton<Test>::instance(),
         kxnet::ThreadLocalSingleton<Test>::instance().name().c_str());

  // no need to manually delete it
  // kxnet::ThreadLocalSingleton<Test>::destroy();
}

int main()
{
  kxnet::ThreadLocalSingleton<Test>::instance().setName("main one");
  kxnet::Thread t1(std::bind(threadFunc, "thread1"));
  kxnet::Thread t2(std::bind(threadFunc, "thread2"));
  t1.start();
  t2.start();
  t1.join();
  printf("tid=%d, %p name=%s\n",
         kxnet::CurrentThread::tid(),
         &kxnet::ThreadLocalSingleton<Test>::instance(),
         kxnet::ThreadLocalSingleton<Test>::instance().name().c_str());
  t2.join();

  pthread_exit(0);
}
