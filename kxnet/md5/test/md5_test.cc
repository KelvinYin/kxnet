#include <kxnet/md5/MD5.h>

#include <kxnet/base/Timestamp.h>
#include <iostream>
#include <string>
#include <iomanip>

using namespace kxnet;

void benchmark()
{
    string s = "1234567890abcdefghijklmnopqrstuvwxyz 1234567890abcdefghijklmnopqrstuvwxyz";
    int times = 1000 * 1000;
    string val("");
    Timestamp start(Timestamp::now()); 
    for (int i = 0; i < times; ++i)
    {
        val = MD5(s).toStr();
    }
    Timestamp end(Timestamp::now());
    std::cout << "Time consumed: " 
        << timeDifference(end, start) << std::endl;
    std::cout.setf(std::ios::fixed);
    std::cout << "Time per value: " << timeDifference(end, start) / times << std::endl;
}

int main()
{
    // "hello, world" MD5 value is e4d7f1b4ed2e42d15898f4b27b019da4
    string s = "hello, world";
    string mValue = MD5(s).toStr();
    std::cout << "Supposed      : e4d7f1b4ed2e42d15898f4b27b019da4" << std::endl;
    std::cout << "Compute result: " << mValue << std::endl;
    benchmark();
}
