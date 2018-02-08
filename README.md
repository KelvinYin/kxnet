# kxnet
基于[@Chen Shuo](https://github.com/chenshuo/muduo)的muduo网络库，开发一套通用的服务器程序框架。使开发人员不再拘泥于细节，能够集中精力在业务层面。使用muduo库是因为研读过该库的代码，容易懂而且性能够用，用的人比较多，国内一些公司的开源项目也用到了muduo库，也证明了库的稳定性。

我这里使用了muduo库的C++11版本，C++11中还没有实现Any，muduo是使用了boost::any, kxnet根据网上资料自己实现了一个。
```
C++17 已经有了any的实现，考虑到C++17使用到的g++版本较新，C++11用的已经比较广泛，故采用C++11的分支作为开发基础。
```

部分代码依赖boost库，不过都可以很容易的改掉。
- base/BoundedBlockingQueue.h用到了boost::circular_buffer，后续考虑自己实现CircularBuffer替换boost实现。如果用不到这个队列实现，可以将代码从代码库中移除。
- base/tests/LogStream_test.cc文件中使用了boost::test::unit_test，不关心这部分测试代码的话，直接删除即可。

# TODO
- 实现Circular Buffer
- net部分增加macOS Kqueue支持
- 实现一个简单的HTTP服务器
