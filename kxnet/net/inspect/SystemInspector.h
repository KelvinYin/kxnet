// This is an internal header file, you should not include this.
#pragma once

#include <kxnet/net/inspect/Inspector.h>

namespace kxnet
{
namespace net
{

class SystemInspector : noncopyable
{
 public:
  void registerCommands(Inspector* ins);

  static string overview(HttpRequest::Method, const Inspector::ArgList&);
  static string loadavg(HttpRequest::Method, const Inspector::ArgList&);
  static string version(HttpRequest::Method, const Inspector::ArgList&);
  static string cpuinfo(HttpRequest::Method, const Inspector::ArgList&);
  static string meminfo(HttpRequest::Method, const Inspector::ArgList&);
  static string stat(HttpRequest::Method, const Inspector::ArgList&);
};

}
}

