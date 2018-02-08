// This is a public header file, it must only include public header files.
#pragma once

#include <kxnet/base/StringPiece.h>

namespace kxnet
{

namespace FileUtil
{

// read small file < 64KB
class ReadSmallFile : noncopyable
{
 public:
  ReadSmallFile(StringArg filename);
  ~ReadSmallFile();

  // return errno
  template<typename String>
  int readToString(int maxSize,
                   String* content,
                   int64_t* fileSize,
                   int64_t* modifyTime,
                   int64_t* createTime);

  /// Read at maxium kBufferSize into buf_
  // return errno
  int readToBuffer(int* size);

  const char* buffer() const { return buf_; }

  static const int kBufferSize = 64*1024;

 private:
  int fd_;
  int err_;
  char buf_[kBufferSize];
};

// read the file content, returns errno if error happens.
template<typename String>
int readFile(StringArg filename,
             int maxSize,
             String* content,
             int64_t* fileSize = NULL,
             int64_t* modifyTime = NULL,
             int64_t* createTime = NULL)
{
  ReadSmallFile file(filename);
  return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

// not thread safe
class AppendFile : noncopyable
{
 public:
  explicit AppendFile(StringArg filename);

  ~AppendFile();

  void append(const char* logline, const size_t len);

  void flush();

  size_t writtenBytes() const { return writtenBytes_; }

 private:

  size_t write(const char* logline, size_t len);

  FILE* fp_;
  char buffer_[64*1024];
  size_t writtenBytes_;
};
}

}

