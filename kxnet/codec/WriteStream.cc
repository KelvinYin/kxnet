// Copyright 2017, Kelvin Yin. All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)

#include <kxnet/codec/WriteStream.h>

#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

namespace kxnet 
{
BinaryWriteStream::BinaryWriteStream(std::string* data) : 
    data_(data)
{
    data_->clear();
    char str[kHeaderLength + kCheckSumLength];
    data_->append(str,sizeof(str));
}

bool BinaryWriteStream::Write(const char* str,size_t length)
{
    char buf[5];
    size_t buflen;
    compress_(static_cast<uint32_t>(length), buf, buflen);
    data_->append(buf, sizeof(char)*buflen);

    data_->append(str, length);
    return true;
}

bool BinaryWriteStream::Write(int32_t i, bool isNULL)
{
    int32_t i2 = 0;
    if(isNULL == false)
        i2 = htonl(i);
    data_->append(reinterpret_cast<char*>(&i2), sizeof(i2));
    return true;
}

bool BinaryWriteStream::Write(int16_t i, bool isNULL)
{
    int16_t i2 = 0;
    if(isNULL == false)
        i2 = htons(i);
    data_->append(reinterpret_cast<char*>(&i2), sizeof(i2));
    return true;
}

bool BinaryWriteStream::Write(char c, bool isNULL)
{
    char c2 = 0;
    if(isNULL == false)
        c2 = c;
    (*data_) += c2;
    return true;
}

bool BinaryWriteStream::Write(double value,bool isNULL)
{
    char doublestr[128] = {0};
    if(isNULL == false)
    {
        sprintf(doublestr, "%f", value);
        Write(doublestr, strlen(doublestr));
    }
    else
        Write(doublestr,0);
    return true;
}

bool BinaryWriteStream::Write(long value,bool isNULL)
{
    char int64str[128] = {0};
    if(isNULL == false)
    {
        sprintf(int64str, "%ld", value);
        Write(int64str, strlen(int64str));
    }
    else
        Write(int64str, 0);
    return true;
}

void BinaryWriteStream::Flush()
{
    char *ptr = &(*data_)[0];
    unsigned int ulen = htonl(static_cast<unsigned int>(data_->length()));
    memcpy(ptr, &ulen, sizeof(ulen));
}

void BinaryWriteStream::Clear()
{
    data_->clear();
    char str[kHeaderLength + kCheckSumLength];
    data_->append(str, sizeof(str));
}
}
