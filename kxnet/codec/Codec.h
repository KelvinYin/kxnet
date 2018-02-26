// Copyright 2017, Kelvin Yin.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)
//
// This is a public header file, it must only include public header files.

#pragma once
#include <kxnet/base/Logging.h>
#include <kxnet/net/Buffer.h>
#include <kxnet/net/Endian.h>
#include <kxnet/net/TcpConnection.h>

using namespace std;

namespace kxnet 
{
class LengthHeaderCodec : kxnet::noncopyable
{
 public:
  typedef std::function<void (const kxnet::net::TcpConnectionPtr&,
                                const string& message,
                                kxnet::Timestamp)> StringMessageCallback;

  explicit LengthHeaderCodec(const StringMessageCallback& cb)
    : messageCallback_(cb)
  {
  }

  void onMessage(const kxnet::net::TcpConnectionPtr& conn,
                 kxnet::net::Buffer* buf,
                 kxnet::Timestamp receiveTime);

  void send(const kxnet::net::TcpConnectionPtr& conn,
            const kxnet::StringPiece& message);

 private:
  StringMessageCallback messageCallback_;
};

const int32_t kHeaderLength = 4;
const int32_t kPackageMaxLength = 0x100000; // 64M
const int32_t kCheckSumLength = 2;

uint16_t checksum(const uint16_t *buffer, int size);
bool compress_(uint32_t i, char *buf, size_t &length);
bool uncompress_(char *buf, size_t length, uint32_t &i);
} // namespace
