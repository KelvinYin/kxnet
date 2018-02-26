// Copyright 2017, Kelvin Yin.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)
//
// This is a public header file, it must only include public header files.

#pragma once
#include <kxnet/codec/Codec.h>

#include <string>
#include <stdint.h>

namespace kxnet 
{
class BinaryWriteStream
{
public:
	BinaryWriteStream(std::string *data);
    const char* GetData() const { return data_->data(); }
    size_t GetSize() const { return data_->length(); }
    size_t GetCurrentPos() const{return data_->length();}

    bool Write(const char* str,size_t length);
    bool Write(double value,bool isNULL = false);
    bool Write(long value,bool isNULL = false);
    bool Write(int i,bool isNULL = false);
    bool Write(short i,bool isNULL = false);
    bool Write(char c,bool isNULL = false);
    void Flush();
    void Clear();

private:
    std::string *data_;
};
}
