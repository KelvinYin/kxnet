// Copyright 2017, Kelvin Yin. All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)

#include <kxnet/codec/ReadStream.h>

#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <arpa/inet.h>

namespace kxnet 
{
bool BinaryReadStream::Read(char* str, size_t strlen, /* out */ size_t& outlen)
{
    size_t fieldlen;
    size_t headlen;
    if ( !ReadLengthWithoutOffset(headlen, fieldlen) ) {
        return false;
    }

    // user buffer is not enough
    if (fieldlen > strlen) {
        return false;
    }

    // 偏移到数据的位置
    //current_ += kPackageHeaderLength;	
    current_ += headlen;	
    if(current_ + fieldlen > data_+length_)
    {
        outlen = 0;
        return false;
    }
    memcpy(str, current_, fieldlen);
    outlen = fieldlen;  
    current_ += outlen;    
    return true;
}

bool BinaryReadStream::Read(string *str,size_t maxlen,size_t& outlen)
{
    size_t headlen;
    size_t fieldlen;
    if ( !ReadLengthWithoutOffset(headlen, fieldlen) ) {
        return false;
    }

    // user buffer is not enough
    if (maxlen != 0 && fieldlen > maxlen) {
        return false;
    }

    // 偏移到数据的位置
    //current_ += kPackageHeaderLength;	
    current_ += headlen;	
    if(current_ + fieldlen > data_+length_)
    {
        outlen=0;
        return false;
    }
    str->assign(current_,fieldlen);
    outlen = fieldlen;   
    current_ += outlen;		
    return true;
}

bool BinaryReadStream::Read(const char** str,size_t maxlen,size_t& outlen)
{
    size_t headlen;
    size_t fieldlen;
    if ( !ReadLengthWithoutOffset(headlen, fieldlen) ) {
        return false;
    }
    // user buffer is not enough
    if (maxlen != 0 && fieldlen > maxlen) {
        return false;
    }

    // 偏移到数据的位置
    //current_ += kPackageHeaderLength;	
    current_ += headlen;

    //memcpy(str, current_, fieldlen);
    if(current_ + fieldlen > data_+length_)
    {
        outlen=0;
        return false;
    }
    *str = current_;
    outlen = fieldlen; 
    current_ += outlen;
    return true;
}

bool BinaryReadStream::Read(int &i)
{
    const int VALUE_SIZE = sizeof(int);

    if (current_ + VALUE_SIZE > data_ + length_) {
        return false;
    }

    memcpy(&i, current_, VALUE_SIZE);
    i = ntohl(i);

    current_ += VALUE_SIZE;

    return true;
}

bool BinaryReadStream::Read(int16_t &i)
{
    const int VALUE_SIZE = sizeof(int16_t);

    if (current_ + VALUE_SIZE > data_ + length_) {
        return false;
    }

    memcpy(&i, current_, VALUE_SIZE);
    i = ntohs(i);

    current_ += VALUE_SIZE;

    return true;
}

bool BinaryReadStream::Read(char &c)
{
    const int VALUE_SIZE = sizeof(char);

    if (current_ + VALUE_SIZE > data_ + length_) {
        return false;
    }

    memcpy(&c, current_, VALUE_SIZE);
    current_ += VALUE_SIZE;

    return true;
}

bool BinaryReadStream::ReadLength(size_t & outlen)
{
    size_t headlen;
    if ( !ReadLengthWithoutOffset(headlen, outlen) ) {
        return false;
    }

    //current_ += kPackageHeaderLength;
    current_ += headlen;
    return true;
}

bool BinaryReadStream::ReadLengthWithoutOffset(size_t& headlen, size_t& outlen)
{
    headlen = 0;
    const char *temp = current_;
    char buf[5];
    for (size_t i = 0; i < sizeof(buf); i++)
    {
        memcpy(buf + i, temp, sizeof(char));
        temp++;
        headlen++;

        //if ((buf[i] >> 7 | 0x0) == 0x0)
        if ((buf[i] & 0x80) == 0x00)
            break;
    }
    if (current_ + headlen > data_ +length_) {
        return false;
    }

    unsigned int value; 	
    uncompress_(buf, headlen, value);
    outlen = value;

    return true;
}

size_t BinaryReadStream::ReadAll(char* szBuffer, size_t iLen) const
{
    size_t iRealLen = min(iLen, length_);
    memcpy(szBuffer, data_, iRealLen);
    return iRealLen;
}
}
