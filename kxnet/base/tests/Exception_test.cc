#include <kxnet/base/Exception.h>
#include <stdio.h>

class Bar
{
 public:
  void test()
  {
    throw kxnet::Exception("oops");
  }
};

void foo()
{
  Bar b;
  b.test();
}

int main()
{
  try
  {
    foo();
  }
  catch (const kxnet::Exception& ex)
  {
    printf("reason: %s\n", ex.what());
    printf("stack trace: %s\n", ex.stackTrace());
  }
}
