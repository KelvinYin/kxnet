#include <kxnet/net/Poller.h>
#include <kxnet/net/poller/PollPoller.h>
#include <kxnet/net/poller/EPollPoller.h>

#include <stdlib.h>

using namespace kxnet::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("MUDUO_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
