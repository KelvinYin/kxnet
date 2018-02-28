// This is an internal header file, you should not include this.

#pragma once

#include <functional>

#include <kxnet/net/Channel.h>
#include <kxnet/net/Socket.h>

namespace kxnet
{
namespace net
{

class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
class Acceptor : noncopyable
{
 public:
  typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  bool listenning() const { return listenning_; }
  void listen();

 private:
  void handleRead();

  EventLoop* loop_;
  Socket     acceptSocket_;
  Channel    acceptChannel_;
  bool       listenning_;
  int        idleFd_;
  NewConnectionCallback newConnectionCallback_;
};

}
}

