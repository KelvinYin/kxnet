#include <kxnet/net/inspect/Inspector.h>
#include <kxnet/net/EventLoop.h>
#include <kxnet/net/EventLoopThread.h>

using namespace kxnet;
using namespace kxnet::net;

int main()
{
  EventLoop loop;
  EventLoopThread t;
  Inspector ins(t.startLoop(), InetAddress(12345), "test");
  loop.loop();
}

