#pragma once
#include <string>
#include <cstring>

using std::string;

namespace kxnet
{
// Parameters of MD5
 const int s11 = 7;
 const int s12 = 12;
 const int s13 = 17;
 const int s14 = 22;
 const int s21 = 5;
 const int s22 = 9;
 const int s23 = 14;
 const int s24 = 20;
 const int s31 = 4;
 const int s32 = 11;
 const int s33 = 16;
 const int s34 = 23;
 const int s41 = 6;
 const int s42 = 10;
 const int s43 = 15;
 const int s44 = 21;

// @Basic MD5 functions.
// 
// @param there bit32.
// @return one bit32.
//
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// @Rotate Left.
// 
// @param {num} the raw number.
// @param {n} rotate left n.
// 
// @return the number after rotated left.
// 
#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))

// @Transformations for rounds 1, 2, 3, and 4.
#define FF(a, b, c, d, x, s, ac) { \
    (a) += F ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATELEFT ((a), (s)); \
    (a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
    (a) += G ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATELEFT ((a), (s)); \
    (a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
    (a) += H ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATELEFT ((a), (s)); \
    (a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
    (a) += I ((b), (c), (d)) + (x) + ac; \
    (a) = ROTATELEFT ((a), (s)); \
    (a) += (b); \
}

// Define of btye
typedef unsigned char byte;
// Define of byte
typedef unsigned int bit32;

class MD5 
{
public:
    // Construct a MD5 object with a string
    MD5(const string& message);

    // Generate md5 digest
    const byte* getDigest();

    // Convert digest to string value
    string toStr();

private:
    // Initialization the md5 object, processing another message block,
    // and updating the context
    void init(const byte* input, size_t len);

    // MD5 basic transformation. Transforms state based on block
    void transform(const byte block[64]);

    // Encodes input (usigned long) into output (byte)
    void encode(const bit32* input, byte* output, size_t length);

    // Decodes input (byte) into output (usigned long)
    void decode(const byte* input, bit32* output, size_t length);

private:
    // Flag for mark whether calculate finished
    bool finished;

    // state (ABCD)
    bit32 state[4];

    // number of bits, low-order word first
    bit32 count[2];

    // input buffer
    byte buffer[64];

    // message digest
    byte digest[16];

    // padding for calculate
    static const byte PADDING[64];

    // Hex numbers
    static const char HEX_NUMBERS[16];
};

}
