#include <kxnet/base/ProcessInfo.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main()
{
  printf("pid = %d\n", kxnet::ProcessInfo::pid());
  printf("uid = %d\n", kxnet::ProcessInfo::uid());
  printf("euid = %d\n", kxnet::ProcessInfo::euid());
  printf("start time = %s\n", kxnet::ProcessInfo::startTime().toFormattedString().c_str());
  printf("hostname = %s\n", kxnet::ProcessInfo::hostname().c_str());
  printf("opened files = %d\n", kxnet::ProcessInfo::openedFiles());
  printf("threads = %zd\n", kxnet::ProcessInfo::threads().size());
  printf("num threads = %d\n", kxnet::ProcessInfo::numThreads());
  printf("status = %s\n", kxnet::ProcessInfo::procStatus().c_str());
}
