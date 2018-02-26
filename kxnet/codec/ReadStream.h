// Copyright 2017, Kelvin Yin.  All rights reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author:  Kelvin Yin (yinakngxi at gmail dot com)
//
// This is a public header file, it must only include public header files.

#pragma once
#include <kxnet/codec/Codec.h>

#include <stdlib.h>
#include <cassert>
#include <string>
#include <stdint.h>

namespace kxnet 
{
class BinaryReadStream
{
public:
    BinaryReadStream(const char* data, size_t length)
        : data_(data), length_(length), current_(data)
    {
        current_ += kHeaderLength + kCheckSumLength;
    }

    bool IsEmpty() const { return length_ <= kHeaderLength; }
    bool IsEnd() const {
        assert(current_ <= data_ + length_);
        return current_ == data_ + length_;
    }
    const char* GetData() const { return data_; }
    size_t GetSize() const { return length_; }
	const char* GetCurrent() const{ return current_; }

	bool Read(string *str, size_t maxlen, size_t& outlen);
	bool Read(char* str, size_t strlen, size_t& length);
	bool Read(const char** str, size_t maxlen, size_t& outlen);
	bool Read(int &i);
	bool Read(int16_t &i);
	bool Read(char &c);
	size_t ReadAll(char* szBuffer, size_t iLen) const;
	bool ReadLength(size_t& length);
	bool ReadLengthWithoutOffset(size_t& headlen, size_t& outlen);

private:
	const char* const data_;
	const size_t length_;
	const char* current_;
};
}
